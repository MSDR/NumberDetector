//Input class adapted from LimeOats: https://limeoats.com/, modified and commented by Mason Sklar [2020]

#ifndef INPUT_H_
#define INPUT_H_

#include <map>
#include "SDL_include/SDL.h"

class Input {
public:
	Input();
	~Input();

	//Resets keys that are no longer relevant
	void beginNewFrame();

	//Called upon a key release
	void keyUpEvent(const SDL_Event& event);

	//Called upon a key press
	void keyDownEvent(const SDL_Event& event);

	//Checks if a key is pressed in the current frame
	bool wasKeyPressed(SDL_Scancode key);

	//Checks if a key is released in the current frame
	bool wasKeyReleased(SDL_Scancode key);

	//Checks if a key is being held in the current frame
	bool isKeyHeld(SDL_Scancode key);

private:
	std::map<SDL_Scancode, bool> heldKeys_;
	std::map<SDL_Scancode, bool> pressedKeys_;
	std::map<SDL_Scancode, bool> releasedKeys_;
};

#endif INPUT_H_