#ifndef ML_START_UPPER_H
#define ML_START_UPPER_H

#include <Nooskewl_Engine/map.h>
#include <Nooskewl_Engine/map_logic.h>
#include <Nooskewl_Engine/tween.h>
#include <Nooskewl_Engine/widgets.h>

using namespace Nooskewl_Engine;

class ML_start_upper : public Map_Logic {
public:
	void start();
	void trigger(Map_Entity *entity);
};

#endif // ML_START_UPPER_H
