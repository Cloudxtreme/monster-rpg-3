#include "Nooskewl_Engine/engine.h"

#include "ml_start_upper.h"

void ML_start_upper::start()
{
	noo.play_music("town.mml");
}

void ML_start_upper::trigger(Map_Entity *entity)
{
	Point<int> collide_pos;
	if (entity->get_id() == 0 && entity->tiles_collide(Point<int>(22, 3), Size<int>(2, 2), collide_pos)) {
		entity->stop();
		noo.map->change_map("start.map", Point<int>(30, 2), S);
	}
}