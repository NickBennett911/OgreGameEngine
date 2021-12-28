#pragma once
#include <stdafx.h>
#include <component.h>

namespace ssuge
{
	// Forward reference to game object class (used to prevent a circular dependency loop)
	class GameObject;

	/// The types of lights we support.
	enum class LightType { POINT, DIRECTIONAL, SPOT };

	/// The LightComponent component-specilization is used to create a standard light (spot, point, or directional) on the 
	/// attached game object.
	class LightComponent : public Component
	{
	// ***** ATTRIBUTES *****
	protected:
		/// The ogre light we're based upon.
		Ogre::Light * mLight;

	// ***** CONSTRUCTORS / DESTRUCTORS *****
	public:
		/// Constructor
		LightComponent(LightType type, GameObject * owner);

		/// Destructor
		~LightComponent();

	// ***** COMPONENT OVERRIDES *****
	public:
		/// Return the type of this component
		ComponentType get_type() override { return ComponentType::LIGHT; }

		/// Deactivates this component if is_visible is false.
		void set_visible(bool is_visible) override { mLight->setVisible(is_visible); }
		
	// ***** SETTERS *****
	public:
		/// Sets the diffuse light color of this light
		void set_diffuse_color(float r, float g, float b) { mLight->setDiffuseColour(Ogre::ColourValue(r, g, b)); }

		/// Sets the specuilar light color of this light.
		void set_specular_color(float r, float g, float b) { mLight->setSpecularColour(Ogre::ColourValue(r, g, b)); }

		/// Only affects spotlights.
		void set_spotlight_params(float inner_angle, float outer_angle) { mLight->setSpotlightRange(Ogre::Degree(inner_angle), Ogre::Degree(outer_angle)); }
	};
}
