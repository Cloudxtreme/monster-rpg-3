#include "ml_test.h"
#include "tweens.h"

void ML_Test::start(Map *map)
{
	map->add_speech("Hello there. I'm just wondering, what's the price on the beeves?");
}

void ML_Test::trigger(Map *map, Map_Entity *entity)
{
	Point<int> collide_pos;
	if (entity->get_id() == 0 && entity->tiles_collide(Point<int>(7, 1), Size<int>(2, 1), collide_pos)) {
		entity->stop();
		map->change_map("test2.map", Point<int>(7, 18) + collide_pos, N);
	}
}
