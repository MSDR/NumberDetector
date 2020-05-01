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

	void update(float elapsedTime);

	Sprite sprite_;
};

#endif GAME_H_