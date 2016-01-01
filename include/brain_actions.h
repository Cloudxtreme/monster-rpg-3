#ifndef BRAIN_ACTIONS_H
#define BRAIN_ACTIONS_H

#include <Nooskewl_Engine/Nooskewl_Engine.h>

using namespace Nooskewl_Engine;

class Pick_Pocketable_Brain {
public:
	Pick_Pocketable_Brain() : can_pick_pocket(true) {}

	virtual void pick_pocket(Map_Entity *pocket_picker, Map_Entity *pocket_pickee);
	bool can_pick_pocket;
};

#endif // BRAIN_ACTIONS_H
