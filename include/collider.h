#pragma once
#include <stdafx.h>
#include <component.h>

namespace ssuge {

	class Collider : public Component {
		///---ATTRIBUTES---///


	protected:

		/// ---CONSTRUCTOR/DESTRUCTOR---///
	public:
		Collider(ssuge::GameObject* owner);
		~Collider();

		/// ---GETTERS/SETTERS--- ///

		///---OTHER METHODS---///
		virtual void update(float dt) {};
	};

}
