#ifndef MONSTER_RPG_3_H
#define MONSTER_RPG_3_H

#define DEC_HUNGER(amount) \
	stats->hunger -= MIN(amount, stats->hunger);
#define DEC_THIRST(amount) \
	stats->thirst -= MIN(amount, stats->thirst);
#define DEC_SOBRIETY(amount) { \
		int amt = MIN(amount, stats->sobriety); \
		if (stats->sobriety > 0 && stats->sobriety - amt <= 0xffff / 100) { \
			stats->set_status(Stats::DRUNK); \
		} \
		stats->sobriety -= amt; \
	}
#define DEC_KARMA(amount) \
	stats->karma -= MIN(amount, stats->karma); \
	noo.add_notification(TRANSLATE("You lost karma...")END);

#include <Nooskewl_Engine/Nooskewl_Engine.h>

using namespace Nooskewl_Engine;

bool give_item(Map_Entity *activator, std::string item_name, int quantity, int milestone);
Direction get_facing_direction(Map_Entity *activator, Map_Entity *activated);

#endif
