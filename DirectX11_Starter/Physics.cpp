#include "Physics.h"
#include "btBulletDynamicsCommon.h"



Physics::Physics()
{
	CreatePhysicsWorld();
}


Physics::~Physics()
{

	delete broadphase ;
	delete collisionConfiguration;
	delete dispatcher;
	delete solver;
	delete dynamicsWorld;

}



void Physics::CreatePhysicsWorld()
{

	 broadphase = new btDbvtBroadphase();

	// Set up the collision configuration and dispatcher
	 collisionConfiguration = new btDefaultCollisionConfiguration();
	 dispatcher = new btCollisionDispatcher(collisionConfiguration);

	// The actual physics solver
	 solver = new btSequentialImpulseConstraintSolver();

	// The world.
   dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0.1, 0, 0));


	//create a ground
	// groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);
	//groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));
	//btRigidBody::btRigidBodyConstructionInfo
	//	groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
	// groundRigidBody = new btRigidBody(groundRigidBodyCI);
	//dynamicsWorld->addRigidBody(groundRigidBody);

}