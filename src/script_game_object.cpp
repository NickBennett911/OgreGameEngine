#include <stdafx.h>
#include <script_game_object.h>
#include <game_object_manager.h>
#include <structmember.h>
#include <application.h>
#include <physics_manager.h>

namespace ssuge {
	namespace script {

		int GameObject_init(PyObject* self, PyObject* args, PyObject* kwds)
		{
			int size = PyTuple_GET_SIZE(args);
			if (size != 3 && size != 4) {
				PyErr_SetString(PyExc_ValueError, "only takes 3 or 4 args");
				return -1;
			}
			const char* name;
			const char* group;
			int tag;
			if (size == 3) {
				if (!PyArg_ParseTuple(args, "ssi", &name, &group, &tag))
					return -1;
				ssuge::script::GameObject* myObj = (ssuge::script::GameObject*)(self);
				ssuge::GameObject* ssugeObj = GAME_OBJECT_MANAGER->create_game_object(group, name, NULL, tag);
				myObj->mTwin = ssugeObj;
				ssugeObj->set_twin(self);
			}
			else if (size == 4) {
				PyObject* parent;
				if (!PyArg_ParseTuple(args, "ssiO", &name, &group, &tag, &parent))
					return -1;
				ssuge::script::GameObject* myObj = (ssuge::script::GameObject*)(self);
				ssuge::GameObject* ssugeObj = GAME_OBJECT_MANAGER->create_game_object(group, name, ((ssuge::script::GameObject*)parent)->mTwin, tag);
				myObj->mTwin = ssugeObj;
				ssugeObj->set_twin(self);
			}
			Py_INCREF(self);
			return 0;
		}

		PyObject* ssuge::script::name(PyObject* self, PyObject* args) {
			if (PyTuple_GET_SIZE(args) != 0) {
				PyErr_SetString(PyExc_ValueError, "this method does not take any arguemnts");
				return NULL;
			}
			ssuge::script::GameObject* myObj = (ssuge::script::GameObject*)(self);
			std::string name = myObj->mTwin->get_name();
			PyObject* py_name = PyUnicode_FromString(name.c_str());

			return py_name;
		}

		PyObject* ssuge::script::create_mesh_component(PyObject* self, PyObject* args) {
			if (PyTuple_GET_SIZE(args) != 1) {
				PyErr_SetString(PyExc_ValueError, "this method only takes 1 argument");
				return NULL;
			}
			const char* fname;
			if (PyArg_ParseTuple(args, "s", &fname)) {
				ssuge::script::GameObject* myObj = (ssuge::script::GameObject*)(self);
				ssuge::MeshComponent* comp = myObj->mTwin->create_mesh_component(fname);
				Py_INCREF(Py_None);
				return Py_None;
			}
			PyErr_SetString(PyExc_ValueError, "PyArg_ParseTuple could not get string value");
			return NULL;
		}

		PyObject* ssuge::script::parent(PyObject* self, PyObject* args) {
			if (PyTuple_GET_SIZE(args) != 0) {
				PyErr_SetString(PyExc_ValueError, "this function does not take any arguments");
				return NULL;
			}
			
			ssuge::script::GameObject* myObj = (ssuge::script::GameObject*)(self);
			ssuge::GameObject* parent = myObj->mTwin->get_parent();
			if (parent == NULL) {
				Py_INCREF(Py_None);
				return Py_None;
			}
			Py_INCREF(parent);
			return parent->get_twin();
		}

		PyObject* ssuge::script::play_animation(PyObject* self, PyObject* args) {
			if (PyTuple_GET_SIZE(args) != 2) {
				PyErr_SetString(PyExc_ValueError, "this function takes only two arguments");
				return NULL;
			}
			const char* anim_name;
			int looping;
			if (PyArg_ParseTuple(args, "sp", &anim_name, &looping)) {
				ssuge::script::GameObject* myObj = (ssuge::script::GameObject*)(self);
				if (myObj->mTwin->has_component_type(ssuge::ComponentType::MESH)) {
					ssuge::MeshComponent* mesh = myObj->mTwin->get_component<ssuge::MeshComponent>(ssuge::ComponentType::MESH);
					mesh->play_animation(anim_name, looping);
					Py_INCREF(Py_None);
					return Py_None;
				}
				PyErr_SetString(PyExc_ValueError, "Game object does not have a mesh component");
				return NULL;
			}
			PyErr_SetString(PyExc_ValueError, "could not parse args tuple");
			return NULL;
			
		}

		PyObject* ssuge::script::rotate_world(PyObject* self, PyObject* args) {
			if (PyTuple_GET_SIZE(args) != 4) {
				PyErr_SetString(PyExc_ValueError, "this function only takes 4 arguments");
				return NULL;
			}
			float dr, vx, vy, vz;
			if (PyArg_ParseTuple(args, "ffff", &dr, &vx, &vy, &vz)) {
				ssuge::script::GameObject* myObj = (ssuge::script::GameObject*)(self);
				myObj->mTwin->rotate_world(dr, vx, vy, vz);
				Py_INCREF(Py_None);
				return Py_None;
			}
			PyErr_SetString(PyExc_ValueError, "could not parse args tuple");
			return NULL;

		}

		PyObject* ssuge::script::scale(PyObject* self, PyObject* args) {
			if (PyTuple_GET_SIZE(args) != 3) {
				PyErr_SetString(PyExc_ValueError, "this function only takes 3 arguments");
				return NULL;
			}
			float dx, dy, dz;
			if (PyArg_ParseTuple(args, "fff", &dx, &dy, &dz)) {
				ssuge::script::GameObject* myObj = (ssuge::script::GameObject*)(self);
				myObj->mTwin->set_scale(dx, dy, dz);
				Py_INCREF(Py_None);
				return Py_None;
			}
			PyErr_SetString(PyExc_ValueError, "could not parse args tuple");
			return NULL;

		}

		PyObject* ssuge::script::set_position(PyObject* self, PyObject* args) {
			if (PyTuple_GET_SIZE(args) != 3) {
				PyErr_SetString(PyExc_ValueError, "this function only takes 3 arguments");
				return NULL;
			}
			float x, y, z;
			if (PyArg_ParseTuple(args, "fff", &x, &y, &z)) {
				ssuge::script::GameObject* myObj = (ssuge::script::GameObject*)(self);
				myObj->mTwin->set_position(x, y, z);
				Py_INCREF(Py_None);
				return Py_None;
			}
			PyErr_SetString(PyExc_ValueError, "could not parse args tuple");
			return NULL;

		}

		PyObject* ssuge::script::set_world_position(PyObject* self, PyObject* args) {
			if (PyTuple_GET_SIZE(args) != 3) {
				PyErr_SetString(PyExc_ValueError, "this function only takes 3 arguments");
				return NULL;
			}
			float x, y, z;
			if (PyArg_ParseTuple(args, "fff", &x, &y, &z)) {
				ssuge::script::GameObject* myObj = (ssuge::script::GameObject*)(self);
				myObj->mTwin->set_position_world(x, y, z);
				Py_INCREF(Py_None);
				return Py_None;
			}
			PyErr_SetString(PyExc_ValueError, "could not parse args tuple");
			return NULL;

		}

		PyObject* ssuge::script::set_world_orientation(PyObject* self, PyObject* args) {
			if (PyTuple_GET_SIZE(args) != 4) {
				PyErr_SetString(PyExc_ValueError, "this function only takes 4 arguments");
				return NULL;
			}

			float w, x, y, z;
			if (PyArg_ParseTuple(args, "ffff", &w, &x, &y, &z)) {
				ssuge::script::GameObject* myObj = (ssuge::script::GameObject*)(self);
				myObj->mTwin->set_orientation(w, x, y, z);
				Py_INCREF(Py_None);
				return Py_None;
			}
			PyErr_SetString(PyExc_ValueError, "could not parse args tuple");
			return NULL;
		}

		PyObject* ssuge::script::tag(PyObject* self, PyObject* args) {
			if (PyTuple_GET_SIZE(args) != 0) {
				PyErr_SetString(PyExc_ValueError, "this function takes no arguments");
				return NULL;
			}

			ssuge::script::GameObject* myObj = (ssuge::script::GameObject*)(self);
			int tag = myObj->mTwin->get_tag();
			PyObject* tag2 = PyLong_FromLong((long)tag);
			return tag2;

		}

		PyObject* ssuge::script::translate_local(PyObject* self, PyObject* args) {
			if (PyTuple_GET_SIZE(args) != 3) {
				PyErr_SetString(PyExc_ValueError, "this function only takes 3 arguments");
				return NULL;
			}
			float dx, dy, dz;
			if (PyArg_ParseTuple(args, "fff", &dx, &dy, &dz)) {
				ssuge::script::GameObject* myObj = (ssuge::script::GameObject*)(self);
				ssuge::GameObject* curobj = myObj->mTwin;
				curobj->translate_local(dx, dy, dz);
				Py_INCREF(Py_None);
				return Py_None;
			}
			PyErr_SetString(PyExc_ValueError, "could not parse args tuple");
			return NULL;
		}

		PyObject* ssuge::script::get_world_orientation(PyObject* self, PyObject* args) {
			if (PyTuple_GET_SIZE(args) != 0) {
				PyErr_SetString(PyExc_ValueError, "this function takes no arguments");
				return NULL;
			}

			ssuge::script::GameObject* myObj = (ssuge::script::GameObject*)(self);
			ssuge::GameObject* curobj = myObj->mTwin;
			Ogre::Quaternion orien = curobj->get_world_orientation();
			Ogre::Degree deg;
			Ogre::Vector3 vec;
			orien.ToAngleAxis(deg, vec);

			PyObject* myargs = PyTuple_New(4);
			PyTuple_SetItem(myargs, 0, PyFloat_FromDouble(deg.valueDegrees()));
			PyTuple_SetItem(myargs, 1, PyFloat_FromDouble(vec.x));
			PyTuple_SetItem(myargs, 2, PyFloat_FromDouble(vec.y));
			PyTuple_SetItem(myargs, 3, PyFloat_FromDouble(vec.z));
			//calls some run_method funciton
			return myargs;

			Py_DecRef(myargs);
		}

		PyObject* ssuge::script::get_world_position(PyObject* self, PyObject* args) {
			if (PyTuple_GET_SIZE(args) != 0) {
				PyErr_SetString(PyExc_ValueError, "this function takes no arguments");
				return NULL;
			}
			ssuge::script::GameObject* myObj = (ssuge::script::GameObject*)(self);
			ssuge::GameObject* curobj = myObj->mTwin;
			Ogre::Vector3 pos = curobj->get_world_position();

			PyObject* myargs = PyTuple_New(3);
			PyTuple_SetItem(myargs, 0, PyFloat_FromDouble(pos.x));
			PyTuple_SetItem(myargs, 1, PyFloat_FromDouble(pos.y));
			PyTuple_SetItem(myargs, 2, PyFloat_FromDouble(pos.z));
			//calls some run_method funciton
			return myargs;

			Py_DecRef(myargs);

		}

		PyObject* ssuge::script::create_box_collider(PyObject* self, PyObject* args) {
			if (PyTuple_GET_SIZE(args) != 5) {
				PyErr_SetString(PyExc_ValueError, "this function only takes 5 arguments");
				return NULL;
			}

			float xdim, ydim, zdim, mass;
			int is_kinematic;
			if (PyArg_ParseTuple(args, "ffffp", &xdim, &ydim, &zdim, &mass, &is_kinematic)) {
				ssuge::script::GameObject* myObj = (ssuge::script::GameObject*)(self);
				ssuge::GameObject* curobj = myObj->mTwin;
				curobj->create_box_collider(xdim, ydim, zdim, mass, is_kinematic);
				Py_INCREF(Py_None);
				return Py_None;
			}
			PyErr_SetString(PyExc_ValueError, "could not parse args tuple");
			return NULL;
		}

		PyObject* ssuge::script::create_sphere_collider(PyObject* self, PyObject* args) {
			if (PyTuple_GET_SIZE(args) != 3) {
				PyErr_SetString(PyExc_ValueError, "this function only takes 5 arguments");
				return NULL;
			}

			float radius, mass;
			int is_kinematic;
			if (PyArg_ParseTuple(args, "ffp", &radius, &mass, &is_kinematic)) {
				ssuge::script::GameObject* myObj = (ssuge::script::GameObject*)(self);
				ssuge::GameObject* curobj = myObj->mTwin;
				curobj->create_sphere_collider(radius, mass, is_kinematic);
				Py_INCREF(Py_None);
				return Py_None;
			}
			PyErr_SetString(PyExc_ValueError, "could not parse args tuple");
			return NULL;
		}

		PyObject* ssuge::script::destroy(PyObject* self, PyObject* args) {
			if (PyTuple_GET_SIZE(args) != 0) {
				PyErr_SetString(PyExc_ValueError, "this function only takes no arguments");
				return NULL;
			}
			ssuge::script::GameObject* myObj = (ssuge::script::GameObject*)(self);
			ssuge::GameObject* curobj = myObj->mTwin;

			PHYSICS_MANAGER->queue_listener_for_removal(curobj);
			GAME_OBJECT_MANAGER->queue_game_object_destruction(curobj);
			Py_INCREF(Py_None);
			return Py_None;

		}

		void GameObject_dealloc(PyObject* self)
		{
			// No need to decref / free mAccessCount -- it'll be freed below
			Py_TYPE(self)->tp_free((PyObject*)self);
		}

		// Called once when a new instance is *allocated* (we're not usually aware of this step in python,
		// but there is a __new__ method that can be overridden in python).
		// This function should go in the tp_new "slot"
		PyObject* GameObject_new(PyTypeObject* type, PyObject* args, PyObject* kwds)
		{
			GameObject* self = (GameObject*)type->tp_alloc(type, 0);
			if (self != NULL)
			{
				self->mTwin = nullptr;
			}
			else {
				PyErr_SetString(PyExc_MemoryError, "Couldnt allocatte a foo object");
			}
			return (PyObject*)self;
		}

		PyMemberDef ssuge_members[] =
		{
			//{"name", T_OBJECT_EX, offsetof(MyFooObjectData, mName), 0, "the name of this thing"},
			//{"MyOgre", T_INT, offsetof(GameObject, mAccessCount), 0, "number of times we've access this thing"},
			{NULL} // Sentinel
		};


		PyMethodDef ssuge_methods[] =
		{
			{"create_mesh_component", (PyCFunction)ssuge::script::create_mesh_component, METH_VARARGS, "Return the name repeated access_count times"},
			{"name", (PyCFunction)ssuge::script::name, METH_VARARGS, "Return the name repeated access_count times"},
			{"parent", (PyCFunction)ssuge::script::parent, METH_VARARGS, "Return the name repeated access_count times"},
			{"play_animation", (PyCFunction)ssuge::script::play_animation, METH_VARARGS, "Return the name repeated access_count times"},
			{"rotate_world", (PyCFunction)ssuge::script::rotate_world, METH_VARARGS, "Return the name repeated access_count times"},
			{"scale", (PyCFunction)ssuge::script::scale, METH_VARARGS, "Return the name repeated access_count times"},
			{"set_position", (PyCFunction)ssuge::script::set_position, METH_VARARGS, "Return the name repeated access_count times"},
			{"set_world_position", (PyCFunction)ssuge::script::set_world_position, METH_VARARGS, "Return the name repeated access_count times"},
			{"tag", (PyCFunction)ssuge::script::tag, METH_VARARGS, "Return the name repeated access_count times"},
			{"translate_local", (PyCFunction)ssuge::script::translate_local, METH_VARARGS, "args {three number for its change in position} returns nothing"},
			{"get_world_orientation", (PyCFunction)ssuge::script::get_world_orientation, METH_VARARGS, "args {none} returns {pithc, roll, yaw}"},
			{"get_world_position", (PyCFunction)ssuge::script::get_world_position, METH_VARARGS, "args {none} returns {x, y, z}"},
			{"set_world_orientation", (PyCFunction)ssuge::script::set_world_orientation, METH_VARARGS, "args {w, x, y, z} returns None"},
			{"create_box_collider", (PyCFunction)ssuge::script::create_box_collider, METH_VARARGS, "args {xdim, ydim, zdim, mass, is_kinematic} returns None"},
			{"create_sphere_collider", (PyCFunction)ssuge::script::create_sphere_collider, METH_VARARGS, "args {radius, mass, is_kinematic} returns None"},
			{"destroy", (PyCFunction)ssuge::script::destroy, METH_VARARGS, "args {none} returns None, deletes game object"},
			{NULL} /* Sentinel */
		};

		
	}
}
PyTypeObject GameObjectClass =
{
	// This initializes the first part of our struct (which is the PyObject part) to
	// default values. The address of MyFooClass can be safely cast to a PyObject*
	PyVarObject_HEAD_INIT(NULL, 0)
	"ssuge_module.GameObject", // const char* tp_name
	sizeof(ssuge::script::GameObject), // Py_ssize_t tp_basicsize (size of the "attributes" portion)
	0, // Py_ssize_t tp_itemsize
	(destructor)ssuge::script::GameObject_dealloc, // destructor tp_dealloc
	0, // Py_ssize_t tp_vectorcall_offset
	NULL, // getattrfunc tp_getattr
	NULL, // setattrfunc tp_setattr
	NULL, // PyAsyncMethods* tp_as_async // formerly known as tp_compare (Python 2)
										// or tp_reserved (Python 3)
	NULL, // reprfunc tp_repr

	// Method suites for standard classes
	NULL, // PyNumberMethods* tp_as_number
	NULL, // PySequenceMethods* tp_as_sequence
	NULL, // PyMappingMethods* tp_as_mapping

	// More standard operations (here for binary compatibility)
	NULL, // hashfunc tp_hash;
	NULL, // ternaryfunc tp_call;
	NULL,			// reprfunc tp_str;
	NULL, // getattrofunc tp_getattro;
	NULL, // setattrofunc tp_setattro;
	// Functions to access object as input/output buffer
	NULL, // PyBufferProcs* tp_as_buffer;
	// Flags to define presence of optional/expanded features
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, // unsigned long tp_flags;
	"doc string for ScriptGameObject", // const char* tp_doc; // Documentation string

	// Assigned meaning in release 2.0
	// call function for all accessible objects
	NULL, // traverseproc tp_traverse;

	// delete references to contained objects
	NULL, // inquiry tp_clear;

	// Assigned meaning in release 2.1
	// rich comparisons
	NULL, // richcmpfunc tp_richcompare;

	// weak reference enabler
	NULL, // Py_ssize_t tp_weaklistoffset;

	// Iterators
	NULL, // getiterfunc tp_iter;
	NULL, // iternextfunc tp_iternext;

	// Attribute descriptor and subclassing stuff
	ssuge::script::ssuge_methods, // struct PyMethodDef* tp_methods;
	ssuge::script::ssuge_members, // struct PyMemberDef* tp_members;
	NULL, // struct PyGetSetDef* tp_getset;
	NULL, // struct _typeobject* tp_base;
	NULL, // PyObject* tp_dict;
	NULL, // descrgetfunc tp_descr_get;
	NULL, // descrsetfunc tp_descr_set;
	0, // Py_ssize_t tp_dictoffset;
	(initproc)ssuge::script::GameObject_init, // initproc tp_init;
	NULL, // allocfunc tp_alloc;
	ssuge::script::GameObject_new, // newfunc tp_new;
	NULL, // freefunc tp_free; /* Low-level free-memory routine */

	// There are more fields, but we don't need to modify anything after tp_new. Since we assigned
	// NULL last, that value will be assigned to any other fields later in the struct (that we're not
	// manually assigning a value to)
};