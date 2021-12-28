#pragma once
#include <stdafx.h>


namespace ssuge {
	template<class T>
	class Singleton {
		///---ATTRIBUTES---///
	protected:
		static T* msSingleton;

		/// ---CONSTRUCTOR/DESTRUCTOR---///
	public:
		Singleton() {
			if (msSingleton == NULL) {
				msSingleton = (T*)this;	/// cast the singleton to the type that we want to make it
			}
			else {
				/// give error for trying to make multiple singletons of a class
				throw std::runtime_error("cannot create muiltiple singletons of the same type");
			}
			
		}
		~Singleton() {
			msSingleton = NULL;
		}

		/// ---GETTERS/SETTERS--- ///
	public:
		static T* get_singleton_ptr() {
			return msSingleton;
		}
		///---OTHER METHODS---///
	};
}