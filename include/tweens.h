#ifndef TWEENS_H
#define TWEENS_H

#include <Nooskewl_Engine/Nooskewl_Engine.h>

using namespace Nooskewl_Engine;

class Move_Tween : public Tween {
public:
	Map_Entity *entity;
	Point<int> start_pos;
	Point<int> destination;
	Uint32 start_time;
	float speed; // pixels/second

	bool update(Tween *tween);
};

#endif // TWEENS_H
