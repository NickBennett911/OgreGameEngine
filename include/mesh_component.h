#pragma once
#include <stdafx.h>
#include <component.h>

namespace ssuge
{
	class GameObject;

	/// The MeshComponent component-type is used to load and display a .mesh file instance
	class MeshComponent : public Component
	{
	// ***** ATTRIBUTES *****
	protected:
		/// The ogre entity.
		Ogre::Entity * mEntity;

		Ogre::AnimationState* mAnimation = nullptr;

	// ***** CONSTRUCTORS / DESTRUCTORS *****
	public:
		/// The constructor
		MeshComponent(std::string fname, GameObject * owner);

		/// Destructor
		~MeshComponent();

	// ***** METHODS *****
	public:
		/// Sets the material for a sub-entity
		void set_material(int sub_entity, std::string mat_name);

	// ***** OVERRIDES from COMPONENT *****
	public:
		/// Returns the type of this component.
		ssuge::ComponentType get_type() override { return ssuge::ComponentType::MESH; }
		
		/// Makes this mesh not render (if is_visible is false)
		void set_visible(bool is_visible) override { mEntity->setVisible(is_visible); }

	// ****** OTHER ******
	public:
		void play_animation(std::string anim, bool looping);

		void update(float dt) override;
	};
}
