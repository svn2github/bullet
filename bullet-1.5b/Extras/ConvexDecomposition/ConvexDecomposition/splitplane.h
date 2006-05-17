#ifndef SPLIT_PLANE_H

#define SPLIT_PLANE_H


namespace ConvexDecomposition
{

class ConvexDecompInterface;

bool computeSplitPlane(unsigned int vcount,
                       const float *vertices,
                       unsigned int tcount,
                       const unsigned int *indices,
                       ConvexDecompInterface *callback,
                       float *plane);


};

#endif
