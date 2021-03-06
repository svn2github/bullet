/*
Bullet Continuous Collision Detection and Physics Library
Copyright (c) 2003-2006 Erwin Coumans  http://continuousphysics.com/Bullet/

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#ifndef btOdeJoint_H
#define btOdeJoint_H

struct btOdeSolverBody;
class btOdeJoint;

#include "LinearMath/btScalar.h"

struct BU_ContactJointNode {
  btOdeJoint *joint;		// pointer to enclosing btOdeJoint object
  btOdeSolverBody* body;			// *other* body this joint is connected to
};
typedef btScalar dVector3[4];


class btOdeJoint  {
  
public:
	// naming convention: the "first" body this is connected to is node[0].body,
  // and the "second" body is node[1].body. if this joint is only connected
  // to one body then the second body is 0.

  // info returned by getInfo1 function. the constraint dimension is m (<=6).
  // i.e. that is the total number of rows in the jacobian. `nub' is the
  // number of unbounded variables (which have lo,hi = -/+ infinity).

  btOdeJoint();
  virtual ~btOdeJoint();
  

  struct Info1 {
    int m_numConstraintRows,nub;
  };

  // info returned by getInfo2 function

  struct Info2 {
    // integrator parameters: frames per second (1/stepsize), default error
    // reduction parameter (0..1).
    btScalar fps,erp;

    // for the first and second body, pointers to two (linear and angular)
    // n*3 jacobian sub matrices, stored by rows. these matrices will have
    // been initialized to 0 on entry. if the second body is zero then the
    // J2xx pointers may be 0.
    btScalar *m_J1linearAxis,*m_J1angularAxis,*m_J2linearAxis,*m_J2angularAxis;

    // elements to jump from one row to the next in J's
    int rowskip;

    // right hand sides of the equation J*v = c + cfm * lambda. cfm is the
    // "constraint force mixing" vector. c is set to zero on entry, cfm is
    // set to a constant value (typically very small or zero) value on entry.
    btScalar *m_constraintError,*cfm;

    // lo and hi limits for variables (set to -/+ infinity on entry).
    btScalar *m_lowerLimit,*m_higherLimit;

    // findex vector for variables. see the LCP solver interface for a
    // description of what this does. this is set to -1 on entry.
    // note that the returned indexes are relative to the first index of
    // the constraint.
    int *findex;
  };

  // virtual function table: size of the joint structure, function pointers.
  // we do it this way instead of using C++ virtual functions because
  // sometimes we need to allocate joints ourself within a memory pool.

  virtual void GetInfo1 (Info1 *info)=0;
  virtual void GetInfo2 (Info2 *info)=0;

  int flags;			// dJOINT_xxx flags
  BU_ContactJointNode node[2];		// connections to bodies. node[1].body can be 0
   btScalar lambda[6];		// lambda generated by last step
};


#endif //btOdeJoint_H
