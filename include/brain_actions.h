#ifndef BRAIN_ACTIONS_H
#define BRAIN_ACTIONS_H

#include <Nooskewl_Engine/Nooskewl_Engine.h>

using namespace Nooskewl_Engine;

class Pick_Pocketable_Brain {
public:
	virtual void pick_pocket(Map_Entity *pocket_picker, Map_Entity *pocket_pickee);
	virtual void pick_pocket_failure_reaction() {}

	virtual ~Pick_Pocketable_Brain() {}
};

class Bartender_Pick_Pocketable_Brain : public Pick_Pocketable_Brain {
public:
	void pick_pocket_failure_reaction();

	virtual ~Bartender_Pick_Pocketable_Brain() {}
};

#endif // BRAIN_ACTIONS_H
