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

#ifndef COMPOUND_COLLISION_ALGORITHM_H
#define COMPOUND_COLLISION_ALGORITHM_H

#include "btActivatingCollisionAlgorithm.h"
#include "BulletCollision/BroadphaseCollision/btDispatcher.h"
#include "BulletCollision/BroadphaseCollision/btBroadphaseInterface.h"
#include "BulletCollision/NarrowPhaseCollision/btPersistentManifold.h"
#include "BulletCollision/BroadphaseCollision/btBroadphaseProxy.h"
#include "btCollisionCreateFunc.h"
#include "LinearMath/btAlignedObjectArray.h"
#include "BulletCollision/CollisionDispatch/btCollisionWorld.h"

class btDispatcher;
struct btCollider;
class btCollisionObject;

/// btCompoundCollisionAlgorithm  supports collision between CompoundCollisionShapes and other collision shapes
class btCompoundCollisionAlgorithm  : public btActivatingCollisionAlgorithm
{
	btAlignedObjectArray<btCollisionAlgorithm*> m_childCollisionAlgorithms;
	bool m_isSwapped;
	int	m_compoundShapeRevision;//to keep track of changes, so that childAlgorithm array can be updated
	btManifoldArray m_manifoldArray;
	
	void	removeChildAlgorithms(btDispatcher* dispatcher);
	
	void	preallocateChildAlgorithms(btDispatcher* dispatcher, const btCollider& body0,const btCollider& body1);

public:

	btCompoundCollisionAlgorithm( const btCollisionAlgorithmConstructionInfo& ci);

	virtual ~btCompoundCollisionAlgorithm();
	virtual void nihilize(btDispatcher* dispatcher);
	virtual void processCollision (const btCollisionProcessInfo& processInfo);

	btScalar	calculateTimeOfImpact(btCollisionObject* body0,btCollisionObject* body1,const btDispatcherInfo& dispatchInfo,btManifoldResult* resultOut);

	virtual	void	getAllContactManifolds(btManifoldArray&	manifoldArray)
	{
		int i;
		for (i=0;i<m_childCollisionAlgorithms.size();i++)
		{
			if (m_childCollisionAlgorithms[i])
				m_childCollisionAlgorithms[i]->getAllContactManifolds(manifoldArray);
		}
	}

	static void performConvexcast (
		btCollisionWorld::ConvexCastInfo& convexCastInfo,
		const btCollider& compoundCollider,
		const btVector3& boxSource, 
		const btVector3& boxTarget, 
		const btVector3& aabbMin, 
		const btVector3& aabbMax);

	struct CreateFunc :public 	btCollisionAlgorithmCreateFunc
	{
		virtual	btCollisionAlgorithm* CreateCollisionAlgorithm(btCollisionAlgorithmConstructionInfo& ci)
		{
			void* mem = ci.m_dispatcher1->allocateCollisionAlgorithm(sizeof(btCompoundCollisionAlgorithm));
			return new(mem) btCompoundCollisionAlgorithm(ci);
		}
	};

	struct SwappedCreateFunc :public 	btCollisionAlgorithmCreateFunc
	{
		virtual	btCollisionAlgorithm* CreateCollisionAlgorithm(btCollisionAlgorithmConstructionInfo& ci)
		{
			ci.m_isSwapped = true;
			void* mem = ci.m_dispatcher1->allocateCollisionAlgorithm(sizeof(btCompoundCollisionAlgorithm));
			return new(mem) btCompoundCollisionAlgorithm(ci);
		}
	};

};

#endif //COMPOUND_COLLISION_ALGORITHM_H
