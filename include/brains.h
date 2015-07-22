#ifndef BRAINS_H
#define BRAINS_H

#include <Nooskewl_Engine/main.h>
#include <Nooskewl_Engine/talk_brain.h>

using namespace Nooskewl_Engine;

class Animated_Brain : public Talk_Brain {
public:
	Animated_Brain(std::string name);

	void update();
	bool save(SDL_RWops *file);
};

#endif // BRAINS_H