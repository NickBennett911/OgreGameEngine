#include <stdafx.h>
#include <iostream>
#include <fstream> 
#include <script_manager.h>
#include <log_manager.h>
#include <script_functions.h>
#include <structmember.h>

ssuge::ScriptManager* ssuge::Singleton<ssuge::ScriptManager>::msSingleton = NULL;		/// this allows to set the msSingleton in singleton class 

extern PyMethodDef ssuge_script_functions[];	// Defined in script_functions.cpp
extern PyTypeObject GameObjectClass;			// Defined in script_game_object.cpp

static PyModuleDef ssuge_module =
{
	PyModuleDef_HEAD_INIT,
	"ssuge_module", // name of module
	"the python module for my first game engine ssuge", // module documentation, may be NULL
	-1, // size of per-interpreter state of the module,
		// or -1 if the module keeps state in global variables.
	ssuge_script_functions
};

PyMODINIT_FUNC PyInit_ssuge_module(void)
{
	if (PyType_Ready(&GameObjectClass) < 0) {
		return NULL;
	}
		
	SCRIPTMANAGER->mModule = PyModule_Create(&ssuge_module);

	Py_INCREF(&GameObjectClass);
	if (!PyModule_AddObject(SCRIPTMANAGER->mModule, "GameObject", (PyObject*)&GameObjectClass) < 0) {
		Py_DECREF(&GameObjectClass);
		Py_DECREF(SCRIPTMANAGER->mModule);
		//PyErr_SetString(PyExec_ImportError, "couldn't add foo class to the module");
		return NULL;
	}

	return SCRIPTMANAGER->mModule;
}

ssuge::ScriptManager::ScriptManager() {
	Py_SetPath(L"python39.zip");
	int result = PyImport_AppendInittab("ssuge", PyInit_ssuge_module);
	if (result == -1) {
		ssuge::ScriptManager::handle_error();
	}
	Py_Initialize(); 
}

ssuge::ScriptManager::~ScriptManager() {
	Py_Finalize();
}

void ssuge::ScriptManager::run_method(PyObject* obj, std::string meth_name, PyObject* tuple) {
 	int result = PyObject_HasAttrString(obj, meth_name.c_str());
	if (!result) {
		return;
	}
	PyObject* attrib = PyObject_GetAttrString(obj, meth_name.c_str());
	if (!attrib) {
		ssuge::ScriptManager::handle_error();
	}

	if (PyCallable_Check(attrib)) {
		PyObject* r = PyObject_Call(attrib, tuple, NULL);
		if (!r) {
			ssuge::ScriptManager::handle_error();
		}
	}
	Py_XDECREF(attrib);
}

void ssuge::ScriptManager::run_script(const char * fname) {
	std::ifstream file(fname);
	file.seekg(0, std::ios::end);
	size_t size = file.tellg();
	std::string buffer(size, ' ');
	file.seekg(0);
	file.read(&buffer[0], size);

	PyObject* CodeObj = Py_CompileString(buffer.c_str(), fname, Py_file_input);
	if (CodeObj == NULL) {
		ssuge::ScriptManager::handle_error();
	}

	PyObject* result = PyImport_ExecCodeModule(fname, CodeObj);
	if (result == NULL) {
		ssuge::ScriptManager::handle_error();
	}
}

void ssuge::ScriptManager::handle_error()
{
	std::string errString;
	PyObject* pErr;
	PyObject* pErrType = NULL, * pErrValue = NULL, * pErrTraceback = NULL;

	// Make sure we really have an error.  If not, return.
	pErr = PyErr_Occurred();
	if (!pErr)
		return;

	PyErr_Fetch(&pErrType, &pErrValue, &pErrTraceback);
	// Sometimes the errors fetched on the line above aren't fully-formed.  This
	// fleshes out the exception, if necessary.
	PyErr_NormalizeException(&pErrType, &pErrValue, &pErrTraceback);
	if (pErrTraceback == NULL)
	{
		// We don't have a traceback -- just convert the error value to a string and use that.  This
		// is useful for syntax errors
		PyObject* temp_str = PyObject_Str(pErrValue);
		errString = std::string(PyUnicode_AsUTF8(temp_str));
		Py_DECREF(temp_str);
	}
	else
	{
		// In case we fail along the way, generate *some* kind of error message
		errString = "Can't generate stack trace...";

		// In python, you can generate a stack trace (like Idle does), by doing this:
		// import traceback
		// import sys
		// ... (when an error has occurred)
		//    exception_type, exception_value, exception_traceback = sys.exc_info()  # Like PyErr_Fetch above
		//    s = traceback.format_exception(exception_type, exception_value, exception_traceback)
		//    print(s)
		// We want to do essentially the same thing here.
		PyObject* traceback_module = PyImport_ImportModule("traceback");
		if (traceback_module)
		{
			PyObject* format_exception_func = PyObject_GetAttrString(traceback_module, "format_exception");
			if (format_exception_func && PyCallable_Check(format_exception_func))
			{
				// Construct a tuple to pass some arguments to format_exception
				PyObject* args_tuple = PyTuple_New(3);
				// We don't own these objects -- incref them b/c the PyTuple_SetItem steals a reference
				Py_INCREF(pErrType);
				Py_INCREF(pErrValue);
				Py_INCREF(pErrTraceback);// new reference
				PyTuple_SetItem(args_tuple, 0, pErrType);
				PyTuple_SetItem(args_tuple, 1, pErrValue);
				PyTuple_SetItem(args_tuple, 2, pErrTraceback);
				PyObject* result = PyObject_Call(format_exception_func, args_tuple, NULL);
				if (result != NULL && PyList_Check(result))
				{
					errString = "";
					for (int i = 0; i < PyList_Size(result); i++)
						errString += std::string(PyUnicode_AsUTF8(PyList_GetItem(result, i)));

					Py_XDECREF(result);
				}

				Py_XDECREF(args_tuple);
			}

			Py_XDECREF(format_exception_func);
			Py_DECREF(traceback_module);
		}

	}
	LOG_MANAGER->log(errString, Ogre::ColourValue(1.0f, 0.8f, 0.2f), 30.0f);
}