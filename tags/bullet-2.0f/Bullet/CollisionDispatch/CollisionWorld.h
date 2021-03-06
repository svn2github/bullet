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


/**
 * @mainpage Bullet Documentation
 *
 * @section intro_sec Introduction
 * Bullet Collision Detection & Physics SDK
 *
 * Bullet is a Collision Detection and Rigid Body Dynamics Library. The Library is Open Source and free for commercial use, under the ZLib license ( http://opensource.org/licenses/zlib-license.php ).
 *
 * There is the Physics Forum for Feedback and General Collision Detection and Physics discussions.
 * Please visit http://www.continuousphysics.com/Bullet/phpBB2/index.php
 *
 * @section install_sec Installation
 *
 * @subsection step1 Step 1: Download
 * You can download the Bullet Physics Library from our website: http://www.continuousphysics.com/Bullet/
 * @subsection step2 Step 2: Building
 * Bullet comes with autogenerated Project Files for Microsoft Visual Studio 6, 7, 7.1 and 8.
 * The main Workspace/Solution is located in Bullet/msvc/8/wksbullet.sln (replace 8 with your version).
 * 
 * Under other platforms, like Linux or Mac OS-X, Bullet can be build using either using cmake, http://www.cmake.org, or jam, http://www.perforce.com/jam/jam.html . cmake can autogenerate Xcode, KDevelop, MSVC and other build systems. just run cmake . in the root of Bullet.
 * Jam is a build system that can build the library, demos and also autogenerate the MSVC Project Files.
 * So if you are not using MSVC, you can run configure and jam .
 * If you don't have jam installed, you can make jam from the included jam-2.5 sources, or download jam from ftp://ftp.perforce.com/pub/jam/
 * 
 * @subsection step3 Step 3: Testing demos
 * Try to run and experiment with CcdPhysicsDemo executable as a starting point.
 * Bullet can be used in several ways, as Full Rigid Body simulation, as Collision Detector Library or Low Level / Snippets like the GJK Closest Point calculation.
 * The Dependencies can be seen in this documentation under Directories
 * 
 * @subsection step4 Step 4: Integrating in your application, Full Rigid Body Simulation
 * Check out CcdPhysicsDemo how to create a CcdPhysicsEnvironment , CollisionShape and RigidBody, Stepping the simulation and synchronizing your derived version of the PHY_IMotionState class.
 * @subsection step5 Step 5 : Integrate the Collision Detection Library (without Dynamics and other Extras)
 * Bullet Collision Detection can also be used without the Dynamics/Extras.
 * Check out CollisionWorld and CollisionObject, and the CollisionInterfaceDemo. Also in Extras/test_BulletOde.cpp there is a sample Collision Detection integration with Open Dynamics Engine, ODE, http://www.ode.org
 * @subsection step6 Step 6 : Use Snippets like the GJK Closest Point calculation.
 * Bullet has been designed in a modular way keeping dependencies to a minimum. The ConvexHullDistance demo demonstrates direct use of GjkPairDetector.
 *
 * @section copyright Copyright
 * Copyright (C) 2005-2006 Erwin Coumans, some contributions Copyright Gino van den Bergen, Christer Ericson, Simon Hobbs, Ricardo Padrela, F Richter(res), Stephane Redon
 * Special thanks to all visitors of the Bullet Physics forum, and in particular above contributors, Dave Eberle, Dirk Gregorius, Erin Catto, Dave Eberle, Adam Moravanszky,
 * Pierre Terdiman, Kenny Erleben, Russell Smith, Oliver Strunk, Jan Paul van Waveren.
 * 
 */
 
 

#ifndef COLLISION_WORLD_H
#define COLLISION_WORLD_H


class CollisionShape;
class BroadphaseInterface;
#include "SimdVector3.h"
#include "SimdTransform.h"
#include "CollisionObject.h"
#include "CollisionDispatcher.h" //for definition of CollisionObjectArray
#include "BroadphaseCollision/OverlappingPairCache.h"

#include <vector>



///CollisionWorld is interface and container for the collision detection
class CollisionWorld
{

	

	std::vector<CollisionObject*>	m_collisionObjects;
	
	Dispatcher*	m_dispatcher1;

	OverlappingPairCache*	m_pairCache;
	

public:

	CollisionWorld(Dispatcher* dispatcher,OverlappingPairCache* pairCache)
		:m_dispatcher1(dispatcher),
		m_pairCache(pairCache)
	{

	}
	virtual ~CollisionWorld();


	BroadphaseInterface*	GetBroadphase()
	{
		return m_pairCache;
	}

	OverlappingPairCache*	GetPairCache()
	{
		return m_pairCache;
	}


	Dispatcher*	GetDispatcher()
	{
		return m_dispatcher1;
	}

	///LocalShapeInfo gives extra information for complex shapes
	///Currently, only TriangleMeshShape is available, so it just contains triangleIndex and subpart
	struct	LocalShapeInfo
	{
		int	m_shapePart;
		int	m_triangleIndex;
		
		//const CollisionShape*	m_shapeTemp;
		//const SimdTransform*	m_shapeLocalTransform;
	};

	struct	LocalRayResult
	{
		LocalRayResult(const CollisionObject*	collisionObject, 
			LocalShapeInfo*	localShapeInfo,
			const SimdVector3&		hitNormalLocal,
			float hitFraction)
		:m_collisionObject(collisionObject),
		m_localShapeInfo(m_localShapeInfo),
		m_hitNormalLocal(hitNormalLocal),
		m_hitFraction(hitFraction)
		{
		}

		const CollisionObject*	m_collisionObject;
		LocalShapeInfo*			m_localShapeInfo;
		const SimdVector3&		m_hitNormalLocal;
		float					m_hitFraction;

	};

	///RayResultCallback is used to report new raycast results
	struct	RayResultCallback
	{
		virtual ~RayResultCallback()
		{
		}
		float	m_closestHitFraction;
		bool	HasHit()
		{
			return (m_closestHitFraction < 1.f);
		}

		RayResultCallback()
			:m_closestHitFraction(1.f)
		{
		}
		virtual	float	AddSingleResult(const LocalRayResult& rayResult) = 0;
	};

	struct	ClosestRayResultCallback : public RayResultCallback
	{
		ClosestRayResultCallback(SimdVector3	rayFromWorld,SimdVector3	rayToWorld)
		:m_rayFromWorld(rayFromWorld),
		m_rayToWorld(rayToWorld),
		m_collisionObject(0)
		{
		}

		SimdVector3	m_rayFromWorld;//used to calculate hitPointWorld from hitFraction
		SimdVector3	m_rayToWorld;

		SimdVector3	m_hitNormalWorld;
		SimdVector3	m_hitPointWorld;
		const CollisionObject*	m_collisionObject;
		
		virtual	float	AddSingleResult(const LocalRayResult& rayResult)
		{

//caller already does the filter on the m_closestHitFraction
			assert(rayResult.m_hitFraction <= m_closestHitFraction);
			
			m_closestHitFraction = rayResult.m_hitFraction;
			m_collisionObject = rayResult.m_collisionObject;
			m_hitNormalWorld = m_collisionObject->m_worldTransform.getBasis()*rayResult.m_hitNormalLocal;
			m_hitPointWorld.setInterpolate3(m_rayFromWorld,m_rayToWorld,rayResult.m_hitFraction);
			return rayResult.m_hitFraction;
		}
	};


	

	int	GetNumCollisionObjects() const
	{
		return m_collisionObjects.size();
	}

	/// RayTest performs a raycast on all objects in the CollisionWorld, and calls the resultCallback
	/// This allows for several queries: first hit, all hits, any hit, dependent on the value returned by the callback.
	void	RayTest(const SimdVector3& rayFromWorld, const SimdVector3& rayToWorld, RayResultCallback& resultCallback);

	/// RayTestSingle performs a raycast call and calls the resultCallback. It is used internally by RayTest.
	/// In a future implementation, we consider moving the ray test as a virtual method in CollisionShape.
	/// This allows more customization.
	void	RayTestSingle(const SimdTransform& rayFromTrans,const SimdTransform& rayToTrans,
					  CollisionObject* collisionObject,
					  const CollisionShape* collisionShape,
					  const SimdTransform& colObjWorldTransform,
					  RayResultCallback& resultCallback);

	void	AddCollisionObject(CollisionObject* collisionObject,short int collisionFilterGroup=1,short int collisionFilterMask=1);

	CollisionObjectArray& GetCollisionObjectArray()
	{
		return m_collisionObjects;
	}

	const CollisionObjectArray& GetCollisionObjectArray() const
	{
		return m_collisionObjects;
	}


	void	RemoveCollisionObject(CollisionObject* collisionObject);

	virtual void	PerformDiscreteCollisionDetection();

};


#endif //COLLISION_WORLD_H
