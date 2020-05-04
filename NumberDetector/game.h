#ifndef GAME_H_
#define GAME_H_

#include <algorithm>?
#include <time.h>
#include <vector>

#include "cnn.h"
#include "graphics.h"
#include "input.h"
#include "sprite.h"
#include "text.h"

using globals::matrix;

class Game {
public:
	Game();
	~Game();

private:
	void gameLoop();

	CNN cnn_;

	void draw(Graphics &graphics);
	void drawBackground(SDL_Renderer* renderer);
	void drawBrushOutline(SDL_Renderer* renderer, int mouseX, int mouseY);
	void drawCanvas(SDL_Renderer* renderer);
	void update(float elapsedTime);

	bool collectingData_;
	bool emptyCanvas_;

	int canvasNum_;
	Text* numRequestLine_;
	matrix canvas_;
	std::vector<matrix> canvases_;
	//Sprite* brushOutline_;
};

#endif GAME_H_