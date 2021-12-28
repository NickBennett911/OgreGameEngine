#pragma once
#include <stdafx.h>

namespace ssuge {
	class GameObject;		//forward declartion of GameObject
	enum ComponentType {
		CAMERA, LIGHT, MESH, BOX_PHYS, SPHERE_PHYS
	};
	class Component {

		///---ATTRIBUTES---///
	

	protected:
		GameObject* mOwningGameObject;
		friend class Application;

		/// ---CONSTRUCTOR/DESTRUCTOR---///
	public:
		Component(ssuge::GameObject* owner);
		~Component();

		/// ---GETTERS/SETTERS--- ///
		virtual ComponentType get_type() = 0;	// must get overrode from the children component
		virtual void set_visible(bool isVisible);	// also must get overrode from the children component

		GameObject* get_owner() { return mOwningGameObject; }

		///---OTHER METHODS---///
		virtual void update(float dt);


	};
}