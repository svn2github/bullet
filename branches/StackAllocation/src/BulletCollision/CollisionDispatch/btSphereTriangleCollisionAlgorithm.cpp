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


#include "btSphereTriangleCollisionAlgorithm.h"
#include "BulletCollision/CollisionDispatch/btCollisionDispatcher.h"
#include "BulletCollision/CollisionShapes/btSphereShape.h"
#include "BulletCollision/CollisionDispatch/btCollisionObject.h"
#include "SphereTriangleDetector.h"


btSphereTriangleCollisionAlgorithm::btSphereTriangleCollisionAlgorithm(const btCollisionAlgorithmConstructionInfo& ci)
: btActivatingCollisionAlgorithm(ci),
m_swapped(ci.m_isSwapped)
{
}

btSphereTriangleCollisionAlgorithm::~btSphereTriangleCollisionAlgorithm()
{
}

void btSphereTriangleCollisionAlgorithm::processCollision (const btCollisionProcessInfo& processInfo)
{
	if (!m_manifoldPtr)
		return;

	const btCollider& sphereObj = m_swapped? processInfo.m_body1 : processInfo.m_body0;
	const btCollider& triObj = m_swapped? processInfo.m_body0 : processInfo.m_body1;

	const btSphereShape* sphere = (const btSphereShape*)sphereObj.getCollisionShape();
	const btTriangleShape* triangle = (const btTriangleShape*)triObj.getCollisionShape();
	
	/// report a contact. internally this will be kept persistent, and contact reduction is done
	processInfo.m_result->setPersistentManifold(m_manifoldPtr);
	SphereTriangleDetector detector(sphere,triangle, m_manifoldPtr->getContactBreakingThreshold());
	
	btDiscreteCollisionDetectorInterface::ClosestPointInput input;
	input.m_maximumDistanceSquared = btScalar(BT_LARGE_FLOAT);///@todo: tighter bounds
	input.m_transformA = sphereObj.getWorldTransform();
	input.m_transformB = triObj.getWorldTransform();

	bool swapResults = m_swapped;

	detector.getClosestPoints(input,*processInfo.m_result,processInfo.m_dispatchInfo.m_debugDraw,swapResults);

	if (m_ownManifold)
		processInfo.m_result->refreshContactPoints();
	
}

btScalar btSphereTriangleCollisionAlgorithm::calculateTimeOfImpact(btCollisionObject* col0,btCollisionObject* col1,const btDispatcherInfo& dispatchInfo,btManifoldResult* resultOut)
{
	(void)resultOut;
	(void)dispatchInfo;
	(void)col0;
	(void)col1;

	//not yet
	return btScalar(1.);
}
