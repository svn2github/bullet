#ifndef RAY_TRI_H

#define RAY_TRI_H


// returns true if the ray intersects the triangle.
bool lineIntersectsTriangle(const float *rayStart,const float *rayEnd,const float *p1,const float *p2,const float *p3,float *sect);
bool rayIntersectsTriangle(const float *p,const float *d,const float *v0,const float *v1,const float *v2,float &t);

#endif
