#ifndef FLOAT_MATH_H

#define FLOAT_MATH_H

// a set of routines that last you do common 3d math
// operations without any vector, matrix, or quaternion
// classes or templates.
//
// a vector (or point) is a 'float *' to 3 floating point numbers.
// a matrix is a 'float *' to an array of 16 floating point numbers representing a 4x4 transformation matrix compatible with D3D or OGL
// a quaternion is a 'float *' to 4 floats representing a quaternion x,y,z,w

const float FM_PI = 3.141592654f;
const float FM_DEG_TO_RAD = ((2.0f * FM_PI) / 360.0f);
const float FM_RAD_TO_DEG = (360.0f / (2.0f * FM_PI));

void  fm_identity(float *matrix); // set 4x4 matrix to identity.
void  fm_inverseRT(const float *matrix,const float *pos,float *t); // inverse rotate translate the point.
void  fm_eulerMatrix(float ax,float ay,float az,float *matrix); // convert euler (in radians) to a dest 4x4 matrix (translation set to zero)
void  fm_getAABB(unsigned int vcount,const float *points,unsigned int pstride,float *bmin,float *bmax);
void  fm_eulerToQuat(float roll,float pitch,float yaw,float *quat); // convert euler angles to quaternion.
void  fm_quatToMatrix(const float *quat,float *matrix); // convert quaterinion rotation to matrix, translation set to zero.
void  fm_quatRotate(const float *quat,const float *v,float *r); // rotate a vector directly by a quaternion.
void  fm_getTranslation(const float *matrix,float *t);
void  fm_matrixToQuat(const float *matrix,float *quat); // convert the 3x3 portion of a 4x4 matrix into a quaterion as x,y,z,w
float fm_sphereVolume(float radius); // return's the volume of a sphere of this radius (4/3 PI * R cubed )

#endif
