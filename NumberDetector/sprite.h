//Sprite class adapted from LimeOats: https://limeoats.com/, modified and commented by Mason Sklar [2020]

#ifndef SPRITE_H_
#define SPRITE_H_

#include "graphics.h"
#include "rectangle.h"

class Sprite {
public:
	Sprite();
	Sprite(Graphics &graphics, const std::string &filePath, int sourceX, int sourceY, int width, int height, float posX = 0.0, float posY = 0.0);
	virtual ~Sprite();

	//Updates the Sprite and its members
	virtual void update(float elapsedTime);

	//Draws the Sprite onto the screen
	void draw(Graphics &graphics, int x, int y);

	const Rectangle getBoundingBox() const;
	const float getWidth() const;
	const float getHeight() const;
	const float getX() const;
	const float getY() const;

	//Updates the BoundingBox_ of the Sprite
	void updateBoundingBox();

	//Given another Rectangle, returns the side of collision
	const sides::Side getCollisionSide(Rectangle &other) const;

protected:
	SDL_Rect sourceRect_;
	SDL_Texture* spriteSheet_;
	float x_;
	float y_;

	Rectangle boundingBox_;
};

#endif SPRITE_H_