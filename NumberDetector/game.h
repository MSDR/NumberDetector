#ifndef GAME_H_
#define GAME_H_

#include <algorithm>?

#include "graphics.h"
#include "input.h"
#include "sprite.h"

class Game {
public:
	Game();
	~Game();

private:
	void gameLoop();

	void draw(Graphics &graphics);
	void drawBackground(SDL_Renderer* renderer);
	void drawCanvas(SDL_Renderer* renderer);
	void update(float elapsedTime);

	int canvas_[28][28];
	//Sprite* brushOutline_;
};

#endif GAME_H_