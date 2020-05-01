//Graphics class adapted from LimeOats: https://limeoats.com/, modified and commented by Mason Sklar [2020]

#include "graphics.h"

Graphics::Graphics() {
	SDL_CreateWindowAndRenderer(globals::SCREEN_WIDTH*globals::SPRITE_SCALE, globals::SCREEN_HEIGHT*globals::SPRITE_SCALE,
		0, &window_, &renderer_);
	SDL_SetWindowTitle(window_, "NumberDetector");
}

Graphics::~Graphics() {
	SDL_DestroyWindow(window_);
	SDL_DestroyRenderer(renderer_);
}

SDL_Surface* Graphics::loadImage(const std::string &filePath) {
	if (spriteSheets_.count(filePath) == 0) {
		spriteSheets_[filePath] = IMG_Load(filePath.c_str());
		std::cout << IMG_GetError() << std::endl;
	}

	return spriteSheets_[filePath];
}

void Graphics::blitSurface(SDL_Texture* texture, SDL_Rect* sourceRectangle, SDL_Rect* destinationRectangle) {
	SDL_RenderCopy(renderer_, texture, sourceRectangle, destinationRectangle);
}

void Graphics::flip() {
	SDL_RenderPresent(renderer_);
}

void Graphics::clear() {
	SDL_RenderClear(renderer_);
}

SDL_Renderer* Graphics::getRenderer() const {
	return renderer_;
}