#pragma once
#include <stdafx.h>
#include <singleton.h>

#define LOG_MANAGER ssuge::LogManager::get_singleton_ptr()

namespace ssuge {
	class LogManager : public Singleton<LogManager> {
	///---ATTRIBUTES---///
	protected:
		struct LogEntry {
			std::string mText;
			float mTimeTillDeath;
			Ogre::ColourValue mColor;
		};

		std::ofstream* mLog;

		Ogre::Overlay* mLogOverlay;
		std::vector<Ogre::TextAreaOverlayElement*> mLogSlots;
		std::vector<ssuge::LogManager::LogEntry> mLogInformation;

		int mMaxOnScreenMessages;
	
	/// ---CONSTRUCTOR/DESTRUCTOR---///
	public:
		LogManager(std::string log_fname = "log.txt", int num_text_slots = 30);
		~LogManager(); 

	/// ---GETTERS/SETTERS--- ///

	///---OTHER METHODS---///
	protected:
		/// responsible for scrolling the on screen message up when one despawns
		void resink_log();

	public:
		/// will just print message to the txt file given to the LogManager when initialized
		void log(std::string message);
		/// will print message to file and on screen
		void log(std::string message, Ogre::ColourValue color, float time_to_keep_on_screen);
		/// will print message to file and on screen, makes a random color automatically
		void log(std::string message, float time_to_keep_on_screen);
		/// prints to log file and screen, can specify the alpha of the text
		void log(std::string message, float alpha, float time_to_keep_on_screen);
		/// responsible for updating the mTimeTillDeath inside LogEntries also calls resink_log()
		void update_log_manager(float dt);
	};
}
