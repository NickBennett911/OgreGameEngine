#include <stdafx.h>
#include <input_manager.h>
#include <log_manager.h>
#include <tinyxml2.h>
#include <application.h>

ssuge::InputManager* ssuge::Singleton<ssuge::InputManager>::msSingleton = NULL;

ssuge::InputManager::InputManager(std::string bindings_file) {
	mKeyNames = 
    {
        {"?", SDLK_UNKNOWN},
        {"return", SDLK_RETURN},
        {"escape", SDLK_ESCAPE},
        {"backspace", SDLK_BACKSPACE},
        {"tab", SDLK_TAB},
        {"space", SDLK_SPACE},
        {"!", SDLK_EXCLAIM},
        {"double-quote", SDLK_QUOTEDBL},
        {"#", SDLK_HASH},
        {"%", SDLK_PERCENT},
        {"$", SDLK_DOLLAR},
        {"&", SDLK_AMPERSAND},
        {"single-quote", SDLK_QUOTE},
        {"(", SDLK_LEFTPAREN},
        {")", SDLK_RIGHTPAREN},
        {"*", SDLK_ASTERISK},
        {"+", SDLK_PLUS},
        {",", SDLK_COMMA},
        {"-", SDLK_MINUS},
        {".", SDLK_PERIOD},
        {"/", SDLK_SLASH},
        {"0", SDLK_0},
        {"1", SDLK_1},
        {"2", SDLK_2},
        {"3", SDLK_3},
        {"4", SDLK_4},
        {"5", SDLK_5},
        {"6", SDLK_6},
        {"7", SDLK_7},
        {"8", SDLK_8},
        {"9", SDLK_9},
        {":", SDLK_COLON},
        {";", SDLK_SEMICOLON},
        {"<", SDLK_LESS},
        {", ", SDLK_EQUALS},
        {">", SDLK_GREATER},
        {"?", SDLK_QUESTION},
        {"@", SDLK_AT},

        {"[", SDLK_LEFTBRACKET},
        {"backslash", SDLK_BACKSLASH},
        {"]", SDLK_RIGHTBRACKET},
        {"^", SDLK_CARET},
        {"_", SDLK_UNDERSCORE},
        {"`", SDLK_BACKQUOTE},
        {"a", SDLK_a},
        {"b", SDLK_b},
        {"c", SDLK_c},
        {"d", SDLK_d},
        {"e", SDLK_e},
        {"f", SDLK_f},
        {"g", SDLK_g},
        {"h", SDLK_h},
        {"i", SDLK_i},
        {"j", SDLK_j},
        {"k", SDLK_k},
        {"l", SDLK_l},
        {"m", SDLK_m},
        {"n", SDLK_n},
        {"o", SDLK_o},
        {"p", SDLK_p},
        {"q", SDLK_q},
        {"r", SDLK_r},
        {"s", SDLK_s},
        {"t", SDLK_t},
        {"u", SDLK_u},
        {"v", SDLK_v},
        {"w", SDLK_w},
        {"x", SDLK_x},
        {"y", SDLK_y},
        {"z", SDLK_z},
        {"caps-lock", SDLK_CAPSLOCK},
        {"f1", SDLK_F1},
        {"f2", SDLK_F2},
        {"f3", SDLK_F3},
        {"f4", SDLK_F4},
        {"f5", SDLK_F5},
        {"f6", SDLK_F6},
        {"f7", SDLK_F7},
        {"f8", SDLK_F8},
        {"f9", SDLK_F9},
        {"f10", SDLK_F10},
        {"f11", SDLK_F11},
        {"f12", SDLK_F12},
        {"print-screen", SDLK_PRINTSCREEN},
        {"scroll-lock", SDLK_SCROLLLOCK},
        {"pause", SDLK_PAUSE},
        {"insert", SDLK_INSERT},
        {"home", SDLK_HOME},
        {"page-up", SDLK_PAGEUP},
        {"delete", SDLK_DELETE},
        {"end", SDLK_END},
        {"page-down", SDLK_PAGEDOWN},
        {"right", SDLK_RIGHT},
        {"left", SDLK_LEFT},
        {"down", SDLK_DOWN},
        {"up", SDLK_UP},
        {"numlock-clear", SDLK_NUMLOCKCLEAR},
        {"keypad-divide", SDLK_KP_DIVIDE},
        {"keypad-multiply", SDLK_KP_MULTIPLY},
        {"keypad-minus", SDLK_KP_MINUS},
        {"keypad-plus", SDLK_KP_PLUS},
        {"keypad-enter", SDLK_KP_ENTER},
        {"keypad-1", SDLK_KP_1},
        {"keypad-2", SDLK_KP_2},
        {"keypad-3", SDLK_KP_3},
        {"keypad-4", SDLK_KP_4},
        {"keypad-5", SDLK_KP_5},
        {"keypad-6", SDLK_KP_6},
        {"keypad-7", SDLK_KP_7},
        {"keypad-8", SDLK_KP_8},
        {"keypad-9", SDLK_KP_9},
        {"keypad-0", SDLK_KP_0},
        {"keypad-peiod", SDLK_KP_PERIOD},
        {"application", SDLK_APPLICATION},
        {"power", SDLK_POWER},
        {"keypad-equal", SDLK_KP_EQUALS},
        {"f13", SDLK_F13},
        {"f14", SDLK_F14},
        {"f15", SDLK_F15},
        {"f16", SDLK_F16},
        {"f17", SDLK_F17},
        {"f18", SDLK_F18},
        {"f19", SDLK_F19},
        {"f20", SDLK_F20},
        {"f21", SDLK_F21},
        {"f22", SDLK_F22},
        {"f23", SDLK_F23},
        {"f24", SDLK_F24},
        {"execute", SDLK_EXECUTE},
        {"help", SDLK_HELP},
        {"menu", SDLK_MENU},
        {"select", SDLK_SELECT},
        {"stop", SDLK_STOP},
        {"again", SDLK_AGAIN},
        {"undo", SDLK_UNDO},
        {"cut", SDLK_CUT},
        {"copy", SDLK_COPY},
        {"paste", SDLK_PASTE},
        {"find", SDLK_FIND},
        {"mute", SDLK_MUTE},
        {"volume-up", SDLK_VOLUMEUP},
        {"volume-down", SDLK_VOLUMEDOWN},
        {"comma", SDLK_KP_COMMA},
        {"keypad-(", SDLK_KP_LEFTPAREN},
        {"keypad-)", SDLK_KP_RIGHTPAREN},
        {"keypad-{", SDLK_KP_LEFTBRACE},
        {"keypad-}", SDLK_KP_RIGHTBRACE},
        {"keypad-tab", SDLK_KP_TAB},
        {"keypad-backspace", SDLK_KP_BACKSPACE},
        {"left-ctrl", SDLK_LCTRL},
        {"left-shift", SDLK_LSHIFT},
        {"left-alt", SDLK_LALT},
        {"left-gui", SDLK_LGUI},
        {"right-ctrl", SDLK_RCTRL},
        {"right-shift", SDLK_RSHIFT},
        {"right-alt", SDLK_RALT},
        {"right-gui", SDLK_RGUI}
    };

    mGamepadButtonNames = 
    {
        {"a", SDL_CONTROLLER_BUTTON_A},
        {"b", SDL_CONTROLLER_BUTTON_B },
        {"x", SDL_CONTROLLER_BUTTON_X},
        {"y", SDL_CONTROLLER_BUTTON_Y},
        {"back", SDL_CONTROLLER_BUTTON_BACK},
        {"guide", SDL_CONTROLLER_BUTTON_GUIDE},
        {"start", SDL_CONTROLLER_BUTTON_START},
        {"left-stick", SDL_CONTROLLER_BUTTON_LEFTSTICK},
        {"right-stick", SDL_CONTROLLER_BUTTON_RIGHTSTICK},
        {"left-shoulder", SDL_CONTROLLER_BUTTON_LEFTSHOULDER},
        {"right-shoulder", SDL_CONTROLLER_BUTTON_RIGHTSHOULDER},
        {"dpad-up", SDL_CONTROLLER_BUTTON_DPAD_UP},
        {"dpad-down", SDL_CONTROLLER_BUTTON_DPAD_DOWN},
        {"dpad-left", SDL_CONTROLLER_BUTTON_DPAD_LEFT},
        {"dpad-right", SDL_CONTROLLER_BUTTON_DPAD_RIGHT},
        {"max", SDL_CONTROLLER_BUTTON_MAX}
    };

    mGamepadAxisNames = 
    {
        {"left-x", SDL_CONTROLLER_AXIS_LEFTX},
        {"left-y", SDL_CONTROLLER_AXIS_LEFTY},
        {"right-x", SDL_CONTROLLER_AXIS_RIGHTX},
        {"right-y", SDL_CONTROLLER_AXIS_RIGHTY},
        {"trigger-left", SDL_CONTROLLER_AXIS_TRIGGERLEFT},
        {"trigger-right", SDL_CONTROLLER_AXIS_TRIGGERRIGHT},
        {"max", SDL_CONTROLLER_AXIS_MAX}
    };

    load_bindings(bindings_file);
}

ssuge::InputManager::~InputManager() {

}

bool ssuge::InputManager::has_axis(std::string name)
{
    auto axisit = mAxisValues.find(name);
    if (axisit != mAxisValues.end())
        return true;
    return false;
}

bool ssuge::InputManager::has_action(std::string name)
{
    auto actionit = mActionValues.find(name);
    if (actionit != mActionValues.end())
        return true;
    return false;
}

float ssuge::InputManager::get_axis(std::string name)
{
    if (!has_axis(name)) {
        LOG_MANAGER->log(name + ": is not bound as an axis", (1, 0, 0), 3.0);
        return 0.f;
    }
    auto axisit = mAxisValues.find(name);
    return axisit->second;
}

bool ssuge::InputManager::is_action_active(std::string name)
{
    if (!has_action(name)) {
        LOG_MANAGER->log(name + ": is not bound as an action", (1, 0, 0), 3.0);
        return false;
    }
    auto actionit = mActionValues.find(name);
    if (actionit->second > 0)
        return true;
    return false;
}


void ssuge::InputManager::load_bindings(std::string file) {
    tinyxml2::XMLDocument doc;
    const char* charname = file.c_str();

    tinyxml2::XMLError result = doc.LoadFile(charname);
    if (result != tinyxml2::XMLError::XML_SUCCESS) {
        LOG_MANAGER->log("couldnt load bindings file: " + file, 5);
        return;
    }

    tinyxml2::XMLElement* root = doc.RootElement();
    if (root == nullptr) {
        LOG_MANAGER->log("couldnt get root node from bindings file: " + file, 5);
        return;
    }

    LOG_MANAGER->log(root->Name()); // this will be input_bindings
    tinyxml2::XMLElement* actions = root->FirstChildElement();   // this is the actions element
    for (tinyxml2::XMLElement* child = actions->FirstChildElement(); child != nullptr; child = child->NextSiblingElement()) {
        std::string name = (std::string)child->Attribute("name");
        std::string type = (std::string)child->Attribute("type");
        std::string value = (std::string)child->Attribute("value");

        if (type == "key") {
            mKeyActionBindings[mKeyNames[value]] = name;
            create_action(name);
        }
        else if (type == "gamepad") {
            int pad_id = child->IntAttribute("id_num");
            auto id_it = mGamepadButtonBindings.find(pad_id);
            if (id_it == mGamepadButtonBindings.end()) {
                // create and add a gamepad to mGamepads
            }
            std::map<SDL_GameControllerButton, std::string> buttons = mGamepadButtonBindings[pad_id];       // get buttons for the gamepad desired
            buttons[mGamepadButtonNames[value]] = name;         // add the button and corresponding a binding name for that button
        }
    }

    tinyxml2::XMLElement* axes = actions->NextSiblingElement();   // this is the actions element
    for (tinyxml2::XMLElement* child = axes->FirstChildElement(); child != nullptr; child = child->NextSiblingElement()) {
        std::string name = (std::string)child->Attribute("name");
        std::string type = (std::string)child->Attribute("type");

        if (type == "key") {
            std::string pos = (std::string)child->Attribute("positive");
            std::string neg = (std::string)child->Attribute("negative");

            std::pair<std::string, float> pos_pair = std::make_pair(name, 1);
            std::pair<std::string, float> neg_pair = std::make_pair(name, -1);

            mKeyAxisBindings[mKeyNames[pos]] = pos_pair;
            mKeyAxisBindings[mKeyNames[neg]] = neg_pair;

            create_axis(name);
        }
    }
}

void ssuge::InputManager::create_axis(std::string name) {
    if (!has_axis(name)) {
        mAxisValues[name] = 0.0;
    }
}

void ssuge::InputManager::create_action(std::string name) {
    if (!has_action(name)) {
        mActionValues[name] = 0;
    }
}

void ssuge::InputManager::register_listener(InputListener* gobj)
{
    mListeners.insert(gobj);
}

void ssuge::InputManager::deregister_listener(InputListener* gobj)
{
    mListeners.erase(gobj);
}

void ssuge::InputManager::broadcast_action(std::string action, bool is_pressed)
{
   
    for (ssuge::InputListener* listener : mListeners) {
        if (is_pressed)
            listener->action_pressed(action);
        else if (!is_pressed)
            listener->action_released(action);
    }
}

void ssuge::InputManager::broadcast_axis_event(std::string axis, float new_value)
{
    for (ssuge::InputListener* listener : mListeners) {
        listener->axis_updated(axis, new_value);
    }
}

void ssuge::InputManager::scan_for_gamepads()
{
}

void ssuge::InputManager::update(float dt) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {   // only want first notification of key down not the ones that follow

            auto actionit = mKeyActionBindings.find(e.key.keysym.sym);
            if (actionit != mKeyActionBindings.end()) {
                handle_action_key(e.key.keysym.sym, true);
            }

            auto axisit = mKeyAxisBindings.find(e.key.keysym.sym);
            if (axisit != mKeyAxisBindings.end()) {
                handle_axis_key(e.key.keysym.sym, true);
            }
        }

        if (e.type == SDL_KEYUP && e.key.repeat == 0) {
            auto actionit = mKeyActionBindings.find(e.key.keysym.sym);
            if (actionit != mKeyActionBindings.end()) {
                handle_action_key(e.key.keysym.sym, false);
            }

            auto axisit = mKeyAxisBindings.find(e.key.keysym.sym);
            if (axisit != mKeyAxisBindings.end()) {
                handle_axis_key(e.key.keysym.sym, false);
            }
        }
    }
}

void ssuge::InputManager::handle_axis_key(SDL_Keycode k, bool is_pressed) {
    
    auto axisit = mKeyAxisBindings.find(k);

    if (is_pressed) {
        mAxisValues[axisit->second.first] = axisit->second.second;
    }
    if (!is_pressed) {
        mAxisValues[axisit->second.first] = 0;
    }

    broadcast_axis_event(axisit->second.first, mAxisValues[axisit->second.first]);
}

void ssuge::InputManager::handle_action_key(SDL_Keycode k, bool is_pressed) {
    if (is_pressed) {
        mActionValues[mKeyActionBindings[k]] += 1;
    }

    if (!is_pressed) {
        mActionValues[mKeyActionBindings[k]] -= 1;
    }

    broadcast_action(mKeyActionBindings[k], is_pressed);
}

void ssuge::InputManager::handle_joy_button(SDL_GameControllerButton button, int joy_id, bool is_pressed)
{
}

void ssuge::InputManager::handle_joy_axis(SDL_GameControllerAxis axis, int joy_id, Sint16 value)
{
}
