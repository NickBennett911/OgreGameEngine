#include <stdafx.h>
#include <physics_manager.h>
#include <game_object.h>

ssuge::PhysicsManager* ssuge::Singleton<ssuge::PhysicsManager>::msSingleton = NULL;		/// this allows to set the msSingleton in singleton class 

ssuge::PhysicsManager::PhysicsManager() {
	///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
	mCollisionConfiguration = new btDefaultCollisionConfiguration();

	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	mDispatcher = new btCollisionDispatcher(mCollisionConfiguration);

	///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	mOverlappingPairCache = new btDbvtBroadphase();

	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	mSolver = new btSequentialImpulseConstraintSolver;

	mDynamicsWorld = new btDiscreteDynamicsWorld(mDispatcher, mOverlappingPairCache, mSolver, mCollisionConfiguration);
}

ssuge::PhysicsManager::~PhysicsManager() {
	delete mCollisionConfiguration;
	delete mDispatcher;
	delete mOverlappingPairCache;
	delete mSolver;
	delete mDynamicsWorld;
}

void ssuge::PhysicsManager::add_ridgid_body(btRigidBody* ridgbody) {
	mDynamicsWorld->addRigidBody(ridgbody);
}

void ssuge::PhysicsManager::update(float dt) {
	mDynamicsWorld->stepSimulation(dt, 10);

    // get all contacts to detect collisions which are represented as manifolds
    // in bullet
    const int num_manifolds = mDispatcher->getNumManifolds();
    for (int manifold_idx = 0; manifold_idx < num_manifolds; ++manifold_idx) {
        const btPersistentManifold* const manifold = mDispatcher->getManifoldByIndexInternal(manifold_idx);

        const btRigidBody* const object1 = static_cast<const btRigidBody*>(manifold->getBody0());
        const btRigidBody* const object2 = static_cast<const btRigidBody*>(manifold->getBody1());

		ssuge::Collider* collider1 = (ssuge::Collider*)object1->getUserPointer();
		ssuge::Collider* collider2 = (ssuge::Collider*)object2->getUserPointer();

 		if (collider1 && collider2) {
			broadcast_collision(collider1, collider2);
		}
    }
	if (mListenerRemoveQueue.size()) { 
		purge_listeners();
	}
}

void ssuge::PhysicsManager::register_listener(ssuge::CollisionListener* list) {
	mListeners.insert(list);
}

void ssuge::PhysicsManager::deregister_listener(ssuge::CollisionListener* listener) {
	if (mListeners.find(listener) != mListeners.end()) {	// if listener is actually registered already
		mListeners.erase(listener);
	}
}

void ssuge::PhysicsManager::broadcast_collision(ssuge::Collider* collider, ssuge::Collider* hit_collider) {
	for (ssuge::CollisionListener* listener : mListeners) {
		std::string curname = listener->get_gobj()->get_name();	// get name of the current listener game object

		if (curname == collider->get_owner()->get_name()) {	// if hitting obj is found to be a listener we try and send that notification out
			listener->collision_happened(hit_collider);
		}
		if (curname == hit_collider->get_owner()->get_name()) { // if hit obj is found to be a listener we try and send that notication out
			listener->collision_happened(collider);
		}
	}
}

void ssuge::PhysicsManager::purge_listeners() {
	for (ssuge::CollisionListener* listener : mListenerRemoveQueue) {
		PHYSICS_MANAGER->deregister_listener(listener);
	}
	mListenerRemoveQueue.clear();
}
