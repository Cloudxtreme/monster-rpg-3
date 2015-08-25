#include <Nooskewl_Engine/Nooskewl_Engine.h>

#include "ml.h"
#include "ml_town.h"
#include "tweens.h"

ML_town::ML_town()
{
}

void ML_town::start(bool been_here_before)
{
	noo.play_music("town.mml");

	if (!been_here_before) {
	}
}

void ML_town::end()
{
}

void ML_town::trigger(Map_Entity *entity)
{
	Point<int> collide_pos;
	if (entity->get_id() == 0 && entity->tiles_collide(Point<int>(38, 10), Size<int>(2, 1), collide_pos)) {
		entity->stop();
		noo.map->change_map("start.map", Point<int>(11, 24), N);
	}
}

void ML_town::update()
{
}

void ML_town::activate(Map_Entity *activator, Map_Entity *activated)
{
}
