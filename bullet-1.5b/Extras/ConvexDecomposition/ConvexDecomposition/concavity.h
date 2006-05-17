#ifndef COMPUTE_CONCAVITY_H

#define COMPUTE_CONCAVITY_H


namespace ConvexDecomposition
{

class ConvexDecompInterface;

// compute's how 'concave' this object is and returns the total volume of the
// convex hull as well as the volume of the 'concavity' which was found.
float computeConcavity(unsigned int vcount,
                       const float *vertices,
                       unsigned int tcount,
                       const unsigned int *indices,
                       ConvexDecompInterface *callback,
                       float *plane,
                       float &volume);


};


#endif
