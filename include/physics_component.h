#pragma once
#include <stdafx.h>
#include <component.h>

namespace ssuge {

	class GameObject;

	class PhysicsComponent : public Component{
		// ***** ATTRIBUTES *****
	protected:

		Ogre::AnimationState* mAnimation = nullptr;

		// ***** CONSTRUCTORS / DESTRUCTORS *****
	public:
		/// The constructor
		PhysicsComponent(std::string fname, GameObject* owner);

		/// Destructor
		~PhysicsComponent();

		// ***** METHODS *****
	public:
		/// Sets the material for a sub-entity
		void set_material(int sub_entity, std::string mat_name);

		// ***** OVERRIDES from COMPONENT *****
	public:
		/// Returns the type of this component.
		ssuge::ComponentType get_type() override { return ssuge::ComponentType::PHYSICS; }

		// ****** OTHER ******
	public:
		void play_animation(std::string anim, bool looping);

		void update(float dt) override;
	};
}