//Rectangle class adapted from LimeOats: https://limeoats.com/, modified and commented by Mason Sklar [2020]

#ifndef RECTANGLE_H_
#define RECTANGLE_H_

#include "globals.h"

class Rectangle {
public:
	Rectangle() {};
	Rectangle(float x, float y, int w, int h) :
		x_(x),
		y_(y),
		w_(w),
		h_(h)
	{}

	const float getCenterX() const { return x_ + w_ / 2; }
	const float getCenterY() const { return y_ + h_ / 2; }

	const float getLeft() const { return x_; }
	const float getRight() const { return x_ + w_; }
	const float getTop() const { return y_; }
	const float getBottom() const { return y_ + h_; }

	const int getWidth() const { return w_; }
	const int getHeight() const { return h_; }

	const int getSide(const sides::Side side) const {
		return
			side == sides::TOP ? getTop() :
			side == sides::BOTTOM ? getBottom() :
			side == sides::LEFT ? getLeft() :
			side == sides::RIGHT ? getRight() :
			sides::NONE;
	}

	const bool collidesWith(const Rectangle &other) const {
		return
			getRight() >= other.getLeft() &&
			getLeft() <= other.getRight() &&
			getTop() <= other.getBottom() &&
			getBottom() >= other.getTop();
	}

	const bool isValidRect() const {
		return (x_ >= 0 && y_ >= 0 && w_ >= 0 && h_ >= 0);
	}

	float x_;
	float y_;
	int w_;
	int h_;
};

#endif // !RECTANGLE_H_