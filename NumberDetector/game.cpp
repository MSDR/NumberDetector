#include "game.h"


Game::Game() {
	SDL_Init(SDL_INIT_EVERYTHING);
	//IMG_Init(IMG_INIT_PNG);
	if (TTF_Init() == -1) {
		std::cout << TTF_GetError() << std::endl;
		SDL_Quit();
	}

	srand(time(0)); 

	learnRate_ = 0.001;
	cnn_ = CNN(8, 10);

	gameLoop();
}

Game::~Game() {
	TTF_Quit();
	//IMG_Quit();
	SDL_Quit();
}

void Game::gameLoop() {
	//Initialize core program functionality
	Graphics graphics; //Game window established in here
	Input input;
	SDL_Event event;

	int LAST_UPDATE_TIME = SDL_GetTicks();

	//brushOutline_ = new Sprite(graphics, "Images/BrushOutline.png", 0, 0, 92, 92); 

	for (int i = 0; i < 28; ++i) {
		canvas_.push_back(std::vector<double>());
		for (int j = 0; j < 28; ++j) {
			canvas_[i].push_back(-0.5);
		}
	}

	filepath_ = "large0-9.txt";
	bool confirmingDataClear = false;

	emptyCanvas_ = true;
	collectingData_ = true;
	printForExcel_ = false;
	guess_ = -1;

	//Set up number request text
	canvasNum_ = std::rand() % 10;
	numRequestLine_ = new Text();
	numRequestLine_->loadFont("Assets/Pixel NES.ttf", 2*globals::SPRITE_SCALE);
	numRequestLine_->update(graphics, "Draw a " + std::to_string(canvasNum_), { 0, 0, 0, 255 });

	//Start game loop
	while (true) {
		input.beginNewFrame();

		if (SDL_WaitEvent(&event) != 0) {
			if (event.type == SDL_KEYDOWN) {
				if (event.key.repeat == 0) {
					input.keyDownEvent(event);
					if (confirmingDataClear && !input.wasKeyPressed(SDL_SCANCODE_RETURN)) {
						confirmingDataClear = false;
						std::cout << "\nData will NOT be cleared.\n";
					}
					else if (confirmingDataClear && input.wasKeyPressed(SDL_SCANCODE_RETURN)) {
						confirmingDataClear = false;
						std::cout << "\nData clearing will proceed.\n";
						writeData(filepath_, false);
					}

				}
			} else if (event.type = SDL_KEYUP) {
				input.keyUpEvent(event);
			} else if (event.type == SDL_QUIT) {
				return;
			}
		}

		//Exit the program
		if (input.wasKeyPressed(SDL_SCANCODE_DELETE))
			return;

		if (confirmingDataClear)
			continue;
		

		if (input.wasKeyPressed(SDL_SCANCODE_RSHIFT))
			collectingData_ = !collectingData_;

		//Draw onto canvas
		if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT) || input.isKeyHeld(SDL_SCANCODE_D)) {
			emptyCanvas_ = false;
			int mouseX = 0;
			int mouseY = 0;
			SDL_GetMouseState(&mouseX, &mouseY);
			int cx = (mouseX - mouseX % globals::SPRITE_SCALE) / globals::SPRITE_SCALE - 3;
			int cy = (mouseY - mouseY % globals::SPRITE_SCALE) / globals::SPRITE_SCALE - 3;
			for (int i = cy; i < cy + 3; ++i) {
				for (int j = cx; j < cx + 3; ++j) {
					if(j <= 27 && i <= 27 && j >= 0 && i >= 0) 
						canvas_[i][j] = 0.5;
				}
			}
		}
		
		//Clear canvas
		if (input.wasKeyPressed(SDL_SCANCODE_Z) || input.wasKeyPressed(SDL_SCANCODE_BACKSPACE)) {
			emptyCanvas_ = true;
			for (int i = 0; i < 28; ++i) {
				for (int j = 0; j < 28; ++j) {
					canvas_[i][j] = -0.5;
				}
			}

			numRequestLine_->update(graphics, "Draw a " + (collectingData_ ? std::to_string(canvasNum_) : "number"), { 0, 0, 0, 255 });
			guess_ = -1;
		}

		//Submit a canvas
		if (!emptyCanvas_ && (input.wasKeyPressed(SDL_SCANCODE_E) || input.wasKeyPressed(SDL_SCANCODE_RETURN))) {
			emptyCanvas_ = true;
			matrix c;
			for (int i = 0; i < 28; ++i) {
				c.push_back(std::vector<double>());
				for (int j = 0; j < 28; ++j) {
					c[i].push_back(canvas_[i][j]);
					canvas_[i][j] = -0.5;
				}
			}
			canvases_.push_back(std::make_pair(c, canvasNum_));

			//Reset number request
			canvasNum_ = std::rand() % 10;
			numRequestLine_->update(graphics, "Draw a " + (collectingData_ ? std::to_string(canvasNum_) : "number"), { 0, 0, 0, 255 });
			guess_ = -1;
		}

		//Print canvases_ to the console
		if (input.wasKeyPressed(SDL_SCANCODE_S)) {
			std::cout << "\n __________________________________________________________\n";
			for(int v = 0; v < canvases_.size(); ++v){
				for (int i = -1; i < 28; ++i) {
					std::cout << "| ";
					for (int j = 0; j < 28; ++j) {
						std::cout << (i == -1 || canvases_[v].first[i][j] == -0.5  ? "  " : "[]") << (j == 27 ? "" : ""); //should only be -0.5 exactly if manually set
					}
					std::cout << " |\n";
				}
				std::cout << "|__________________________________________________________|\n";
			}
			std::cout << std::endl;
		}

		//Pass current canvas forward through the CNN
		if (input.wasKeyPressed(SDL_SCANCODE_F)) {
			cnnPass();
			numRequestLine_->update(graphics, "I guess " + std::to_string(guess_));
			draw(graphics);
		}

		if (input.wasKeyPressed(SDL_SCANCODE_W)) {
			if (input.isKeyHeld(SDL_SCANCODE_LSHIFT)) {
				confirmingDataClear = true;
				std::cout << "\nTotal canvases: " << canvases_.size();
				std::cout << "\nAre you sure you want to clear and replace all data in " << filepath_ << "?\n Press ENTER to continue, anything else to cancel.\n";
			} else {
				writeData(filepath_, true);
			}
		}

		if (input.wasKeyPressed(SDL_SCANCODE_T)){
			if (printForExcel_) std::cout << "Epoch	MeanLossDiff	Loss\n";
			bool cd = collectingData_;
			collectingData_ = true;
			trainFromData(filepath_);
			collectingData_ = cd;
		}

		if (input.wasKeyPressed(SDL_SCANCODE_L))
			loadData(filepath_);

		const int CURRENT_TIME_MILLIS = SDL_GetTicks();
		int ELAPSED_TIME_MILLIS = CURRENT_TIME_MILLIS - LAST_UPDATE_TIME;
		update(ELAPSED_TIME_MILLIS);
		LAST_UPDATE_TIME = CURRENT_TIME_MILLIS;
		draw(graphics);
	}//end game loop
}

void Game::cnnPass(bool printStats) {
	//Forward pass
	std::vector<double> result = cnn_.forward(canvas_);

	//Print results
	if(printStats){
		int guess = 0;
		for (int i = 0; i < result.size(); ++i) {
			guess = result[i] > result[guess] ? i : guess;
			std::cout << std::endl << i << " | Confidence: " << (result[i] < 0.0001 ? "~0.000" : " " + std::to_string(result[i]));
		}
		guess_ = guess;
		std::cout << "\nGuess: " << guess << std::endl;
	}
	if(collectingData_){
		double loss = -std::log(result[canvasNum_]);
		if(printStats && !printForExcel_) std::cout << "Loss: " << loss << std::endl;

		lossCache_.push_front(loss);
		//if (lossCache_.size() > 25) lossCache_.pop_back(); //25 chosen arbitrarily
		double diffTotal = 0.0;
		double prevLoss = -1.0;
		for (std::list<double>::iterator lossItr = lossCache_.begin(); lossItr != lossCache_.end(); ++lossItr) {
			if (prevLoss != -1.0) diffTotal += *lossItr - prevLoss;
			prevLoss = *lossItr;
		}
		if(printForExcel_) std::cout << diffTotal / lossCache_.size() << "	" << loss << std::endl;

		//Calculate gradient for backpropagation
		std::vector<double> gradient(10, 0);
		gradient[canvasNum_] = -(1 / result[canvasNum_]);
		//Backwards pass (adjusting weights)
		cnn_.backProp(gradient, learnRate_);
	}
}

void Game::draw(Graphics &graphics) {
	graphics.clear();

	drawBackground(graphics.getRenderer());
	drawCanvas(graphics.getRenderer());

	int mouseX, mouseY = 0;
	SDL_GetMouseState(&mouseX, &mouseY);
	drawBrushOutline(graphics.getRenderer(), mouseX, mouseY);
	//std::cout << mouseX-mouseX%globals::SPRITE_SCALE << " " << mouseY-mouseY%globals::SPRITE_SCALE << std::endl;
	
	//Draw text
	numRequestLine_->draw(graphics, 2 * globals::SPRITE_SCALE - 1, 30 * globals::SPRITE_SCALE);

	graphics.flip();
}

void Game::drawBackground(SDL_Renderer* renderer) {
	//Solid white background
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(renderer, NULL);

	//Drawing space outline
	SDL_Rect outline; outline.x = 2*globals::SPRITE_SCALE-1; outline.y = 2*globals::SPRITE_SCALE-1; 
							outline.w = (28*globals::SPRITE_SCALE)+2; outline.h = (28*globals::SPRITE_SCALE)+2;
	SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
	SDL_RenderDrawRect(renderer, &outline);
}

void Game::drawBrushOutline(SDL_Renderer* renderer, int mouseX, int mouseY) {
	SDL_SetRenderDrawColor(renderer, 238, 37, 37, 120);

	Vector2 p1, p2; 
	Vector2 bounds = Vector2(2*globals::SPRITE_SCALE, 30*globals::SPRITE_SCALE);

	//Upper line
	p1.x = (mouseX - mouseX % globals::SPRITE_SCALE) - globals::SPRITE_SCALE; 
	p1.y = (mouseY - mouseY % globals::SPRITE_SCALE) - globals::SPRITE_SCALE;
	p2.x = (mouseX - mouseX % globals::SPRITE_SCALE) + 2*globals::SPRITE_SCALE; 
	p2.y = p1.y;  
	p1.clampValues(bounds, bounds);
	p2.clampValues(bounds, bounds);
	SDL_RenderDrawLine(renderer, p1.x, p1.y, p2.x, p2.y);

	//Left line
	p1.x = (mouseX - mouseX % globals::SPRITE_SCALE) - globals::SPRITE_SCALE; 
	p1.y = (mouseY - mouseY % globals::SPRITE_SCALE) - globals::SPRITE_SCALE;
	p2.x = p1.x; 
	p2.y = (mouseY - mouseY % globals::SPRITE_SCALE) + 2*globals::SPRITE_SCALE;  
	p1.clampValues(bounds, bounds);
	p2.clampValues(bounds, bounds);
	SDL_RenderDrawLine(renderer, p1.x, p1.y, p2.x, p2.y);

	//Lower line
	p1.x = (mouseX - mouseX % globals::SPRITE_SCALE) - globals::SPRITE_SCALE; 
	p1.y = (mouseY - mouseY % globals::SPRITE_SCALE) + 2*globals::SPRITE_SCALE;
	p2.x = (mouseX - mouseX % globals::SPRITE_SCALE) + 2*globals::SPRITE_SCALE; 
	p2.y = p1.y;  
	p1.clampValues(bounds, bounds);
	p2.clampValues(bounds, bounds);
	SDL_RenderDrawLine(renderer, p1.x, p1.y, p2.x, p2.y);

	//Right line
	p1.x = (mouseX - mouseX % globals::SPRITE_SCALE) + 2*globals::SPRITE_SCALE; 
	p1.y = (mouseY - mouseY % globals::SPRITE_SCALE) - globals::SPRITE_SCALE;
	p2.x = p1.x; 
	p2.y = (mouseY - mouseY % globals::SPRITE_SCALE) + 2*globals::SPRITE_SCALE;  
	p1.clampValues(bounds, bounds);
	p2.clampValues(bounds, bounds);
	SDL_RenderDrawLine(renderer, p1.x, p1.y, p2.x, p2.y);
}

void Game::drawCanvas(SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_Rect r;
	r.w = globals::SPRITE_SCALE;
	r.h = globals::SPRITE_SCALE;

	for (int i = 0; i < 28; ++i) {
		//std::cout << std::endl;
		for (int j = 0; j < 28; ++j) {
			//std::cout << canvas_[i][j] << " ";
			if (canvas_[i][j] != -0.5) {
				r.x = 2 * globals::SPRITE_SCALE + j * globals::SPRITE_SCALE;
				r.y = 2 * globals::SPRITE_SCALE + i * globals::SPRITE_SCALE;
				SDL_RenderFillRect(renderer, &r);
			}
		}
	}
}
void Game::update(float elapsedTime) {
}

void Game::writeData(std::string &filepath, bool append) {
	std::ofstream writer;
	writer.open(filepath, std::fstream::out | (append ? std::fstream::app : std::fstream::trunc));
	if (!writer.good()) {
		std::cout << std::endl << "Write failed. Invalid filepath.\n";
		return;
	}

	//Write data
	for (int v = 0; v < canvases_.size(); ++v) {
		writer << "_" << canvases_[v].second << "_";
		for (int i = 0; i < canvases_[v].first.size(); ++i) {
			writer << std::endl;
			for (int j = 0; j < canvases_[v].first[i].size(); ++j) {
				writer << (canvases_[v].first[i][j]+0.5);
			}
		}
		writer << (v == canvases_.size() - 1 ? "" : "\n");
	}

	writer.close();

	std::cout << std::endl << "Write success" << (append ? " with append." : (". All data in " + filepath) + " has been cleared and replaced.") << std::endl;
}

void Game::trainFromData(std::string &filepath, int epochs) {
	double startTime = SDL_GetTicks();
	std::ifstream reader;
	reader.open(filepath_, std::fstream::in);
	if (!reader.good()) {
		std::cout << std::endl << "Training failed. Invalid filepath.\n";
		return;
	}

	canvas_.clear();
	int epochCounter = 0;
	std::string line = "This is a meaningless string.";

	std::cout << std::endl;

	while (std::getline(reader, line) && !cnn_.maxTrained_) {
		if (line[0] == '_') { //Run canvas through CNN
			if(epochCounter > 0) {
				if(printForExcel_) std::cout << epochCounter << "	";
				cnnPass(false); //REMOVE TERNARY
				if (!printForExcel_ && epochCounter % (epochs == -1 ? 50 : epochs/25) == 0) std::cout << "Completed " << epochCounter << " epochs.\n";
				if (epochs != -1 && epochCounter > epochs) break;
			}
			epochCounter++;
			canvas_.clear();
			canvasNum_ = line[1] - 48;
		} else {
			canvas_.push_back(std::vector<double>());
			for (int i = 0; i < line.length(); ++i) {
				canvas_.back().push_back(line[i] - 48.5);
			}
		}
	}

	std::cout << (cnn_.maxTrained_ ? "Halted training at " : "Completed training at ") << 
					 epochCounter << " epochs in " << 0.001*(SDL_GetTicks()-startTime) << " seconds.\n\n";

	canvas_.clear();
	for (int i = 0; i < 28; ++i) {
		canvas_.push_back(std::vector<double>());
		for (int j = 0; j < 28; ++j) {
			canvas_[i].push_back(-0.5);
		}
	}
}

void Game::loadData(std::string &filepath) {
	std::ifstream reader;
	reader.open(filepath_, std::fstream::in);

	std::string line = "Yahaha! You found me!";

	int loadCounter = 0;
	while (std::getline(reader, line)) {
		if (line[0] == '_') { 
			loadCounter++;
			canvases_.push_back(std::make_pair(matrix(), line[1]-48));
		} else {
			canvases_.back().first.push_back(std::vector<double>());
			for (int i = 0; i < line.length(); ++i) {
				canvases_.back().first.back().push_back(line[i] - 48.5);
			}
		}
	}

	reader.close();
	if (loadCounter == 0) std::cout << filepath << " is empty.\n";
	else if (loadCounter == 1) std::cout << "1 canvas from " << filepath << " has been loaded into memory.\n";
	else std::cout << loadCounter << " canvases from " << filepath << " have been loaded into memory.\n";
	std::cout << "Total canvases: " << canvases_.size() << std::endl;
}
