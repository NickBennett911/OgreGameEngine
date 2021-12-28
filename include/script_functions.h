#pragma once
#include <stdafx.h>

namespace ssuge {
	namespace script {
		// return Py_None when not returning something. NULL means an error:
		// Py_INCREF(Py_None);
		// return Py_None;
		PyObject* load_scene(PyObject* module, PyObject* args);

		PyObject* log(PyObject* module, PyObject* args);

		PyObject* load_script(PyObject* module, PyObject* args);

		PyObject* find_game_object(PyObject* module, PyObject* args);

		PyObject* set_skybox(PyObject* module, PyObject* args);

		PyObject* register_input_listener(PyObject* module, PyObject* args);

		PyObject* get_axis(PyObject* module, PyObject* args);

		PyObject* quit(PyObject* module, PyObject* args);
	
		PyObject* set_physics_gravity(PyObject* self, PyObject* args);

		PyObject* register_collision_listener(PyObject* self, PyObject* args);

		PyObject* get_objects_in_group(PyObject* self, PyObject* args);

	}
}