#ifndef ML_START_H
#define ML_START_H

#include <Nooskewl_Engine/Nooskewl_Engine.h>

using namespace Nooskewl_Engine;

class ML_start : public Map_Logic {
public:
	static void callback(void *data);

	ML_start();

	void start();
	void end();
	void trigger(Map_Entity *entity);
	void update();
	void activate(Map_Entity *activator, Map_Entity *activated);

private:
	static int callback_data;
	static Map_Entity *coro;

	Map_Entity *sunshine;
	Map_Entity *business_man;
	Map_Entity *legendary_warrior;
	Map_Entity *bartender;
	Map_Entity *sitting_lady;
	Map_Entity *laughing_man;
	Map_Entity *drinker;
};

#endif // ML_START_H
