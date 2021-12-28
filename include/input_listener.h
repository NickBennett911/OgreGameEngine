#pragma once
#include <stdafx.h>

namespace ssuge {
	class InputListener {
	public:
		virtual void action_pressed(std::string action) {  }
		virtual void action_released(std::string action) {  }
		virtual void axis_updated(std::string axis, float value) {  }
	};
}