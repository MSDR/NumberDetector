//Globals class adapted from LimeOats: https://limeoats.com/, modified and commented by Mason Sklar [2020]

#ifndef  GLOBALS_H_
#define GLOBALS_H_

namespace globals {
	const int SCREEN_WIDTH = 280;
	const int SCREEN_HEIGHT = 380;

	const int SPRITE_SCALE = 10;
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
};

#endif GLOBALS_H_