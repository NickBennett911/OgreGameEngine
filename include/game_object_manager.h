#pragma once
#include <stdafx.h>
#ifndef _GAME_OBJECT_MANAGER_H_
#define _GAME_OBJECT_MANAGER_H_

#include <singleton.h>
#include <game_object.h>
#include <tinyxml2.h>
#include <log_manager.h>

#define GAME_OBJECT_MANAGER ssuge::GameObjectManager::get_singleton_ptr()

namespace ssuge
{
	/// The GameObjectManager (or GOM for short) is a collection of GameObjects.  This class
	/// is a "Factory" for GameObjects (which means it is the only way to create / destroy
	/// GameObjects, plus there are ways to access a GameObject via the GOM).  The organization
	/// is pretty simple for now -- we have any number of uniquely-named groups.  Within that group
	/// is a collection of uniquely named GameObjects (a map-of-maps) -- in theory a GameObject name
	/// only has to be unique within the group, but when finding a GameObject solely by name, it is 
	/// non-deterministic which one will be returned.
	class GameObjectManager : public Singleton<GameObjectManager>
	{
		// ***** ATTRIBUTES *****
	public:
		std::map<std::string, std::map<std::string, ssuge::GameObject*>> mGameObjects;

		std::vector<ssuge::GameObject*> mDestructionQueue;

		// ***** CONSTRUCTOR / DESTRUCTOR *****
	public:
		/// Constructor
		GameObjectManager();

		/// Destructor
		~GameObjectManager();

		// ***** GETTERS / SETTERS *****
	public:
		/// Gets a game object (NULL if it doesn't exist) -- more costly: searches through all groups
		GameObject* get_game_object(std::string game_object_name);

		/// Gets a game object within a group (NULL if it doesn't exist)
		GameObject* get_game_object(std::string game_object_name, std::string group_name);

		/// Gets all game objects within a group.  Note: returning a std::vector pre-C++11 was a bad idea (it would make_heap
		/// a copy).  Since then, the "move semantics" make this a cheap operation.
		std::vector<ssuge::GameObject*> get_game_objects(std::string group_name);

		/// Returns true if there is a group by this name
		bool has_group(std::string group_name);

		// ***** METHODS ***** 
	public:
		/// Destroys a game object (faster version) if you know the group name
		bool destroy_game_object(std::string group_name, std::string gobj_name);

		/// Destroy a game object (slower version) if you don't know the group name.  Returns true if that game object was destroyed
		/// (i.e. was it found)
		bool destroy_game_object(std::string gobj_name);

		std::string get_group(std::string gobj_name);

		/// Destroys all game objects within the given group.  If destroy_group is true, also remove the group.  
		/// Returns true if that game object was destroyed (i.e. was it found).
		bool group_destroy(std::string group_name, bool destroy_group);

		/// Sets the visibility of all game objects within the given group.
		void set_visibility(std::string group_name, bool is_visible);

		/// Creates a new game object.  This creates a group as well, if the given one doesn't exist.
		GameObject* create_game_object(std::string group_name, std::string object_name, GameObject* parent = NULL, unsigned int tag = 0);

		/// iterates over all game objects and calls their update.
		/// also calls the script update method if the object has a script mTwin
		void update_game_objects(float dt);

		/// recursively goes through the xml fname given loading in and sting game objects as it goes
		tinyxml2::XMLError load_scene_file(std::string group_name, std::string fname);

		/// finds and returns the given ridgidbody
		btRigidBody* find_rigid_body(btRigidBody body);

		/// this will add the given obj to the mDestrucionQueue so that the next time the destruction queue can be purged safely it will get destroyed
		inline void queue_game_object_destruction(ssuge::GameObject* obj) { mDestructionQueue.push_back(obj); }

		/// destroys all the game objects in the destrucion queue
		void purge_destruction_queue();

	protected:
		/* HELPER FUNCTIONS FOR THE LOAD SCENE FILE - THESE WILL NEVER BE USED ANYWHERE ELSE */
		void process_XML_node(tinyxml2::XMLElement * elem, GameObject* cur_parent);

		void process_LIGHT_node(tinyxml2::XMLElement* elem, GameObject* cur_parent);

		bool process_PROPERTY_node(tinyxml2::XMLElement* elem, GameObject* cur_parent, std::string objname, bool handle_as_property_check);

		bool has_class_property(tinyxml2::XMLElement* elem, GameObject* cur_parent, std::string objname);

	};
}

#endif