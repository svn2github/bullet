#ifndef BEST_FIT_H

#define BEST_FIT_H

// This routine was released in 'snippet' form
// by John W. Ratcliff mailto:jratcliff@infiniplex.net
// on March 22, 2006.
//
// This routine computes the 'best fit' plane equation to
// a set of input data points with an optional per vertex
// weighting component.
//
// The implementation for this was lifted directly from
// David Eberly's Magic Software implementation.

// computes the best fit plane to a collection of data points.
// returns the plane equation as A,B,C,D format. (Ax+By+Cz+D)

bool getBestFitPlane(unsigned int vcount,     // number of input data points
                     const float *points,     // starting address of points array.
                     unsigned int vstride,    // stride between input points.
                     const float *weights,    // *optional point weighting values.
                     unsigned int wstride,    // weight stride for each vertex.
                     float *plane);


float getBoundingRegion(unsigned int vcount,const float *points,unsigned int pstride,float *bmin,float *bmax); // returns the diagonal distance
bool  overlapAABB(const float *bmin1,const float *bmax1,const float *bmin2,const float *bmax2); // return true if the two AABB's overlap.

#endif
