#include "Physics.h"
#include "btBulletDynamicsCommon.h"



Physics::Physics()
{
}


Physics::~Physics()
{
}


void Physics::CreatePhysicsWorld()
{
	///collision configuration contains default setup for memory.
	// Advanced users can create their own configuration.
	 mpCollisionConfiguration = new btDefaultCollisionConfiguration();

	///use the default collision dispatcher. For parallel processing you
	// can use a different dispatcher (see Extras/BulletMultiThreaded)
	mpDispatcher = new btCollisionDispatcher(mpCollisionConfiguration);

	mpOverlappingPairCache = new btAxisSweep3(worldAabbMin, worldAabbMax,
		maxProxies);

	///the default constraint solver. For parallel processing you can use
	// a different solver (see Extras/BulletMultiThreaded)
	mpSolver = new btSequentialImpulseConstraintSolver();

	mpDynamicsWorld = new btDiscreteDynamicsWorld(mpDispatcher,
		mpOverlappingPairCache, mpSolver, mpCollisionConfiguration);

	mpDynamicsWorld->setGravity(btVector3(0, -10, 0));
}