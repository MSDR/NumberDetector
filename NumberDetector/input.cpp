//Input class adpated from LimeOats: https://limeoats.com/, modified and commented by Mason Sklar [2020]

#include "input.h"

Input::Input() {
}

Input::~Input() {
}

void Input::beginNewFrame() {
	pressedKeys_.clear();
	releasedKeys_.clear();
}

void Input::keyUpEvent(const SDL_Event& event) {
	releasedKeys_[event.key.keysym.scancode] = true;
	heldKeys_[event.key.keysym.scancode] = false;
}

void Input::keyDownEvent(const SDL_Event& event) {
	pressedKeys_[event.key.keysym.scancode] = true;
	heldKeys_[event.key.keysym.scancode] = true;
}

bool Input::wasKeyPressed(SDL_Scancode key) {
	return pressedKeys_[key];
}

bool Input::wasKeyReleased(SDL_Scancode key) {
	return releasedKeys_[key];
}

bool Input::isKeyHeld(SDL_Scancode key) {
	return heldKeys_[key];
}