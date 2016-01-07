#include "brain_actions.h"
#include "brains.h"
#include "monster-rpg-3.h"

void Pick_Pocketable_Brain::pick_pocket(Map_Entity *pocket_picker, Map_Entity *pocket_pickee)
{
	Monster_RPG_3_Brain *pickee_brain = dynamic_cast<Monster_RPG_3_Brain *>(pocket_pickee->get_brain());

	if (pickee_brain->can_pick_pocket == false) {
		noo.add_notification(TRANSLATE("Cannot pick this pocket")END);
		return;
	}

	Stats *picker_stats = pocket_picker->get_stats();
	Stats *pickee_stats = pocket_pickee->get_stats();
	if (picker_stats == 0 || pickee_stats == 0) {
		// no stats, can't do it
		return;
	}
	float picker_agility = (float)picker_stats->characteristics.agility;
	float pickee_agility = (float)pickee_stats->characteristics.agility;
	if (pickee_agility == 0.0f) {
		// can't divide by zero
		return;
	}
	float ratio = picker_agility / pickee_agility;
	if (ratio <= 1.0f) {
		pickee_brain->can_pick_pocket = false;
		noo.add_notification(TRANSLATE("Failed to pick pocket")END);
		if (pickee_brain->caught_pick_pocket_text != "") {
			noo.map->add_speech(pickee_brain->caught_pick_pocket_text);
		}
		pick_pocket_failure_reaction();
		return;
	}
	ratio -= 1.0f;
	// max 75% chance to pick a pocket
	if (ratio > 0.75f) {
		ratio = 0.75f;
	}
	int r = rand() % 1000; // FIXME: mersenne twister?
	if (r < int(1000.0f * ratio)) {
		int gold = pickee_stats->inventory->gold;
		if (gold > 0) {
			int r = int(((rand() % gold) + 1) * ratio);
			if (r == 0) {
				r = 1;
			}
			picker_stats->inventory->gold += r;
			pickee_stats->inventory->gold -= r;
			noo.add_notification(TRANSLATE("Got")END + " " + itos(r) + " " + TRANSLATE("gold")END);
			DEC_KARMA(picker_stats, 500);
		}
		else {
			noo.add_notification(TRANSLATE("Got nothing...")END);
		}
	}
	else {
		pickee_brain->can_pick_pocket = false;
		noo.add_notification(TRANSLATE("Failed to pick pocket")END);
		if (pickee_brain->caught_pick_pocket_text != "") {
			noo.map->add_speech(pickee_brain->caught_pick_pocket_text);
		}
		pick_pocket_failure_reaction();
	}
}
