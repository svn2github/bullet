#ifndef PLANE_TRI_H

#define PLANE_TRI_H


enum PlaneTriResult
{
  PTR_FRONT,
  PTR_BACK,
  PTR_SPLIT,
};

PlaneTriResult planeTriIntersection(const float *plane,    // the plane equation in Ax+By+Cz+D format
                                    const float *triangle, // the source position triangle.
                                    unsigned int tstride,  // stride in bytes between vertices of the triangle.
                                    float        epsilon,  // the co-planer epsilon value.
                                    float       *front,    // the triangle in front of the
                                    unsigned int &fcount,  // number of vertices in the 'front' triangle.
                                    float       *back,      // the triangle in back of the plane
                                    unsigned int &bcount);  // the number of vertices in the 'back' triangle.


#endif
