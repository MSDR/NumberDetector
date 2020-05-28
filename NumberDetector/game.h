#ifndef GAME_H_
#define GAME_H_

#include <algorithm>?
#include <fstream>
#include <list>
#include <string>
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
	void cnnPass(bool printStats = true, bool stop = false);

	void draw(Graphics &graphics);
	void drawBackground(SDL_Renderer* renderer);
	void drawBrushOutline(SDL_Renderer* renderer, int mouseX, int mouseY);
	void drawCanvas(SDL_Renderer* renderer);
	void update(float elapsedTime);

	std::string filepath_;
	void writeData(std::string &filepath, bool append);
	void trainFromData(std::string &filepath, int epochs = -1);
	void loadData(std::string &filepath);

	bool printForExcel_;
	bool collectingData_;
	bool emptyCanvas_;

	double learnRate_;
	std::list<double> lossCache_;
	int guess_;
	int canvasNum_;
	Text* numRequestLine_;
	matrix canvas_;
	std::vector<std::pair<matrix, int> > canvases_;
	//Sprite* brushOutline_;
};

#endif GAME_H_