#ifndef ML_START_H
#define ML_START_H

#include <Nooskewl_Engine/map.h>
#include <Nooskewl_Engine/map_logic.h>
#include <Nooskewl_Engine/tween.h>
#include <Nooskewl_Engine/widgets.h>

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

	bool sat;
	MO3_Text_Button *yes, *pass;
	Map_Entity *sunshine;
	Map_Entity *business_man;
	Map_Entity *legendary_warrior;
	Map_Entity *bartender;
	Map_Entity *sitting_lady;
};

#endif // ML_START_H
