#include <stdafx.h>
#include <script_functions.h>
#include <log_manager.h>
#include <application.h>
#include <game_object_manager.h>
#include <script_game_object.h>
#include <physics_manager.h>

extern PyTypeObject GameObjectClass;			// Defined in script_game_object.cpp


PyObject* ssuge::script::log(PyObject* module, PyObject* args) {
	int size = PyTuple_GET_SIZE(args);
	const char* temps;
	float r, g, b, time;
	if (size == 1) {
		if (PyArg_ParseTuple(args, "s", &temps))	// does the args contain just one string
			LOG_MANAGER->log(temps);
		else {
			PyErr_SetString(PyExc_ValueError, "PyArg_ParseTuple faild to get the tuple");
			return NULL;
		}
	}
	else {
		if (PyArg_ParseTuple(args, "s(fff)f", &temps, &r, &g, &b, &time))	// does args contain a string, tuple with three floats, and a float (in that order)
			LOG_MANAGER->log(temps, (r, g, b), time);
		else {
			PyErr_SetString(PyExc_ValueError, "PyArg_ParseTuple failed to get the tuple");
			return NULL;
		}

	}
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject* ssuge::script::set_skybox(PyObject* module, PyObject* args) {
	int size = PyTuple_GET_SIZE(args);
	if (size != 3) {
		PyErr_SetString(PyExc_ValueError, "this function only takes 3 args");
		return NULL;	// this function takes only three arguments
	}
	const char* path;
	int dist, active;
	int result = PyArg_ParseTuple(args, "sip", &path, &dist, &active);
	if (!result) {
		PyErr_SetString(PyExc_ValueError, "failed to parse the tuple");
		return NULL;
	}
	APPLICATION->setSkyBox(active, path, dist);
	LOG_MANAGER->log("skybox has been set");
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject* ssuge::script::load_scene(PyObject* module, PyObject* args) {
	int size = PyTuple_GET_SIZE(args);
	if (size != 2) {
		PyErr_SetString(PyExc_ValueError, "function only takes 2 args");
		return NULL;	// this function takes only 2 arguments
	}
	const char* name;
	const char* path;
	int result = PyArg_ParseTuple(args, "ss", &name, &path);
	if (!result) {
		PyErr_SetString(PyExc_ValueError, "failed to parse tuple");
		return NULL;
	}
	GAME_OBJECT_MANAGER->load_scene_file(name, path);
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject* ssuge::script::find_game_object(PyObject* module, PyObject* args) {
	int size = PyTuple_GET_SIZE(args);
	if (size != 2) {
		PyErr_SetString(PyExc_ValueError, "function only takes 2 args");
		return NULL;	// this function takes only 2 arguments
	}

	const char* objName;
	const char* groupName;
	if (PyArg_ParseTuple(args, "ss", &objName, &groupName)) {
		ssuge::GameObject* ssugeObj = GAME_OBJECT_MANAGER->get_game_object(objName, groupName);
		PyObject* scriptObj = ssugeObj->get_twin();
		return scriptObj;
	}
	PyErr_SetString(PyExc_ValueError, "failed to parse tuple");
	return NULL;

}

PyObject* ssuge::script::load_script(PyObject* module, PyObject* args) {
	int size = PyTuple_GET_SIZE(args);
	if (size != 1) {
		PyErr_SetString(PyExc_ValueError, "this function only takes one argument");
		return NULL;	// this function takes only 1 argument
	}
	const char* fname;
	if (PyArg_ParseTuple(args, "s", &fname)) {
		SCRIPTMANAGER->run_script(fname);
		Py_INCREF(Py_None);
		return Py_None;
	}
	PyErr_SetString(PyExc_ValueError, "failed to parse tuple");
	return NULL;
}

PyObject* ssuge::script::register_input_listener(PyObject* module, PyObject* args) {
	int size = PyTuple_GET_SIZE(args);
	if (size != 1) {
		PyErr_SetString(PyExc_ValueError, "this function only takes one argument");
		return NULL;	// this function takes only 1 argument
	}
	PyObject* obj;
	if (PyArg_ParseTuple(args, "O", &obj)) {
		if (PyObject_IsInstance(obj, (PyObject*)&GameObjectClass)) {
			ssuge::script::GameObject* myObj = (ssuge::script::GameObject*)(obj);
			INPUT_MANAGER->register_listener(myObj->mTwin);
			Py_INCREF(Py_None);
			return Py_None;
		}
		PyErr_SetString(PyExc_ValueError, "object is not of correct type: GameObjectClass");
		return NULL;
	}
	PyErr_SetString(PyExc_ValueError, "failed to parse tuple");
	return NULL;
	
}

PyObject* ssuge::script::get_axis(PyObject* module, PyObject* args) {
	int size = PyTuple_GET_SIZE(args);
	if (size != 1) {
		PyErr_SetString(PyExc_ValueError, "this function only takes one argument");
		return NULL;	// this function takes only 1 argument
	}

	const char* axis;
	if (PyArg_ParseTuple(args, "s", &axis)) {
		float val = INPUT_MANAGER->get_axis(axis);
		return PyFloat_FromDouble(val);
	}
	PyErr_SetString(PyExc_ValueError, "failed to parse tuple");
	return NULL;
}

PyObject* ssuge::script::quit(PyObject* module, PyObject* args) {
	APPLICATION->quit_app();
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject* ssuge::script::set_physics_gravity(PyObject* self, PyObject* args) {
	if (PyTuple_GET_SIZE(args) != 3) {
		PyErr_SetString(PyExc_ValueError, "this function only takes 3 arguments");
		return NULL;
	}

	float gx, gy, gz;
	if (PyArg_ParseTuple(args, "fff", &gx, &gy, &gz)) {
		PHYSICS_MANAGER->set_physics_gravity(gx, gy, gz);
		Py_INCREF(Py_None);
		return Py_None;
	}
	PyErr_SetString(PyExc_ValueError, "could not parse args tuple");
	return NULL;
}

PyObject* ssuge::script::register_collision_listener(PyObject* self, PyObject* args) {
	if (PyTuple_GET_SIZE(args) != 1) {
		PyErr_SetString(PyExc_ValueError, "this function only takes 1 arguments");
		return NULL;
	}
	PyObject* obj;
	if (PyArg_ParseTuple(args, "O", &obj)) {
		if (PyObject_IsInstance(obj, (PyObject*)&GameObjectClass)) {
			ssuge::script::GameObject* myObj = (ssuge::script::GameObject*)(obj);
			PHYSICS_MANAGER->register_listener(myObj->mTwin);
			Py_INCREF(Py_None);
			return Py_None;
		}
		PyErr_SetString(PyExc_ValueError, "object is not of correct type: GameObjectClass");
		return NULL;
	}
	PyErr_SetString(PyExc_ValueError, "failed to parse tuple");
	return NULL;
}

PyObject* ssuge::script::get_objects_in_group(PyObject* self, PyObject* args) {
	if (PyTuple_GET_SIZE(args) != 1) {
		PyErr_SetString(PyExc_ValueError, "this function only takes 1 arguments");
		return NULL;
	}
	const char* group_name;
	if (PyArg_ParseTuple(args, "s", &group_name)) {
		std::vector<ssuge::GameObject*> gobjects = GAME_OBJECT_MANAGER->get_game_objects(group_name);
		PyObject* myargs = PyTuple_New(gobjects.size());
		for (int i = 0; i < gobjects.size(); i++) {
			PyTuple_SetItem(myargs, i, gobjects[i]->get_twin());
		}
		return myargs;
	}
	PyErr_SetString(PyExc_ValueError, "could not parse args tuple");
	return NULL;
}

PyMethodDef ssuge_script_functions[] =
{
	{"log", ssuge::script::log, METH_VARARGS, "this is the log function"},
	{"set_skybox", ssuge::script::set_skybox, METH_VARARGS, "args {'filename', int distance, bool active}"},
	{"load_scene", ssuge::script::load_scene, METH_VARARGS, "args {'name of group for scene', 'path for the scene file'}"},
	{"load_script", ssuge::script::load_script, METH_VARARGS, "args {'fname of script'}"},
	{"find_game_object", ssuge::script::find_game_object, METH_VARARGS, "args {'object name', 'group name'}"},
	{"register_input_listener", ssuge::script::register_input_listener, METH_VARARGS, "args {object (that inherits from ssuge.GameObject) that wants input updates}"},\
	{"register_collision_listener", ssuge::script::register_collision_listener, METH_VARARGS, "args {object (that inherits from ssuge.GameObject) that wants collision updates}"},
	{"get_axis", ssuge::script::get_axis, METH_VARARGS, "args {none}, returns the value of that axis"},
	{"quit", ssuge::script::quit, METH_VARARGS, "args {none}, returns none, quits the application"},
	{"set_physics_gravity", ssuge::script::set_physics_gravity, METH_VARARGS, "args {gx, gy, gz} -> sets what direction the force of gravity will be, returns None"},
	{"get_objects_in_group", ssuge::script::get_objects_in_group, METH_VARARGS, "args {string group_name} -> returns all game objects in specified group" },
	{NULL, NULL, 0, NULL} // Sentinel
};