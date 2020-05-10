#include "game.h"


Game::Game() {
	SDL_Init(SDL_INIT_EVERYTHING);
	//IMG_Init(IMG_INIT_PNG);
	if (TTF_Init() == -1) {
		std::cout << TTF_GetError() << std::endl;
		SDL_Quit();
	}

	srand(time(0)); 

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

	emptyCanvas_ = true;
	//Set up number request text
	collectingData_ = true;
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
				}
			} else if (event.type = SDL_KEYUP) {
				input.keyUpEvent(event);
			} else if (event.type == SDL_QUIT) {
				return;
			}
		}

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
			canvases_.push_back(c);

			//Reset number request
			canvasNum_ = 1;// std::rand() % 10; CHANGE THIS MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMm
			numRequestLine_->update(graphics, "Draw a " + std::to_string(canvasNum_), { 0, 0, 0, 255 });
		}

		//Print canvases_ to the console
		if (input.wasKeyPressed(SDL_SCANCODE_S)) {
			std::cout << "\n __________________________________________________________\n";
			for(int v = 0; v < canvases_.size(); ++v){
				for (int i = -1; i < 28; ++i) {
					std::cout << "| ";
					for (int j = 0; j < 28; ++j) {
						std::cout << (i == -1 || canvases_[v][i][j] == -0.5  ? "  " : "[]") << (j == 27 ? "" : ""); //should only be -0.5 exactly if manually set
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
		}

		//Exit the program
		if (input.wasKeyPressed(SDL_SCANCODE_DELETE))
			return;

		const int CURRENT_TIME_MILLIS = SDL_GetTicks();
		int ELAPSED_TIME_MILLIS = CURRENT_TIME_MILLIS - LAST_UPDATE_TIME;
		update(ELAPSED_TIME_MILLIS);
		LAST_UPDATE_TIME = CURRENT_TIME_MILLIS;
		draw(graphics);
	}//end game loop
}

void Game::cnnPass() {
	//Forward pass
	std::vector<double> result = cnn_.forward(canvas_);

	//Print results
	int guess = 0;
	for (int i = 0; i < result.size(); ++i) {
		guess = result[i] > result[guess] ? i : guess;
		std::cout << std::endl << i << " | Confidence: " << result[i];
	}
	std::cout << "\nGuess: " << guess << "\nLoss: " << -std::log(result[canvasNum_]) << std::endl;

	//Calculate gradient for backpropagation
	std::vector<double> gradient(10, 0);
	gradient[canvasNum_] = -(1 / result[canvasNum_]);

	//Backwards pass (adjusting weights)
	cnn_.backProp(gradient, 0.005);
}

void Game::draw(Graphics &graphics) {
	graphics.clear();
	drawBackground(graphics.getRenderer());
	drawCanvas(graphics.getRenderer());
	int mouseX, mouseY = 0;
	SDL_GetMouseState(&mouseX, &mouseY);
	drawBrushOutline(graphics.getRenderer(), mouseX, mouseY);
	//std::cout << mouseX-mouseX%globals::SPRITE_SCALE << " " << mouseY-mouseY%globals::SPRITE_SCALE << std::endl;


	//brushOutline_->draw(graphics, mouseX-mouseX%globals::SPRITE_SCALE, mouseY-mouseY%globals::SPRITE_SCALE);
	
	//Draw text to request a number
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