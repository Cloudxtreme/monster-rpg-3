#ifndef MONSTER_RPG_3_H
#define MONSTER_RPG_3_H

#define DEC_HUNGER(stats, amount) \
	stats->hunger -= MIN(amount, stats->hunger);
#define DEC_THIRST(stats, amount) \
	stats->thirst -= MIN(amount, stats->thirst);
#define DEC_SOBRIETY(stats, amount) { \
		int amt = MIN(amount, stats->sobriety); \
		if (stats->sobriety > 0 && stats->sobriety - amt <= 0xffff / 100) { \
			stats->set_status(Stats::DRUNK); \
		} \
		stats->sobriety -= amt; \
	}
#define DEC_KARMA(stats, amount) \
	stats->karma -= MIN(amount, stats->karma); \
	noo.add_notification(noo.game_t->translate(150));

#include <Nooskewl_Engine/Nooskewl_Engine.h>

using namespace Nooskewl_Engine;

bool give_item(Map_Entity *activator, std::string item_name, int quantity, int milestone);
Direction get_facing_direction(Map_Entity *activator, Map_Entity *activated);
std::string get_time_string(int seconds);

#endif
