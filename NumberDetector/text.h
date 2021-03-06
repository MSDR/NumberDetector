#ifndef TEXT_H_
#define TEXT_H_

#include "graphics.h"

class Text {
public:
	Text();
	~Text();

	void setColor(Uint8 red, Uint8 green, Uint8 blue);

	void setAlpha(Uint8 alpha);

	bool update(Graphics &graphics, std::string text, SDL_Color textColor = { 0, 0, 0, 255 });

//#ifdef _SDL_TTF_H		
	void draw(Graphics &graphics, int x, int y, double angle = 0.0, SDL_RendererFlip flip = SDL_FLIP_NONE, SDL_Rect* clip = NULL, SDL_Point* center = NULL);
//#endif

	void clearTexture();

	void loadFont(const std::string &filePath, int fontSize);

	int getWidth() const;
	int getHeight() const;
	std::string getText() const;

protected:
	SDL_Texture* texture_;
	TTF_Font *font_ = NULL;

	std::string text_;

	int width_;
	int height_;
	
};

#endif // !TEXT_H_
