#pragma once
#include <stdafx.h>


namespace ssuge {
	class CollisionListener {
	public:
		virtual void collision_happened(ssuge::Collider* other) { }
		virtual ssuge::GameObject* get_gobj() = 0;
	};
}
 