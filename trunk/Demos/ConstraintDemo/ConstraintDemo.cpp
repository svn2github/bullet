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




#include "btBulletDynamicsCommon.h"
#include "LinearMath/btIDebugDraw.h"

#include "GLDebugDrawer.h"

#include "GLDebugFont.h"
#include <stdio.h> //printf debugging

#include "ConstraintDemo.h"
#include "GL_ShapeDrawer.h"
#include "GlutStuff.h"

const int numObjects = 3;

#define ENABLE_ALL_DEMOS 1

#define CUBE_HALF_EXTENTS 1.f

#define M_PI 3.1415926f
#define M_PI_2 ((M_PI)*0.5f)
#define M_PI_4 ((M_PI)*0.25f)

btTransform sliderTransform;
btVector3 lowerSliderLimit = btVector3(-10,0,0);
btVector3 hiSliderLimit = btVector3(10,0,0);

btRigidBody* d6body0 =0;

btHingeConstraint* spDoorHinge = NULL;
btHingeConstraint* spHingeDynAB = NULL;
btGeneric6DofConstraint* spSlider6Dof = NULL;

static bool s_bTestConeTwistMotor = false;



void	drawLimit()
{
		btVector3 from = sliderTransform*lowerSliderLimit;
		btVector3 to = sliderTransform*hiSliderLimit;
		btVector3 color(255,0,0);
		glBegin(GL_LINES);
		glColor3f(color.getX(), color.getY(), color.getZ());
		glVertex3d(from.getX(), from.getY(), from.getZ());
		glVertex3d(to.getX(), to.getY(), to.getZ());
		if (d6body0)
		{
			from = d6body0->getWorldTransform().getOrigin();
			to = from + d6body0->getWorldTransform().getBasis() * btVector3(0,0,10);
			glVertex3d(from.getX(), from.getY(), from.getZ());
			glVertex3d(to.getX(), to.getY(), to.getZ());
		}
		glEnd();
}



void	ConstraintDemo::initPhysics()
{
	setTexturing(true);
	setShadows(true);

	setCameraDistance(26.f);
	m_Time = 0;

	m_collisionConfiguration = new btDefaultCollisionConfiguration();
	m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);
	btVector3 worldMin(-1000,-1000,-1000);
	btVector3 worldMax(1000,1000,1000);
	m_overlappingPairCache = new btAxisSweep3(worldMin,worldMax);
	m_constraintSolver = new btSequentialImpulseConstraintSolver();
	m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher,m_overlappingPairCache,m_constraintSolver,m_collisionConfiguration);


	btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(50.),btScalar(40.),btScalar(50.)));
	m_collisionShapes.push_back(groundShape);
	btTransform groundTransform;
	groundTransform.setIdentity();
	groundTransform.setOrigin(btVector3(0,-56,0));
	btRigidBody* groundBody = localCreateRigidBody(0, groundTransform, groundShape);



	btCollisionShape* shape = new btBoxShape(btVector3(CUBE_HALF_EXTENTS,CUBE_HALF_EXTENTS,CUBE_HALF_EXTENTS));
	m_collisionShapes.push_back(shape);
	btTransform trans;
	trans.setIdentity();
	trans.setOrigin(btVector3(0,20,0));

	float mass = 1.f;
#if ENABLE_ALL_DEMOS
	//point to point constraint (ball socket)
	{
		btRigidBody* body0 = localCreateRigidBody( mass,trans,shape);
		trans.setOrigin(btVector3(2*CUBE_HALF_EXTENTS,20,0));

		mass = 1.f;
		btRigidBody* body1 = 0;//localCreateRigidBody( mass,trans,shape);
//		btRigidBody* body1 = localCreateRigidBody( 0.0,trans,0);
		//body1->setActivationState(DISABLE_DEACTIVATION);
		//body1->setDamping(0.3,0.3);

		btVector3 pivotInA(CUBE_HALF_EXTENTS,-CUBE_HALF_EXTENTS,-CUBE_HALF_EXTENTS);
		btVector3 axisInA(0,0,1);

		btVector3 pivotInB = body1 ? body1->getCenterOfMassTransform().inverse()(body0->getCenterOfMassTransform()(pivotInA)) : pivotInA;
		btVector3 axisInB = body1? 
			(body1->getCenterOfMassTransform().getBasis().inverse()*(body1->getCenterOfMassTransform().getBasis() * axisInA)) : 
		body0->getCenterOfMassTransform().getBasis() * axisInA;

		//btTypedConstraint* p2p = new btPoint2PointConstraint(*body0,*body1,pivotInA,pivotInB);
		//btTypedConstraint* hinge = new btHingeConstraint(*body0,*body1,pivotInA,pivotInB,axisInA,axisInB);
		btHingeConstraint* hinge = new btHingeConstraint(*body0,pivotInA,axisInA);
		
		//use zero targetVelocity and a small maxMotorImpulse to simulate joint friction
		//float	targetVelocity = 0.f;
		//float	maxMotorImpulse = 0.01;
		float	targetVelocity = 1.f;
		float	maxMotorImpulse = 1.0f;
		hinge->enableAngularMotor(true,targetVelocity,maxMotorImpulse);

		m_dynamicsWorld->addConstraint(hinge);//p2p);
//		m_dynamicsWorld->addConstraint(p2p);
		hinge->setDbgDrawSize(btScalar(5.f));

	}
#endif

#if ENABLE_ALL_DEMOS	
	//create a slider, using the generic D6 constraint
	{
		mass = 1.f;
		btVector3 sliderWorldPos(0,10,0);
		btVector3 sliderAxis(1,0,0);
		btScalar angle=0.f;//SIMD_RADS_PER_DEG * 10.f;
		btMatrix3x3 sliderOrientation(btQuaternion(sliderAxis ,angle));
		trans.setIdentity();
		trans.setOrigin(sliderWorldPos);
		//trans.setBasis(sliderOrientation);
		sliderTransform = trans;

		d6body0 = localCreateRigidBody( mass,trans,shape);
		d6body0->setActivationState(DISABLE_DEACTIVATION);
		btRigidBody* fixedBody1 = localCreateRigidBody(0,trans,0);
		m_dynamicsWorld->addRigidBody(fixedBody1);

		btTransform frameInA, frameInB;
		frameInA = btTransform::getIdentity();
		frameInB = btTransform::getIdentity();
		frameInA.setOrigin(btVector3(0., 5., 0.));
		frameInB.setOrigin(btVector3(0., 5., 0.));

//		bool useLinearReferenceFrameA = false;//use fixed frame B for linear llimits
		bool useLinearReferenceFrameA = true;//use fixed frame A for linear llimits
		spSlider6Dof = new btGeneric6DofConstraint(*fixedBody1, *d6body0,frameInA,frameInB,useLinearReferenceFrameA);
		spSlider6Dof->setLinearLowerLimit(lowerSliderLimit);
		spSlider6Dof->setLinearUpperLimit(hiSliderLimit);

		//range should be small, otherwise singularities will 'explode' the constraint
//		spSlider6Dof->setAngularLowerLimit(btVector3(-1.5,0,0));
//		spSlider6Dof->setAngularUpperLimit(btVector3(1.5,0,0));
//		spSlider6Dof->setAngularLowerLimit(btVector3(0,0,0));
//		spSlider6Dof->setAngularUpperLimit(btVector3(0,0,0));
		spSlider6Dof->setAngularLowerLimit(btVector3(-SIMD_PI,0,0));
		spSlider6Dof->setAngularUpperLimit(btVector3(1.5,0,0));

		spSlider6Dof->getTranslationalLimitMotor()->m_enableMotor[0] = true;
		spSlider6Dof->getTranslationalLimitMotor()->m_targetVelocity[0] = -5.0f;
		spSlider6Dof->getTranslationalLimitMotor()->m_maxMotorForce[0] = 0.1f;


		m_dynamicsWorld->addConstraint(spSlider6Dof);
		spSlider6Dof->setDbgDrawSize(btScalar(5.f));

	}
#endif
#if ENABLE_ALL_DEMOS
	{ // create a door using hinge constraint attached to the world
		btCollisionShape* pDoorShape = new btBoxShape(btVector3(2.0f, 5.0f, 0.2f));
		m_collisionShapes.push_back(pDoorShape);
		btTransform doorTrans;
		doorTrans.setIdentity();
		doorTrans.setOrigin(btVector3(-5.0f, -2.0f, 0.0f));
		btRigidBody* pDoorBody = localCreateRigidBody( 1.0, doorTrans, pDoorShape);
		pDoorBody->setActivationState(DISABLE_DEACTIVATION);
		const btVector3 btPivotA(10.f +  2.1f, -2.0f, 0.0f ); // right next to the door slightly outside
		btVector3 btAxisA( 0.0f, 1.0f, 0.0f ); // pointing upwards, aka Y-axis

		spDoorHinge = new btHingeConstraint( *pDoorBody, btPivotA, btAxisA );

//		spDoorHinge->setLimit( 0.0f, M_PI_2 );
		// test problem values
//		spDoorHinge->setLimit( -M_PI, M_PI*0.8f);

//		spDoorHinge->setLimit( 1.f, -1.f);
//		spDoorHinge->setLimit( -M_PI*0.8f, M_PI);
//		spDoorHinge->setLimit( -M_PI*0.8f, M_PI, 0.9f, 0.3f, 0.0f);
//		spDoorHinge->setLimit( -M_PI*0.8f, M_PI, 0.9f, 0.01f, 0.0f); // "sticky limits"
		spDoorHinge->setLimit( -M_PI * 0.25f, M_PI * 0.25f );
//		spDoorHinge->setLimit( 0.0f, 0.0f );
		m_dynamicsWorld->addConstraint(spDoorHinge);
		spDoorHinge->setDbgDrawSize(btScalar(5.f));

		//doorTrans.setOrigin(btVector3(-5.0f, 2.0f, 0.0f));
		//btRigidBody* pDropBody = localCreateRigidBody( 10.0, doorTrans, shape);
	}
#endif
#if ENABLE_ALL_DEMOS
	{ // create a generic 6DOF constraint

		btTransform tr;
		tr.setIdentity();
		tr.setOrigin(btVector3(btScalar(10.), btScalar(6.), btScalar(0.)));
		tr.getBasis().setEulerZYX(0,0,0);
//		btRigidBody* pBodyA = localCreateRigidBody( mass, tr, shape);
		btRigidBody* pBodyA = localCreateRigidBody( 0.0, tr, shape);
//		btRigidBody* pBodyA = localCreateRigidBody( 0.0, tr, 0);
		pBodyA->setActivationState(DISABLE_DEACTIVATION);

		tr.setIdentity();
		tr.setOrigin(btVector3(btScalar(0.), btScalar(6.), btScalar(0.)));
		tr.getBasis().setEulerZYX(0,0,0);
		btRigidBody* pBodyB = localCreateRigidBody(mass, tr, shape);
//		btRigidBody* pBodyB = localCreateRigidBody(0.f, tr, shape);
		pBodyB->setActivationState(DISABLE_DEACTIVATION);

		btTransform frameInA, frameInB;
		frameInA = btTransform::getIdentity();
		frameInA.setOrigin(btVector3(btScalar(-5.), btScalar(0.), btScalar(0.)));
		frameInB = btTransform::getIdentity();
		frameInB.setOrigin(btVector3(btScalar(5.), btScalar(0.), btScalar(0.)));

		btGeneric6DofConstraint* pGen6DOF = new btGeneric6DofConstraint(*pBodyA, *pBodyB, frameInA, frameInB, true);
//		btGeneric6DofConstraint* pGen6DOF = new btGeneric6DofConstraint(*pBodyA, *pBodyB, frameInA, frameInB, false);
		pGen6DOF->setLinearLowerLimit(btVector3(-10., -2., -1.));
		pGen6DOF->setLinearUpperLimit(btVector3(10., 2., 1.));
//		pGen6DOF->setLinearLowerLimit(btVector3(-10., 0., 0.));
//		pGen6DOF->setLinearUpperLimit(btVector3(10., 0., 0.));
//		pGen6DOF->setLinearLowerLimit(btVector3(0., 0., 0.));
//		pGen6DOF->setLinearUpperLimit(btVector3(0., 0., 0.));

//		pGen6DOF->getTranslationalLimitMotor()->m_enableMotor[0] = true;
//		pGen6DOF->getTranslationalLimitMotor()->m_targetVelocity[0] = 5.0f;
//		pGen6DOF->getTranslationalLimitMotor()->m_maxMotorForce[0] = 0.1f;


//		pGen6DOF->setAngularLowerLimit(btVector3(0., SIMD_HALF_PI*0.9, 0.));
//		pGen6DOF->setAngularUpperLimit(btVector3(0., -SIMD_HALF_PI*0.9, 0.));
//		pGen6DOF->setAngularLowerLimit(btVector3(0., 0., -SIMD_HALF_PI));
//		pGen6DOF->setAngularUpperLimit(btVector3(0., 0., SIMD_HALF_PI));

		pGen6DOF->setAngularLowerLimit(btVector3(-SIMD_HALF_PI * 0.5f, -0.75, -SIMD_HALF_PI * 0.8f));
		pGen6DOF->setAngularUpperLimit(btVector3(SIMD_HALF_PI * 0.5f, 0.75, SIMD_HALF_PI * 0.8f));
//		pGen6DOF->setAngularLowerLimit(btVector3(0.f, -0.75, SIMD_HALF_PI * 0.8f));
//		pGen6DOF->setAngularUpperLimit(btVector3(0.f, 0.75, -SIMD_HALF_PI * 0.8f));
//		pGen6DOF->setAngularLowerLimit(btVector3(0.f, -SIMD_HALF_PI * 0.8f, SIMD_HALF_PI * 1.98f));
//		pGen6DOF->setAngularUpperLimit(btVector3(0.f, SIMD_HALF_PI * 0.8f,  -SIMD_HALF_PI * 1.98f));

		
		
//		pGen6DOF->setAngularLowerLimit(btVector3(-0.75,-0.5, -0.5));
//		pGen6DOF->setAngularUpperLimit(btVector3(0.75,0.5, 0.5));
//		pGen6DOF->setAngularLowerLimit(btVector3(-0.75,0., 0.));
//		pGen6DOF->setAngularUpperLimit(btVector3(0.75,0., 0.));
//		pGen6DOF->setAngularLowerLimit(btVector3(0., -0.7,0.));
//		pGen6DOF->setAngularUpperLimit(btVector3(0., 0.7, 0.));
//		pGen6DOF->setAngularLowerLimit(btVector3(-1., 0.,0.));
//		pGen6DOF->setAngularUpperLimit(btVector3(1., 0., 0.));

		m_dynamicsWorld->addConstraint(pGen6DOF, true);
		pGen6DOF->setDbgDrawSize(btScalar(5.f));
	}
#endif
#if ENABLE_ALL_DEMOS
	{ // create a ConeTwist constraint

		btTransform tr;
		tr.setIdentity();
		tr.setOrigin(btVector3(btScalar(-10.), btScalar(5.), btScalar(0.)));
		tr.getBasis().setEulerZYX(0,0,0);
		btRigidBody* pBodyA = localCreateRigidBody( 1.0, tr, shape);
//		btRigidBody* pBodyA = localCreateRigidBody( 0.0, tr, shape);
		pBodyA->setActivationState(DISABLE_DEACTIVATION);

		tr.setIdentity();
		tr.setOrigin(btVector3(btScalar(-10.), btScalar(-5.), btScalar(0.)));
		tr.getBasis().setEulerZYX(0,0,0);
		btRigidBody* pBodyB = localCreateRigidBody(0.0, tr, shape);
//		btRigidBody* pBodyB = localCreateRigidBody(1.0, tr, shape);

		btTransform frameInA, frameInB;
		frameInA = btTransform::getIdentity();
		frameInA.getBasis().setEulerZYX(0, 0, M_PI_2);
		frameInA.setOrigin(btVector3(btScalar(0.), btScalar(-5.), btScalar(0.)));
		frameInB = btTransform::getIdentity();
		frameInB.getBasis().setEulerZYX(0,0,  M_PI_2);
		frameInB.setOrigin(btVector3(btScalar(0.), btScalar(5.), btScalar(0.)));

		m_ctc = new btConeTwistConstraint(*pBodyA, *pBodyB, frameInA, frameInB);
//		m_ctc->setLimit(btScalar(M_PI_4), btScalar(M_PI_4), btScalar(M_PI) * 0.8f);
//		m_ctc->setLimit(btScalar(M_PI_4*0.6f), btScalar(M_PI_4), btScalar(M_PI) * 0.8f, 1.0f); // soft limit == hard limit
		m_ctc->setLimit(btScalar(M_PI_4*0.6f), btScalar(M_PI_4), btScalar(M_PI) * 0.8f, 0.5f);
		m_dynamicsWorld->addConstraint(m_ctc, true);
		m_ctc->setDbgDrawSize(btScalar(5.f));
		// s_bTestConeTwistMotor = true; // use only with old solver for now
		s_bTestConeTwistMotor = false;
	}
#endif
#if ENABLE_ALL_DEMOS
	{ // Hinge connected to the world, with motor (to hinge motor with new and old constraint solver)
		btTransform tr;
		tr.setIdentity();
		tr.setOrigin(btVector3(btScalar(0.), btScalar(0.), btScalar(0.)));
		btRigidBody* pBody = localCreateRigidBody( 1.0, tr, shape);
		pBody->setActivationState(DISABLE_DEACTIVATION);
		const btVector3 btPivotA( 10.0f, 0.0f, 0.0f );
		btVector3 btAxisA( 0.0f, 0.0f, 1.0f );

		btHingeConstraint* pHinge = new btHingeConstraint( *pBody, btPivotA, btAxisA );
//		pHinge->enableAngularMotor(true, -1.0, 0.165); // use for the old solver
		pHinge->enableAngularMotor(true, -1.0, 1.65); // use for the new SIMD solver
		m_dynamicsWorld->addConstraint(pHinge);
		pHinge->setDbgDrawSize(btScalar(5.f));
	}
#endif

#if ENABLE_ALL_DEMOS
	{ 
		// create a universal joint using generic 6DOF constraint
		// create two rigid bodies
		// static bodyA (parent) on top:
		btTransform tr;
		tr.setIdentity();
		tr.setOrigin(btVector3(btScalar(20.), btScalar(4.), btScalar(0.)));
		btRigidBody* pBodyA = localCreateRigidBody( 0.0, tr, shape);
		pBodyA->setActivationState(DISABLE_DEACTIVATION);
		// dynamic bodyB (child) below it :
		tr.setIdentity();
		tr.setOrigin(btVector3(btScalar(20.), btScalar(0.), btScalar(0.)));
		btRigidBody* pBodyB = localCreateRigidBody(1.0, tr, shape);
		pBodyB->setActivationState(DISABLE_DEACTIVATION);
		// add some (arbitrary) data to build constraint frames
		btVector3 parentAxis(1.f, 0.f, 0.f); 
		btVector3 childAxis(0.f, 0.f, 1.f); 
		btVector3 anchor(20.f, 2.f, 0.f);

		btUniversalConstraint* pUniv = new btUniversalConstraint(*pBodyA, *pBodyB, anchor, parentAxis, childAxis);
		pUniv->setLowerLimit(-SIMD_HALF_PI * 0.5f, -SIMD_HALF_PI * 0.5f);
		pUniv->setUpperLimit(SIMD_HALF_PI * 0.5f,  SIMD_HALF_PI * 0.5f);
		// add constraint to world
		m_dynamicsWorld->addConstraint(pUniv, true);
		// draw constraint frames and limits for debugging
		pUniv->setDbgDrawSize(btScalar(5.f));
	}
#endif

#if ENABLE_ALL_DEMOS
	{ // create a generic 6DOF constraint with springs 

		btTransform tr;
		tr.setIdentity();
		tr.setOrigin(btVector3(btScalar(-20.), btScalar(16.), btScalar(0.)));
		tr.getBasis().setEulerZYX(0,0,0);
		btRigidBody* pBodyA = localCreateRigidBody( 0.0, tr, shape);
		pBodyA->setActivationState(DISABLE_DEACTIVATION);

		tr.setIdentity();
		tr.setOrigin(btVector3(btScalar(-10.), btScalar(16.), btScalar(0.)));
		tr.getBasis().setEulerZYX(0,0,0);
		btRigidBody* pBodyB = localCreateRigidBody(1.0, tr, shape);
		pBodyB->setActivationState(DISABLE_DEACTIVATION);

		btTransform frameInA, frameInB;
		frameInA = btTransform::getIdentity();
		frameInA.setOrigin(btVector3(btScalar(10.), btScalar(0.), btScalar(0.)));
		frameInB = btTransform::getIdentity();
		frameInB.setOrigin(btVector3(btScalar(0.), btScalar(0.), btScalar(0.)));

		btGeneric6DofSpringConstraint* pGen6DOFSpring = new btGeneric6DofSpringConstraint(*pBodyA, *pBodyB, frameInA, frameInB, true);
		pGen6DOFSpring->setLinearUpperLimit(btVector3(5., 0., 0.));
		pGen6DOFSpring->setLinearLowerLimit(btVector3(-5., 0., 0.));

		pGen6DOFSpring->setAngularLowerLimit(btVector3(0.f, 0.f, -1.5f));
		pGen6DOFSpring->setAngularUpperLimit(btVector3(0.f, 0.f, 1.5f));

		m_dynamicsWorld->addConstraint(pGen6DOFSpring, true);
		pGen6DOFSpring->setDbgDrawSize(btScalar(5.f));
		
		pGen6DOFSpring->enableSpring(0, true);
		pGen6DOFSpring->setStiffness(0, 39.478f);
		pGen6DOFSpring->setDamping(0, 0.5f);
		pGen6DOFSpring->enableSpring(5, true);
		pGen6DOFSpring->setStiffness(5, 39.478f);
		pGen6DOFSpring->setDamping(0, 0.3f);
		pGen6DOFSpring->setEquilibriumPoint();
	}
#endif
#if ENABLE_ALL_DEMOS
	{ 
		// create a Hinge2 joint
		// create two rigid bodies
		// static bodyA (parent) on top:
		btTransform tr;
		tr.setIdentity();
		tr.setOrigin(btVector3(btScalar(-20.), btScalar(4.), btScalar(0.)));
		btRigidBody* pBodyA = localCreateRigidBody( 0.0, tr, shape);
		pBodyA->setActivationState(DISABLE_DEACTIVATION);
		// dynamic bodyB (child) below it :
		tr.setIdentity();
		tr.setOrigin(btVector3(btScalar(-20.), btScalar(0.), btScalar(0.)));
		btRigidBody* pBodyB = localCreateRigidBody(1.0, tr, shape);
		pBodyB->setActivationState(DISABLE_DEACTIVATION);
		// add some data to build constraint frames
		btVector3 parentAxis(0.f, 1.f, 0.f); 
		btVector3 childAxis(1.f, 0.f, 0.f); 
		btVector3 anchor(-20.f, 0.f, 0.f);
		btHinge2Constraint* pHinge2 = new btHinge2Constraint(*pBodyA, *pBodyB, anchor, parentAxis, childAxis);
		pHinge2->setLowerLimit(-SIMD_HALF_PI * 0.5f);
		pHinge2->setUpperLimit( SIMD_HALF_PI * 0.5f);
		// add constraint to world
		m_dynamicsWorld->addConstraint(pHinge2, true);
		// draw constraint frames and limits for debugging
		pHinge2->setDbgDrawSize(btScalar(5.f));
	}
#endif
#if  ENABLE_ALL_DEMOS
	{ 
		// create a Hinge joint between two dynamic bodies
		// create two rigid bodies
		// static bodyA (parent) on top:
		btTransform tr;
		tr.setIdentity();
		tr.setOrigin(btVector3(btScalar(-20.), btScalar(-2.), btScalar(0.)));
		btRigidBody* pBodyA = localCreateRigidBody( 1.0f, tr, shape);
		pBodyA->setActivationState(DISABLE_DEACTIVATION);
		// dynamic bodyB:
		tr.setIdentity();
		tr.setOrigin(btVector3(btScalar(-30.), btScalar(-2.), btScalar(0.)));
		btRigidBody* pBodyB = localCreateRigidBody(10.0, tr, shape);
		pBodyB->setActivationState(DISABLE_DEACTIVATION);
		// add some data to build constraint frames
		btVector3 axisA(0.f, 1.f, 0.f); 
		btVector3 axisB(0.f, 1.f, 0.f); 
		btVector3 pivotA(-5.f, 0.f, 0.f);
		btVector3 pivotB( 5.f, 0.f, 0.f);
		spHingeDynAB = new btHingeConstraint(*pBodyA, *pBodyB, pivotA, pivotB, axisA, axisB);
		spHingeDynAB->setLimit(-SIMD_HALF_PI * 0.5f, SIMD_HALF_PI * 0.5f);
		// add constraint to world
		m_dynamicsWorld->addConstraint(spHingeDynAB, true);
		// draw constraint frames and limits for debugging
		spHingeDynAB->setDbgDrawSize(btScalar(5.f));
	}
#endif
}

ConstraintDemo::~ConstraintDemo()
{
	//cleanup in the reverse order of creation/initialization

	int i;

	//removed/delete constraints
	for (i=m_dynamicsWorld->getNumConstraints()-1; i>=0 ;i--)
	{
		btTypedConstraint* constraint = m_dynamicsWorld->getConstraint(i);
		m_dynamicsWorld->removeConstraint(constraint);
		delete constraint;
	}
	m_ctc = NULL;

	//remove the rigidbodies from the dynamics world and delete them
	for (i=m_dynamicsWorld->getNumCollisionObjects()-1; i>=0 ;i--)
	{
		btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}
		m_dynamicsWorld->removeCollisionObject( obj );
		delete obj;
	}




	//delete collision shapes
	for (int j=0;j<m_collisionShapes.size();j++)
	{
		btCollisionShape* shape = m_collisionShapes[j];
		delete shape;
	}

	//delete dynamics world
	delete m_dynamicsWorld;

	//delete solver
	delete m_constraintSolver;

	//delete broadphase
	delete m_overlappingPairCache;

	//delete dispatcher
	delete m_dispatcher;

	delete m_collisionConfiguration;


}


void ConstraintDemo::clientMoveAndDisplay()
{
	
 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

 	float dt = float(getDeltaTimeMicroseconds()) * 0.000001f;
	//printf("dt = %f: ",dt);

	// drive cone-twist motor
	m_Time += 0.03f;
	if (s_bTestConeTwistMotor)
	{  // this works only for obsolete constraint solver for now
		// build cone target
		btScalar t = 1.25f*m_Time;
		btVector3 axis(0,sin(t),cos(t));
		axis.normalize();
		btQuaternion q1(axis, 0.75f*M_PI);

		// build twist target
		//btQuaternion q2(0,0,0);
		//btQuaternion q2(btVehictor3(1,0,0), -0.3*sin(m_Time));
		btQuaternion q2(btVector3(1,0,0), -1.49*sin(1.5*m_Time));

		// compose cone + twist and set target
		q1 = q1 * q2;
		m_ctc->enableMotor(true);
		m_ctc->setMotorTargetInConstraintSpace(q1);
	}

	
	{
	 	//during idle mode, just run 1 simulation step maximum
		int maxSimSubSteps = m_idle ? 1 : 1;
		if (m_idle)
			dt = 1.0/420.f;

		int numSimSteps = m_dynamicsWorld->stepSimulation(dt,maxSimSubSteps);

		//optional but useful: debug drawing
		m_dynamicsWorld->debugDrawWorld();
	
		bool verbose = false;
		if (verbose)
		{
			if (!numSimSteps)
				printf("Interpolated transforms\n");
			else
			{
				if (numSimSteps > maxSimSubSteps)
				{
					//detect dropping frames
					printf("Dropped (%i) simulation steps out of %i\n",numSimSteps - maxSimSubSteps,numSimSteps);
				} else
				{
					printf("Simulated (%i) steps\n",numSimSteps);
				}
			}
		}
	}
	renderme();

//	drawLimit();

    glFlush();
    glutSwapBuffers();
}




void ConstraintDemo::displayCallback(void) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

	if (m_dynamicsWorld)
		m_dynamicsWorld->debugDrawWorld();

//	drawLimit();

	renderme();

    glFlush();
    glutSwapBuffers();
}


void ConstraintDemo::keyboardCallback(unsigned char key, int x, int y)
{
	(void)x;
	(void)y;
	switch (key) 
	{
		case 'O' :
			{
				bool offectOnOff;
				if(spDoorHinge)
				{
					offectOnOff = spDoorHinge->getUseFrameOffset();
					offectOnOff = !offectOnOff;
					spDoorHinge->setUseFrameOffset(offectOnOff);
					printf("DoorHinge %s frame offset\n", offectOnOff ? "uses" : "does not use");
				}
				if(spHingeDynAB)
				{
					offectOnOff = spHingeDynAB->getUseFrameOffset();
					offectOnOff = !offectOnOff;
					spHingeDynAB->setUseFrameOffset(offectOnOff);
					printf("HingeDynAB %s frame offset\n", offectOnOff ? "uses" : "does not use");
				}
				if(spSlider6Dof)
				{
					offectOnOff = spSlider6Dof->getUseFrameOffset();
					offectOnOff = !offectOnOff;
					spSlider6Dof->setUseFrameOffset(offectOnOff);
					printf("Slider6Dof %s frame offset\n", offectOnOff ? "uses" : "does not use");
				}
			}
			break;
		default : 
			{
				DemoApplication::keyboardCallback(key, x, y);
			}
			break;
	}
}
