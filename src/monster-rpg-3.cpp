#include <Nooskewl_Engine/Nooskewl_Engine.h>

#include "monster-rpg-3.h"

bool give_item(Map_Entity *activator, std::string item_name, int quantity, int milestone)
{
	Stats *stats = activator->get_stats();

	if (stats) {
		Inventory *inventory = stats->inventory;
		if (inventory) {
			if (quantity > 0) {
				std::string name;
				for (int i = 0; i < quantity; i++) {
					Item *item = new Item(item_name);
					inventory->add(item);
					if (i == 0) {
						name = item->name;
					}
				}
				noo.item_mml->play(false);
				// FIXME: a/an and adding s isn't foolproof
				std::string found = noo.game_t->translate(72);
				if (quantity > 1) {
					noo.map->add_speech(found + " " + itos(quantity) + " " + name + "s");
				}
				else {
					char c = tolower(name.c_str()[0]);
					if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
						noo.map->add_speech(found + " " + noo.game_t->translate(47) + " " + name + "...");
					}
					else {
						noo.map->add_speech(found + " " + TRANSLATE("a") + " " + name + "...");
					}
				}

				if (milestone >= 0) {
					noo.set_milestone(milestone, true);
				}
			}

			return true;
		}
	}

	return false;
}

Direction get_facing_direction(Map_Entity *activator, Map_Entity *activated)
{
	Point<int> offset = activator->get_position() - activated->get_position();
	if (offset.x < 0) {
		return W;
	}
	else if (offset.x > 0) {
		return E;
	}
	else if (offset.y < 0) {
		return N;
	}
	else {
		return S;
	}
}
