#pragma once
#include <stdafx.h>
#include <game_object.h>

namespace ssuge {
	namespace script {
		typedef struct
		{
			// Very important! This makes our struct into a "dervied class" of PyObject
			// (using C "technology", not inheritance in the C++ sense)
			PyObject_HEAD
				// Then we add our specific attributes. Note: we'll indicate the names for these
				// in python elsewhere
				ssuge::GameObject* mTwin;
		}GameObject;

		PyObject* create_mesh_component(PyObject* self, PyObject* args);

		PyObject* name(PyObject* self, PyObject* args);

		PyObject* parent(PyObject* self, PyObject* args);

		PyObject* play_animation(PyObject* self, PyObject* args);

		PyObject* rotate_world(PyObject* self, PyObject* args);

		PyObject* scale(PyObject* self, PyObject* args);

		PyObject* set_position(PyObject* self, PyObject* args);

		PyObject* set_world_position(PyObject* self, PyObject* args);	

		PyObject* set_world_orientation(PyObject* self, PyObject* args);

		PyObject* tag(PyObject* self, PyObject* args);

		PyObject* translate_local(PyObject* self, PyObject* args);

		PyObject* get_world_orientation(PyObject* self, PyObject* args);

		PyObject* get_world_position(PyObject* self, PyObject* args);

		PyObject* quit(PyObject* self, PyObject* args);

		PyObject* create_box_collider(PyObject* self, PyObject* args);

		PyObject* create_sphere_collider(PyObject* self, PyObject* args);

		PyObject* destroy(PyObject* self, PyObject* args);

	}
}
