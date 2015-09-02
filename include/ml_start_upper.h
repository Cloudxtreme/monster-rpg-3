#ifndef ML_START_UPPER_H
#define ML_START_UPPER_H

#include <Nooskewl_Engine/Nooskewl_Engine.h>

using namespace Nooskewl_Engine;

class ML_start_upper : public Map_Logic {
public:
	void start(bool been_here_before, int last_visited_time);
	void trigger(Map_Entity *entity);
};

#endif // ML_START_UPPER_H
