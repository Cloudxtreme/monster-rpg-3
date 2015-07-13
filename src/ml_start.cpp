#include "ml_start.h"
#include "tweens.h"

ML_start::ML_start() :
	sat(false)
{
}

void ML_start::start(Map *map)
{
}

void ML_start::end(Map *map)
{
}

void ML_start::trigger(Map *map, Map_Entity *entity)
{
	if (sat == false) {
		Point<int> collide_pos;
		if (entity->get_id() == 0 && entity->tiles_collide(Point<int>(14, 14), Size<int>(1, 1), collide_pos)) {
			entity->stop();
			entity->set_sitting(true);
			sat = true;
		}
	}
}

void ML_start::update(Map *map)
{
}

void ML_start::activate(Map *map, Map_Entity *activator, Map_Entity *activated)
{
}
