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
	Game(std::string &filepath);
	~Game();

	//The filepath of the data file. Should be a properly formatted .txt, format check not implemented
	std::string filepath_;

private:
	void gameLoop();

	CNN cnn_;

	//Passes canvas_ through the network
	//Includes backpropagation if collectingData
	void cnnPass(bool printStats = true);

	//Draws all graphics to the window
	void draw(Graphics &graphics);
	void drawBackground(SDL_Renderer* renderer);
	void drawBrushOutline(SDL_Renderer* renderer, int mouseX, int mouseY);
	void drawCanvas(SDL_Renderer* renderer);
	Text* numRequestLine_;

	//Writes data from canvases_ into data file
	//If append, appends memory [canvases_] onto file, otherwise replaces data with memory
	void writeData(std::string &filepath, bool append);

	//Trains CNN from data file
	//If epochs = -1 [default], trains as many epochs as available data
	void trainFromData(std::string &filepath, int epochs = -1);

	//Loads data from file into memory
	void loadData(std::string &filepath);
	
	//If true, will print data about training progress in excel-compatible formatting
	bool printForExcel_; 

	//Controls whether a pass will backpropagate, whether user is training or using the network
	bool collectingData_;

	//True if canvas_ is currently empty
	bool emptyCanvas_;

	//The learnrate for backpropagation
	double learnRate_;
	std::list<double> lossCache_;

	int guess_; //CNN's guess for number on canvas, entered after a pass
	int canvasNum_; //True number canvas should represent, only used when collectingData
	matrix canvas_; //A 2D vector of doubles holding the current canvas
	std::vector<std::pair<matrix, int> > canvases_; //All entered or loaded canvases in memory
};

#endif GAME_H_