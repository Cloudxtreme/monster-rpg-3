#ifndef ML_START_UPPER_H
#define ML_START_UPPER_H

#include <Nooskewl_Engine/Nooskewl_Engine.h>

using namespace Nooskewl_Engine;

class ML_start_upper : public Map_Logic {
public:
	ML_start_upper(int last_visited_time);

	void start_audio();
	void start(bool been_here_before);
	void trigger(Map_Entity *entity);
};

#endif // ML_START_UPPER_H
