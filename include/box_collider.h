#pragma once
#include <stdafx.h>
#include <collider.h>
#include <physics_manager.h>
#include <log_manager.h>

namespace ssuge {

	class BoxCollider : public Collider {
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


		BoxCollider(ssuge::GameObject* owner, float xd, float yd, float zd, float mass, bool is_kinematic );
		~BoxCollider();

		/// ---GETTERS/SETTERS--- ///
		virtual ssuge::ComponentType get_type() { return ssuge::ComponentType::BOX_PHYS; }	// must get overrode from the children component

		///---OTHER METHODS---///
		void update(float dt) override;
	};
}