#include <stdafx.h>
#include <component.h>
#include <game_object.h>

ssuge::Component::Component(ssuge::GameObject* owner) : mOwningGameObject(owner) {
	// constructor
}

ssuge::Component::~Component() {
	// destructor
}

void ssuge::Component::set_visible(bool isVisible) {

}

void ssuge::Component::update(float dt) {
	// in update
}