#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../ConvexDecomposition/convexdecomposition.h"
#include "../ConvexDecomposition/cd_wavefront.h"

using namespace ConvexDecomposition;

// Test application to demonstrate how to use the ConvexDecomposition system.

class CBuilder : public ConvexDecompInterface
{
public:

  CBuilder(const char *fname,const DecompDesc &d)
  {
  	char scratch[512];
  	strcpy(scratch,fname);
  	char *dot = strstr(scratch,".");
  	if ( dot ) *dot = 0;
  	strcat(scratch,"_convex.obj");
  	mBaseCount = 0;
  	mHullCount = 0;
  	mFph = fopen(scratch,"wb");


		printf("########################################################################\r\n");
  	printf("# Perfomring approximate convex decomposition for triangle mesh %s\r\n", fname );
  	printf("# Input Mesh has %d vertices and %d triangles.\r\n", d.mVcount, d.mTcount );
  	printf("# Recursion depth: %d\r\n", d.mDepth );
  	printf("# Concavity Threshold Percentage: %0.2f\r\n", d.mCpercent );
  	printf("# Hull Merge Volume Percentage: %0.2f\r\n", d.mPpercent );
  	printf("# Maximum output vertices per hull: %d\r\n", d.mMaxVertices );
  	printf("# Hull Skin Width: %0.2f\r\n", d.mSkinWidth );
		printf("########################################################################\r\n");


  	if ( mFph )
  	{
			fprintf(mFph,"########################################################################\r\n");
  		fprintf(mFph,"# Approximate convex decomposition for triangle mesh %s\r\n", fname );
  		fprintf(mFph,"# Input Mesh has %d vertices and %d triangles.\r\n", d.mVcount, d.mTcount );
  		fprintf(mFph,"# Recursion depth: %d\r\n", d.mDepth );
  		fprintf(mFph,"# Concavity Threshold Percentage: %0.2f\r\n", d.mCpercent );
  		fprintf(mFph,"# Hull Merge Volume Percentage: %0.2f\r\n", d.mPpercent );
  		fprintf(mFph,"# Maximum output vertices per hull: %d\r\n", d.mMaxVertices );
  		fprintf(mFph,"# Hull Skin Width: %0.2f\r\n", d.mSkinWidth );
			fprintf(mFph,"########################################################################\r\n");

		  printf("Opened Wavefront file %s for output.\r\n", scratch );
  	}
  	else
  	{
  		printf("Failed to open output file %s\r\n", scratch );
  	}
  }

	~CBuilder(void)
	{
		if ( mFph )
		{
			fclose(mFph);
		}
	}

  virtual void ConvexDecompResult(ConvexResult &result)
  {
  	// we have a hull...
  	mHullCount++;

  	printf("Received hull %d  HullVolume(%0.3f) OBBVolume(%0.3f) SphereVolume(%0.3f)\r\n", mHullCount, result.mHullVolume, result.mOBBVolume, result.mSphereVolume );

		if ( mFph )
		{
			fprintf(mFph,"########################################################################\r\n");
			fprintf(mFph,"## Hull Piece %d with %d vertices and %d triangles.\r\n", mHullCount, result.mHullVcount, result.mHullTcount );
			fprintf(mFph,"########################################################################\r\n");

			for (unsigned int i=0; i<result.mHullVcount; i++)
			{
				const float *p = &result.mHullVertices[i*3];
				fprintf(mFph,"v %0.9f %0.9f %0.9f\r\n", p[0], p[1], p[2] );
			}

			if ( 1 )
			{
				const unsigned int *src = result.mHullIndices;
				for (unsigned int i=0; i<result.mHullTcount; i++)
				{
					unsigned int i1 = *src++;
					unsigned int i2 = *src++;
					unsigned int i3 = *src++;

          i1+=mBaseCount;
          i2+=mBaseCount;
          i3+=mBaseCount;

				 fprintf(mFph,"f %d %d %d\r\n", i1+1, i2+1, i3+1 );
				}
			}
			mBaseCount+=result.mHullVcount; // advance the 'base index' counter.
		}

  }

	unsigned int  mHullCount;
  FILE         *mFph;
  unsigned int  mBaseCount;
};


void main(int argc,const char **argv)
{
	if ( argc < 2 )
	{
		printf("Usage: Test <meshanme.obj> (options)\r\n");
		printf("\r\n");
		printf("Options:\r\n");
		printf("\r\n");
		printf("            -d<depth>       : How deep to recursively split. Values of 3-7 are reasonable.\r\n");
		printf("            -c<percent>     : Percentage of concavity to keep splitting. 0-20% is reasonable.\r\n");
		printf("            -p<percent>     : Percentage of volume delta to collapse hulls.  0-30% is reasonable.\r\n");
		printf("            -v<maxverts>    : Maximum number of vertices in the output hull.  Default is 32.\r\n");
		printf("            -s<skinwidth>   : Skin Width inflation.  Default is 0.\r\n");

	}
	else
	{
		unsigned int depth = 5;
		float cpercent     = 5;
		float ppercent     = 5;
		unsigned int maxv  = 32;
		float skinWidth    = 0;

		// process command line switches.
		for (int i=2; i<argc; i++)
		{
			const char *o = argv[i];

			if ( strncmp(o,"-d",2) == 0 )
			{
				depth = (unsigned int) atoi( &o[2] );
				if ( depth < 0 || depth > 10 )
				{
					depth = 5;
					printf("Invalid depth value in switch, defaulting to 5.\r\n");
				}
			}

			if ( strncmp(o,"-c",2) == 0 )
			{
				cpercent = (float) atof( &o[2] );
				if ( cpercent < 0 || cpercent > 100 )
				{
					cpercent = 5;
					printf("Invalid concavity percentage in switch, defaulting to 5.\r\n");
				}
			}

			if ( strncmp(o,"-p",2) == 0 )
			{
				ppercent = (float) atof( &o[2] );
				if ( ppercent < 0 || ppercent > 100 )
				{
					ppercent = 5;
					printf("Invalid hull merge percentage in switch, defaulting to 5.\r\n");
				}
			}

			if ( strncmp(o,"-v",2) == 0 )
			{
				maxv = (unsigned int) atoi( &o[2] );
				if ( maxv < 8 || maxv > 256 )
				{
					maxv = 32;
					printf("Invalid max vertices in switch, defaulting to 32.\r\n");
				}
			}


			if ( strncmp(o,"-s",2) == 0 )
			{
				skinWidth = (float) atof( &o[2] );
				if ( skinWidth < 0 || skinWidth > 0.1f )
				{
					skinWidth = 0;
					printf("Invalid skinWidth in switch, defaulting to 0.\r\n");
				}
			}

		}

		WavefrontObj wo;

    unsigned int tcount = wo.loadObj(argv[1]);

    if ( tcount )
    {


    	DecompDesc d;

      d.mVcount       =	wo.mVertexCount;
      d.mVertices     = wo.mVertices;
      d.mTcount       = wo.mTriCount;
      d.mIndices      = (unsigned int *)wo.mIndices;
      d.mDepth        = depth;
      d.mCpercent     = cpercent;
      d.mPpercent     = ppercent;
      d.mMaxVertices  = maxv;
      d.mSkinWidth    = skinWidth;


    	CBuilder   cb(argv[1],d); // receives the answers and writes out a wavefront OBJ file.

      d.mCallback     = &cb;



			unsigned int count = performConvexDecomposition(d);

			if ( count )
			{
				printf("Input triangle mesh with %d triangles produced %d output hulls.\r\n", d.mTcount, count );
			}
			else
			{
				printf("Failed to produce any convex hulls.\r\n");
			}

    }
    else
    {
    	// sorry..no
    	printf("Sorry unable to load file '%s'\r\n", argv[1] );
    }

	}

}
