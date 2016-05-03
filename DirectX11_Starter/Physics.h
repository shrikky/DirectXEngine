#pragma once

#define _XM_NO_INTRINSICS_ 1;

#include "btBulletDynamicsCommon.h"
#include <DirectXMath.h>
using namespace DirectX;

static class Physics
{
public:
	Physics();
	~Physics();

	void CreatePhysicsWorld();

	

	//XMFLOAT3 worldAabbMin;
	//XMFLOAT3 worldAabbMax;
	//static btDiscreteDynamicsWorld* mpDynamicsWorld;


	//btDefaultCollisionConfiguration * mpCollisionConfiguration ;
	//btCollisionDispatcher * mpDispatcher ;


	//int maxProxies;
	//btAxisSweep3* mpOverlappingPairCache;

	/////the default constraint solver. For parallel processing you can use
	//// a different solver (see Extras/BulletMultiThreaded)
	//btSequentialImpulseConstraintSolver * mpSolver;



	btBroadphaseInterface* broadphase;

	// Set up the collision configuration and dispatcher
	btDefaultCollisionConfiguration* collisionConfiguration ;
	btCollisionDispatcher* dispatcher ;

	// The actual physics solver
	btSequentialImpulseConstraintSolver* solver;

	// The world.
	btDiscreteDynamicsWorld* dynamicsWorld ;


	//for ground

	btCollisionShape* groundShape;
	btDefaultMotionState* groundMotionState ;

	btRigidBody* groundRigidBody ;


};

