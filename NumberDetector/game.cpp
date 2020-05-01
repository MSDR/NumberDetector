#include "game.h"

namespace {
	const int FPS = 60;
	const int MAX_FRAME_TIME = 75;
}

namespace input_keys {
	SDL_Scancode key_name = SDL_SCANCODE_0;
}

Game::Game() {
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG);

	gameLoop();

	IMG_Quit();
	SDL_Quit();
}

Game::~Game() {

}

void Game::gameLoop() {
	Graphics graphics;
	Input input;
	SDL_Event event;


	/*sprite_ = Sprite(Graphics &graphics, const std::string &filePath, int sourceX, int sourceY, 
	int width, int height, float posX, float posY); */

	int LAST_UPDATE_TIME = SDL_GetTicks();

	//Start game loop
	while (true) {
		input.beginNewFrame();

		if (SDL_PollEvent(&event)) {
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

		if (input.wasKeyPressed(SDL_SCANCODE_DELETE))
			return;

		const int CURRENT_TIME_MILLIS = SDL_GetTicks();
		int ELAPSED_TIME_MILLIS = CURRENT_TIME_MILLIS - LAST_UPDATE_TIME;
		update(std::min(ELAPSED_TIME_MILLIS, MAX_FRAME_TIME));
		LAST_UPDATE_TIME = CURRENT_TIME_MILLIS;
		draw(graphics);
	}//end game loop
}

void Game::draw(Graphics &graphics) {
	graphics.clear();
	sprite_.draw(graphics, sprite_.getX(), sprite_.getY());
	graphics.flip();
}

void Game::update(float elapsedTime) {
	sprite_.update(elapsedTime);
}