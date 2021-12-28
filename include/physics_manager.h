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
		/// adds the rigid body to the mDynamicsWorld to be handled within Bullet as well
		void add_ridgid_body(btRigidBody* ridg);
		
		/// updates all of our rigid bodys and any collisions between them
		void update(float dt);

		/// registers the listener into the queue to be notified when there is a collision
		void register_listener(ssuge::CollisionListener* listener);

		/// removes the listener from the update list
		void deregister_listener(ssuge::CollisionListener* listener);

		/// will notify the objects if there was a collision between the two and they are registered listeners
		void broadcast_collision(ssuge::Collider* collider, ssuge::Collider* hit_collider);

		/// adds the listener to the queue to be removed
		void queue_listener_for_removal(ssuge::CollisionListener* listener) { mListenerRemoveQueue.push_back(listener); }
		
		/// purges the ListenersRemoveQueue and removes the desired listeners
		void purge_listeners();

		/// removes the rigidbody from the DynamicsWorld so Bullets stops simulating it behind the scenes
		void remove_rigid_body(btRigidBody* body) { mDynamicsWorld->removeRigidBody(body); }
		

	};
}