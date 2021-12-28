#pragma once
#include <stdafx.h>
#include <physics_manager.h>
#include <collider.h>

namespace ssuge {

	class SphereCollider : public Collider {
		///---ATTRIBUTES---///
		friend class PhysicsManager;
	public:

		btRigidBody* mRigidBody;

	protected:
		btCollisionShape* mCollisionBody;
		bool mIs_kinematic;
		btDefaultMotionState* myMotionState;
		int mMass;

		/// ---CONSTRUCTOR/DESTRUCTOR---///
	public:
		SphereCollider(ssuge::GameObject* owner, float radius, float mass, bool is_kinematic);
		~SphereCollider();

		/// ---GETTERS/SETTERS--- ///
		virtual ssuge::ComponentType get_type() { return ssuge::ComponentType::BOX_PHYS; }	// must get overrode from the children component

		///---OTHER METHODS---///
		void update(float dt) override;
	};
}