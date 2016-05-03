#pragma once
#include "btBulletDynamicsCommon.h"
#include <DirectXMath.h>
#include <d3d11.h>

static class Physics
{
public:
	Physics();
	~Physics();

	void CreatePhysicsWorld();

	

	XMFLOAT3 worldAabbMin = XMFLOAT3(-100.0f, -100.0f, -100.0f);
	XMFLOAT3 worldAabbMax = XMFLOAT3(100.0f, 100.0f, 100.0f);
	static btDiscreteDynamicsWorld* mpDynamicsWorld;


	btDefaultCollisionConfiguration * mpCollisionConfiguration ;
	btCollisionDispatcher * mpDispatcher ;


	int maxProxies = 1024;
	btAxisSweep3* mpOverlappingPairCache;

	///the default constraint solver. For parallel processing you can use
	// a different solver (see Extras/BulletMultiThreaded)
	btSequentialImpulseConstraintSolver * mpSolver;


};

