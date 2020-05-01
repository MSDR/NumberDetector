//Graphics class adapted from LimeOats: https://limeoats.com/, modified and commented by Mason Sklar [2020]

#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include <iostream>
#include <map>
#include "SDL_include/SDL.h"
#include "SDL_include/SDL_image.h"
#include "SDL_include/SDL_ttf.h"
#include <string>

#include "globals.h"

class Graphics {
public:
	Graphics();
	~Graphics();

	//Loads an image into the spriteSheets_ map if it doesn't already exist
	//Returns the image frm the map regardless of whether it was just loaded
	SDL_Surface* loadImage(const std::string &filePath);

	//Draws a texture to destinationRectangle from sourceRectangle on the spritesheet
	void blitSurface(SDL_Texture* texture, SDL_Rect* sourceRectangle, SDL_Rect* destinationRectangle);

	//Renders everything to the screen
	void flip();

	//Clears the screen
	void clear();

	//Returns the renderer_
	SDL_Renderer* getRenderer() const;

private:
	SDL_Window* window_;
	SDL_Renderer* renderer_;

	std::map<std::string, SDL_Surface*> spriteSheets_;
};

#endif GRAPHICS_H_