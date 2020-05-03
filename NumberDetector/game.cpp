#include "game.h"


Game::Game() {
	SDL_Init(SDL_INIT_EVERYTHING);
	//IMG_Init(IMG_INIT_PNG);
	if (TTF_Init() == -1) {
		std::cout << TTF_GetError() << std::endl;
		SDL_Quit();
	}

	gameLoop();
}

Game::~Game() {
	TTF_Quit();
	//IMG_Quit();
	SDL_Quit();
}

void Game::gameLoop() {
	//Initialize members
	Graphics graphics;
	Input input;
	SDL_Event event;

	//brushOutline_ = new Sprite(graphics, "Images/BrushOutline.png", 0, 0, 92, 92); 

	canvasNum_ = std::rand() % 10;

	numRequestLine_ = new Text();
	numRequestLine_->loadFont("Assets/Pixel NES.ttf", 1.5*globals::SPRITE_SCALE);
	numRequestLine_->update(graphics, "Draw a " + std::to_string(canvasNum_), { 0, 0, 0, 255 });
	std::cout << canvasNum_;
	int LAST_UPDATE_TIME = SDL_GetTicks();
	canvas_ = new int*[28];
	for (int i = 0; i < 28; ++i) {
		canvas_[i] = new int[28];
		for (int j = 0; j < 28; ++j) {
			canvas_[i][j] = 0;
		}
	}
	
	collectingData_ = true;

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
			int mouseX = 0;
			int mouseY = 0;
			SDL_GetMouseState(&mouseX, &mouseY);
			int cx = (mouseX - mouseX % globals::SPRITE_SCALE)/globals::SPRITE_SCALE - 3;
			int cy = (mouseY - mouseY % globals::SPRITE_SCALE) / globals::SPRITE_SCALE - 3;
			if(cx < 25 && cy < 25 && cx >= 0 && cy >= 0) {
				for (int i = cy; i < cy + 3; ++i) {
					for (int j = cx; j < cx + 3; ++j) {
						canvas_[i][j] = 1;
					}
				}
			}
		}
		
		//Clear canvas
		if (input.wasKeyPressed(SDL_SCANCODE_Z) || input.wasKeyPressed(SDL_SCANCODE_BACKSPACE)) {
			for (int i = 0; i < 28; ++i) {
				for (int j = 0; j < 28; ++j) {
					canvas_[i][j] = 0;
				}
			}
		}

		//Submit a canvas
		if (input.wasKeyPressed(SDL_SCANCODE_E) || input.wasKeyPressed(SDL_SCANCODE_RETURN)) {
			int** c = new int*[28];
			for (int i = 0; i < 28; ++i) {
				c[i] = new int[28];
				for (int j = 0; j < 28; ++j) {
					c[i][j] = canvas_[i][j];
					canvas_[i][j] = 0;
				}
			}
			canvases_.push_back(c);

			canvasNum_ = std::rand() % 10;
			numRequestLine_->update(graphics, "Draw a " + std::to_string(canvasNum_), { 0, 0, 0, 255 });

		}

		//Print canvases_ to the console
		if (input.wasKeyPressed(SDL_SCANCODE_S)) {
			for(int v = 0; v < canvases_.size(); ++v){
				std::cout << std::endl << std::endl;
				for (int i = 0; i < 28; ++i) {
					std::cout << std::endl;
					for (int j = 0; j < 28; ++j) {
						std::cout << canvases_[v][i][j] << " ";
					}
				}
			}
			std::cout << "\n________________________________________________________\n";
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

void Game::draw(Graphics &graphics) {
	graphics.clear();
	drawBackground(graphics.getRenderer());
	drawCanvas(graphics.getRenderer());
	int mouseX = 0;
	int mouseY = 0;
	SDL_GetMouseState(&mouseX, &mouseY);
	//std::cout << mouseX-mouseX%globals::SPRITE_SCALE << " " << mouseY-mouseY%globals::SPRITE_SCALE << std::endl;

	//Draw brush outline
	SDL_Rect brushOutline; 
	{  brushOutline.x = (mouseX - mouseX % globals::SPRITE_SCALE) - globals::SPRITE_SCALE;
		brushOutline.y = (mouseY - mouseY % globals::SPRITE_SCALE) - globals::SPRITE_SCALE;
		brushOutline.w = 3 * globals::SPRITE_SCALE; 
		brushOutline.h = 3 * globals::SPRITE_SCALE;  }
	SDL_SetRenderDrawColor(graphics.getRenderer(), 238, 37, 37, 120);
	SDL_RenderDrawRect(graphics.getRenderer(), &brushOutline);
	SDL_SetRenderDrawColor(graphics.getRenderer(), 255, 255, 255, 255);

	//brushOutline_->draw(graphics, mouseX-mouseX%globals::SPRITE_SCALE, mouseY-mouseY%globals::SPRITE_SCALE);
	
	//Draw text to request a number
	numRequestLine_->draw(graphics, 2 * globals::SPRITE_SCALE, 30 * globals::SPRITE_SCALE);

	graphics.flip();
}

void Game::drawBackground(SDL_Renderer* renderer) {
	//Solid white background
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(renderer, NULL);

	//Drawing space outline
	SDL_Rect outline; outline.x = 2*globals::SPRITE_SCALE-1; outline.y = 2*globals::SPRITE_SCALE-1; 
							outline.w = 28*globals::SPRITE_SCALE+2; outline.h = 28*globals::SPRITE_SCALE+2;
	SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
	SDL_RenderDrawRect(renderer, &outline);
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
			if (canvas_[i][j] > 0) {
				r.x = 2 * globals::SPRITE_SCALE + j * globals::SPRITE_SCALE;
				r.y = 2 * globals::SPRITE_SCALE + i * globals::SPRITE_SCALE;
				SDL_RenderFillRect(renderer, &r);
			}
		}
	}
}
void Game::update(float elapsedTime) {
}