#ifndef BRAIN_ACTIONS_H
#define BRAIN_ACTIONS_H

#include <Nooskewl_Engine/Nooskewl_Engine.h>

using namespace Nooskewl_Engine;

class Pick_Pocketable_Brain {
public:
	virtual void pick_pocket(Map_Entity *pocket_picker, Map_Entity *pocket_pickee);
};

#endif // BRAIN_ACTIONS_H
