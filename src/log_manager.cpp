#include <stdafx.h>
#include <log_manager.h>
#include <Overlay/OgreTextAreaOverlayElement.h>
#include <Overlay/OgreFontManager.h>
#include <ctime>
#pragma warning(disable : 4996)

ssuge::LogManager* ssuge::Singleton<ssuge::LogManager>::msSingleton = NULL;		/// this allows to set the msSingleton in singleton class 

ssuge::LogManager::LogManager(std::string log_fname, int num_text_slots) : mMaxOnScreenMessages(num_text_slots) {
	///setting up and opening the log_fname
	mLog = new std::ofstream(log_fname, std::ios::out);
	
	Ogre::OverlayManager& overlayManager = Ogre::OverlayManager::getSingleton();

	/// Create a panel
	Ogre::OverlayContainer* panel = (Ogre::OverlayContainer*)(overlayManager.createOverlayElement("Panel", "LogPanel"));
	panel->setMetricsMode(Ogre::GMM_PIXELS);
	panel->setPosition(10, 10);
	panel->setDimensions(100, 100);
	///panel->setMaterialName("MaterialName"); // Optional background material

	/// Create all the text areas
	int text_size = 14;
	for (int i = 0; i < num_text_slots; i++)
	{
		Ogre::TextAreaOverlayElement* textArea = static_cast<Ogre::TextAreaOverlayElement*>(overlayManager.createOverlayElement("TextArea", "text_slot" + std::to_string(i)));
		mLogSlots.push_back(textArea);
		textArea->setMetricsMode(Ogre::GMM_PIXELS);
		textArea->setPosition(0, i* text_size);
		textArea->setDimensions(100, 100);
		textArea->setCaption("");
		textArea->setCharHeight(text_size);
		textArea->setFontName("SdkTrays/AppleTea");
		textArea->setColourBottom(Ogre::ColourValue(0.3, 0.5, 0.3));
		textArea->setColourTop(Ogre::ColourValue(0.5, 0.7, 0.5));

		/// Add the text area to the panel
		panel->addChild(textArea);
	}
	

	/// Create an overlay, and add the panel
	Ogre::Overlay* overlay = overlayManager.create("OverlayName");
	overlay->add2D(panel);


	/// Show the overlay
	overlay->show();

}

ssuge::LogManager::~LogManager() {
	mLog->close();
	delete mLogOverlay;

}

void ssuge::LogManager::log(std::string message) {
	std::time_t ttime = time(0);
	std::tm* local_time = localtime(&ttime);
	std::string log_timestamp = "";
	std::string time_indicator = "am";
	log_timestamp += std::to_string(1 + local_time->tm_mon);
	log_timestamp += "/";
	log_timestamp += std::to_string(local_time->tm_mday);
	log_timestamp += "/";
	log_timestamp += std::to_string(1900 + local_time->tm_year);
	log_timestamp += "@";
	int hr = local_time->tm_hour;
	if (hr > 12)
	{
		hr -= 12;
		time_indicator = "pm";
	}
	log_timestamp += std::to_string(hr);
	log_timestamp += ":";
	log_timestamp += std::to_string(1 + local_time->tm_min);
	log_timestamp += ":";
	log_timestamp += std::to_string(1 + local_time->tm_sec);
	log_timestamp += time_indicator;
	log_timestamp += "	";
	log_timestamp += message;

	*mLog << log_timestamp << std::endl;
}

void ssuge::LogManager::log(std::string message, Ogre::ColourValue color, float time_to_keep_on_screen) {


	if (mLogInformation.size() < mMaxOnScreenMessages-1)
	{
		ssuge::LogManager::LogEntry newEntry;
		newEntry.mText = message;
		newEntry.mColor = color;
		newEntry.mTimeTillDeath = time_to_keep_on_screen;
		mLogInformation.push_back(newEntry);
		this->log(message);
	}
	else 
	{
		this->log("WARNING: input on next line cant be logged on screen, log vector full");
		this->log(message);
	}

}

void ssuge::LogManager::log(std::string message, float time_to_keep_on_screen) {

	Ogre::ColourValue color(ssuge::RandomFloat(0.2, 1.0),
		ssuge::RandomFloat(0.2, 1.0),
		ssuge::RandomFloat(0.2, 1.0),
		1.0);

	this->log(message, color, time_to_keep_on_screen);	
}

void ssuge::LogManager::log(std::string message, float alpha, float time_to_keep_on_screen) {
	if (alpha < 0.f)
		alpha = 0.f;
	else if (alpha > 1.f)
		alpha = 1.f;

	Ogre::ColourValue color(ssuge::RandomFloat(0.2, 1.0),
		ssuge::RandomFloat(0.2, 1.0),
		ssuge::RandomFloat(0.2, 1.0),
		alpha);

	this->log(message, color, time_to_keep_on_screen);
}

void ssuge::LogManager::resink_log() {
	/// loop that moves everything up on-screen after a log is destroyed
	for (int i = 0; i < mLogInformation.size(); i++)
	{
		mLogSlots[i]->setCaption(mLogInformation[i].mText);
		mLogSlots[i]->setColour(mLogInformation[i].mColor);
	}
	mLogSlots[mLogInformation.size()]->setCaption("");	/// clears the caption of the last one avaliable
}

void ssuge::LogManager::update_log_manager(float dt) {
	for (int i = 0; i < mLogInformation.size();)
	{
		mLogInformation[i].mTimeTillDeath -= dt;
		if (mLogInformation[i].mTimeTillDeath < 0) 
		{
			mLogInformation.erase(mLogInformation.begin()+i);
			this->resink_log();
		}
		else 
		{
			i++;
		}
	}
	this->resink_log();
}