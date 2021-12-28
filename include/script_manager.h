#pragma once
#include <stdafx.h>
#include <singleton.h>

#define SCRIPTMANAGER ssuge::ScriptManager::get_singleton_ptr()

namespace ssuge {
	class ScriptManager : public Singleton<ScriptManager> {
		///---ATTRIBUTES---///
	protected:
		

		/// ---CONSTRUCTOR/DESTRUCTOR---///
	public:
		ScriptManager();
		~ScriptManager();
		PyObject* mModule;

		/// ---GETTERS/SETTERS--- ///

		///---OTHER METHODS---///
	public:
		void handle_error();
		void run_method(PyObject* obj, std::string meth_name, PyObject* tuple);

	public:
		void run_script(const char * fname);

		PyObject* get_main_module() { return mModule;  }

		//friend PyObject* PyInit_ssuge_module(void);
	};
}