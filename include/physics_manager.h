#pragma once
#include <stdafx.h>
#include <singleton.h>
#include <collider.h>
#include <box_collider.h>
#include <collision_listener.h>

#define PHYSICS_MANAGER ssuge::PhysicsManager::get_singleton_ptr()

namespace ssuge {
	class PhysicsManager : public Singleton<PhysicsManager>
	{

		// ***** ATTRIBUTES *****
		///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
		btDefaultCollisionConfiguration* mCollisionConfiguration;

		///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
		btCollisionDispatcher* mDispatcher;

		///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
		btBroadphaseInterface* mOverlappingPairCache;

		///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
		btSequentialImpulseConstraintSolver* mSolver;

		btDiscreteDynamicsWorld* mDynamicsWorld;

		std::set<ssuge::CollisionListener*> mListeners;

		std::vector<ssuge::CollisionListener*> mListenerRemoveQueue;

	public:

		// ***** CONSTRUCTOR / DESTRUCTOR *****
	public:
		/// Constructor
		PhysicsManager();

		/// Destructor
		~PhysicsManager();

		// ***** GETTERS / SETTERS *****
	public:
		void set_physics_gravity(float gx, float gy, float gz) { mDynamicsWorld->setGravity(btVector3(gx, gy, gz)); }

		// ***** METHODS ***** 
	public:
		void add_ridgid_body(btRigidBody* ridg);
		void update(float dt);
		void register_listener(ssuge::CollisionListener* listener);
		void deregister_listener(ssuge::CollisionListener* listener);
		void broadcast_collision(ssuge::Collider* collider, ssuge::Collider* hit_collider);
		void queue_listener_for_removal(ssuge::CollisionListener* listener) { mListenerRemoveQueue.push_back(listener); }
		void purge_listeners();
		void remove_rigid_body(btRigidBody* body) { mDynamicsWorld->removeRigidBody(body); }


	protected:

		

	};
}