#include "text.h"

Text::Text() :
	texture_(NULL),
	width_(0),
	height_(0)
{

}


Text::~Text() {
	clearTexture();
}

bool Text::update(Graphics &graphics, std::string text, SDL_Color textColor) {
	

	//Get rid of preexisting texture
	clearTexture();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid(font_, text.c_str(), textColor);
	if (textSurface != NULL)
	{
		//Create texture from surface pixels
		texture_ = SDL_CreateTextureFromSurface(graphics.getRenderer(), textSurface);
		if (texture_ == NULL)
		{
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			//Get image dimensions
			width_ = textSurface->w;
			height_ = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface(textSurface);
	}
	else
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}


	//Return success
	return texture_ != NULL;
}

void Text::setColor(Uint8 r, Uint8 g, Uint8 b) {
	SDL_SetTextureColorMod(texture_, r, g, b);

}

void Text::setAlpha(Uint8 a) {
	SDL_SetTextureAlphaMod(texture_, a);
}

#ifdef _SDL_TTF_H
void Text::draw(Graphics &graphics, int x, int y, double angle, SDL_RendererFlip flip, SDL_Rect * clip, SDL_Point * center) {
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, width_, height_ };
	//Set clip rendering dimensions
	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx(graphics.getRenderer(), texture_, clip, &renderQuad, angle, center, flip);
	TTF_CloseFont(font_);
}
#endif

void Text::clearTexture() {
	if (texture_ != NULL)
	{
		SDL_DestroyTexture(texture_);
		texture_ = NULL;
		width_ = 0;
		height_ = 0;
	}
}

void Text::loadFont(const std::string &filePath, int fontSize) {
	font_ = TTF_OpenFont(filePath.c_str(), fontSize);
}

int Text::getWidth() const {
	return width_;
}

int Text::getHeight() const {
	return height_;
}

std::string Text::getText() const {
	return text_;
}
