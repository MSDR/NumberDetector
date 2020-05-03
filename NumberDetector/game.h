#ifndef GAME_H_
#define GAME_H_

#include <algorithm>?
#include <time.h>
#include <vector>

#include "graphics.h"
#include "input.h"
#include "sprite.h"
#include "text.h"

class Game {
public:
	Game();
	~Game();

private:
	void gameLoop();

	void draw(Graphics &graphics);
	void drawBackground(SDL_Renderer* renderer);
	void drawBrushOutline(SDL_Renderer* renderer, int mouseX, int mouseY);
	void drawCanvas(SDL_Renderer* renderer);
	void update(float elapsedTime);

	bool collectingData_;

	int canvasNum_;
	Text* numRequestLine_;

	int** canvas_;
	std::vector<int**> canvases_;
	//Sprite* brushOutline_;
};

#endif GAME_H_