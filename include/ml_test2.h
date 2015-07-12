#ifndef ML_TEST2_H
#define ML_TEST2_H

#include "Nooskewl_Engine/map.h"
#include "Nooskewl_Engine/map_logic.h"
#include "Nooskewl_Engine/tween.h"

using namespace Nooskewl_Engine;

class ML_Test2 : public Map_Logic {
public:
	void start(Map *map);
	void end(Map *map);
	void trigger(Map *map, Map_Entity *entity);
	void update(Map *map);
	void activate(Map *map, Map_Entity *activator, Map_Entity *activated);

private:
	Tweens tweens;
	Map_Entity *test;
};

#endif // ML_TEST2_H
