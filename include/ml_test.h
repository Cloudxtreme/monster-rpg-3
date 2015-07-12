#ifndef ML_TEST_H
#define ML_TEST_H

#include "Nooskewl_Engine/map.h"
#include "Nooskewl_Engine/map_logic.h"

using namespace Nooskewl_Engine;

class ML_Test : public Map_Logic {
public:
	void start(Map *map);
	void trigger(Map *map, Map_Entity *entity);
};

#endif // ML_TEST_H
