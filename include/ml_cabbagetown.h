#ifndef ML_TOWN_H
#define ML_TOWN_H

#include <Nooskewl_Engine/Nooskewl_Engine.h>

using namespace Nooskewl_Engine;

class ML_cabbagetown : public Map_Logic {
public:
	static void callback(void *data);

	ML_cabbagetown();

	void start(bool been_here_before);
	void end();
	void trigger(Map_Entity *entity);
	void update();
	void activate(Map_Entity *activator, Map_Entity *activated);

private:
};

#endif // ML_TOWN_H
