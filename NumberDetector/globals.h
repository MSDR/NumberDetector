//Globals class adapted from LimeOats: https://limeoats.com/, modified and commented by Mason Sklar [2020]

#ifndef  GLOBALS_H_
#define GLOBALS_H_

namespace globals {
	const int SCREEN_WIDTH = 32;
	const int SCREEN_HEIGHT = 42;

	const int SPRITE_SCALE = 15;
}

namespace sides {
	enum Side {
		TOP,
		BOTTOM,
		LEFT,
		RIGHT,
		NONE
	};

	inline Side getOppositeSide(Side side) {
		return
			side == TOP ? BOTTOM :
			side == BOTTOM ? TOP :
			side == LEFT ? RIGHT :
			side == RIGHT ? LEFT :
			NONE;
	}
}

enum Direction {
	LEFT,
	RIGHT,
	UP,
	DOWN
};

struct Vector2 {
	int x, y;
	Vector2() : x(0), y(0) {}
	Vector2(int x, int y) : x(x), y(y) {}

	Vector2 zero() {
		return Vector2(0, 0);
	}

	//Clamps x and y between bounds, where bounds[X/Y].x value is the lower bound and bounds[X/Y]y is the upper bound
	void clampValues(Vector2 boundsX, Vector2 boundsY) {
		x = x < boundsX.x ? boundsX.x : x;
		x = x > boundsX.y ? boundsX.y : x;
		y = y < boundsY.x ? boundsY.x : y;
		y = y > boundsY.y ? boundsY.y : y;
	}
};

#endif GLOBALS_H_