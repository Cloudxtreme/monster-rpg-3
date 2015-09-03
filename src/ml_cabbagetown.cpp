#include <Nooskewl_Engine/Nooskewl_Engine.h>

#include "brains.h"
#include "ml.h"
#include "ml_cabbagetown.h"
#include "tweens.h"

const int num_cabbages = 68;

static void add_cabbage(int x, int y, int chance)
{
	if (noo.map->is_solid(-1, 0, Point<int>(x, y), Size<int>(1, 1), true, true)) {
		return;
	}

	if (rand() % 100 > chance) {
		return;
	}

	Map_Entity *cabbage = new Map_Entity("cabbage");
	int time = noo.get_play_time() - rand() % ((5 * 60) * 4);
	cabbage->set_brain(new Growing_Brain("baby_cabbage", "cabbage", "rotten_cabbage", time));
	cabbage->load_sprite("cabbage");
	cabbage->set_position(Point<int>(x, y));
	noo.map->add_entity(cabbage);
}

ML_cabbagetown::ML_cabbagetown(int last_visited_time)
{
	int now = noo.get_play_time();
	int diff = now - last_visited_time;
	cabbages_to_regrow = int((diff / 1800.0f) * (num_cabbages/2));
}

void ML_cabbagetown::start(bool been_here_before)
{
	noo.play_music("cabbagetown.mml");

	int chance = been_here_before ? cabbages_to_regrow * 100 / num_cabbages : 95;

	for (int y = 17; y <= 17; y++) {
		for (int x = 27; x <= 32; x++) {
			add_cabbage(x, y, chance);
		}
	}
	for (int y = 13; y <= 16; y++) {
		for (int x = 32; x <= 32; x++) {
			add_cabbage(x, y, chance);
		}
	}
	for (int y = 13; y <= 17; y++) {
		for (int x = 36; x <= 36; x++) {
			add_cabbage(x, y, chance);
		}
	}
	for (int y = 13; y <= 17; y++) {
		for (int x = 41; x <= 41; x++) {
			add_cabbage(x, y, chance);
		}
	}
	for (int y = 17; y <= 17; y++) {
		for (int x = 37; x <= 40; x++) {
			add_cabbage(x, y, chance);
		}
	}
	for (int y = 17; y <= 17; y++) {
		for (int x = 46; x <= 50; x++) {
			add_cabbage(x, y, chance);
		}
	}
	for (int y = 13; y <= 17; y++) {
		for (int x = 45; x <= 45; x++) {
			add_cabbage(x, y, chance);
		}
	}
	for (int y = 26; y <= 26; y++) {
		for (int x = 27; x <= 32; x++) {
			add_cabbage(x, y, chance);
		}
	}
	for (int y = 27; y <= 30; y++) {
		for (int x = 32; x <= 32; x++) {
			add_cabbage(x, y, chance);
		}
	}
	for (int y = 26; y <= 30; y++) {
		for (int x = 36; x <= 36; x++) {
			add_cabbage(x, y, chance);
		}
	}
	for (int y = 26; y <= 30; y++) {
		for (int x = 41; x <= 41; x++) {
			add_cabbage(x, y, chance);
		}
	}
	for (int y = 26; y <= 26; y++) {
		for (int x = 37; x <= 40; x++) {
			add_cabbage(x, y, chance);
		}
	}
	for (int y = 26; y <= 30; y++) {
		for (int x = 45; x <= 45; x++) {
			add_cabbage(x, y, chance);
		}
	}
	for (int y = 26; y <= 26; y++) {
		for (int x = 46; x <= 50; x++) {
			add_cabbage(x, y, chance);
		}
	}
}

void ML_cabbagetown::end()
{
}

void ML_cabbagetown::trigger(Map_Entity *entity)
{
	Point<int> collide_pos;
	if (entity->get_id() == 0 && entity->tiles_collide(Point<int>(38, 10), Size<int>(2, 1), collide_pos)) {
		entity->stop();
		noo.map->change_map("start.map", Point<int>(11, 24), N);
	}
}

void ML_cabbagetown::update()
{
}

void ML_cabbagetown::activate(Map_Entity *activator, Map_Entity *activated)
{
}
