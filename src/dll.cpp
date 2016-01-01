#include <Nooskewl_Engine/Nooskewl_Engine.h>

#include "brain_actions.h"
#include "brains.h"
#include "dll.h"
#include "gui.h"
#include "ml_cabbagetown.h"
#include "ml_start.h"
#include "ml_start_upper.h"
#include "quest.h"

bool dll_start()
{
#ifdef DEMO
	noo.window_title = "Monster RPG 3 Demo";
#else
	noo.window_title = "Monster RPG 3";
#endif
	return start_brains();
}

void dll_end()
{
	end_brains();
}

void dll_game_started()
{
	init_quests();
}

void dll_pause()
{
	Pause_GUI *gui = new Pause_GUI();
	gui->start();
	noo.guis.push_back(gui);
}

Map_Logic *dll_get_map_logic(std::string map_name, int last_visited_time)
{
	Map_Logic *ml = NULL;

	if (map_name == "start.map") {
		ml = new ML_start(last_visited_time);
	}
	else if (map_name == "start_upper.map") {
		ml = new ML_start_upper(last_visited_time);
	}
	else if (map_name == "cabbagetown.map") {
		ml = new ML_cabbagetown(last_visited_time);
	}

	return ml;
}

Brain *dll_get_brain(std::string type, std::string data)
{
	if (type == "talk_brain") {
		std::string name = trim(data);
		return new Talk_Brain(name);
	}
	else if (type == "animated_brain") {
		std::string name = trim(data);
		return new Animated_Brain(name);
	}
	else if (type == "talk_then_animate_brain") {
		std::string name = trim(data);
		return new Talk_Then_Animate_Brain(name);
	}
	else if (type == "door_brain") {
		bool open;
		if (trim(data) == "open") {
			open = true;
		}
		else {
			open = false;
		}
		return new Door_Brain(open);
	}
	else if (type == "item_brain") {
		Tokenizer t(data, ',');

		std::string option;

		std::string name = "";;
		int quantity = 1;
		int milestone = -1;

		while ((option = t.next()) != "") {
			Tokenizer t2(option, '=');

			std::string key = t2.next();
			std::string value = t2.next();

			if (key == "name") {
				name = value;
			}
			else if (key == "quantity") {
				quantity = atoi(value.c_str());
			}
			else if (key == "milestone") {
				milestone = atoi(value.c_str());
			}
		}

		return new Item_Brain(name, quantity, milestone);
	}
	else if (type == "item_drop") {
		Inventory *inventory = new Inventory();

		Tokenizer t(data, '\n');

		std::string drop_time_s = t.next();
		int drop_time = atoi(drop_time_s.c_str());

		inventory->from_string(data.substr(drop_time_s.length()+1));

		return new Item_Drop_Brain(inventory, drop_time);
	}
	else if (type == "shop" || type == "no_activate_shop" || type == "bartender_shop") {
		Tokenizer t(data, '\n');

		std::string line1 = t.next();

		Tokenizer t2(line1, ',');

		std::string caption = t2.next();
		caption = unescape_string(caption);

		std::string yes_option = t2.next();
		yes_option = unescape_string(yes_option);

		std::string no_option = t2.next();
		no_option = unescape_string(no_option);

		int multiplier = atoi(t2.next().c_str());

		std::string inv_size_s = t2.next();
		int inv_size = atoi(inv_size_s.c_str());

		std::string original_inv_size_s = t2.next();
		int original_inv_size = atoi(original_inv_size_s.c_str());

		std::string last_visit_s = t2.next();
		int last_visit = atoi(last_visit_s.c_str());

		std::string can_pick_pocket_s = t2.next();
		bool can_pick_pocket = (bool)atoi(can_pick_pocket_s.c_str());

		std::vector<int> costs;

		for (int i = 0; i < inv_size; i++) {
			std::string cost = t2.next();
			costs.push_back(atoi(cost.c_str()));
		}

		std::vector<int> original_costs;

		for (int i = 0; i < original_inv_size; i++) {
			std::string cost = t2.next();
			original_costs.push_back(atoi(cost.c_str()));
		}

		std::string original_inventory_s = t.next() + "\n";
		std::string original_num_items_s = t.next() + "\n";
		original_inventory_s += original_num_items_s;
		int original_num_items = atoi(original_num_items_s.c_str());
		for (int i = 0; i < original_num_items; i++) {
			original_inventory_s += t.next() + "\n";
		}

		trim(original_inventory_s);

		Inventory *original_inventory = new Inventory();
		original_inventory->from_string(original_inventory_s);

		Base_Shop_Brain *brain;

		if (type == "shop") {
			brain = new Shop_Brain(caption, yes_option, no_option, multiplier, costs, original_inventory, original_costs, last_visit);
		}
		else if (type == "bartender_shop") {
			brain = new Bartender_Shop_Brain(caption, yes_option, no_option, multiplier, costs, original_inventory, original_costs, last_visit);
		}
		else {
			brain = new No_Activate_Shop_Brain(caption, yes_option, no_option, multiplier, costs, original_inventory, original_costs, last_visit);
		}

		brain->can_pick_pocket = can_pick_pocket;

		return brain;
	}
	else if (type == "growing_brain") {
		Tokenizer t(data, ',');

		std::string option;

		std::string baby_item = "";
		std::string fresh_item = "";
		std::string rotten_item = "";
		int instantiation_time = noo.get_play_time();
		int karma = 0;

		while ((option = t.next()) != "") {
			Tokenizer t2(option, '=');

			std::string key = t2.next();
			std::string value = t2.next();

			if (key == "baby_item") {
				baby_item = value;
			}
			else if (key == "fresh_item") {
				fresh_item = value;
			}
			else if (key == "rotten_item") {
				rotten_item = value;
			}
			else if (key == "instantiation_time") {
				instantiation_time = atoi(value.c_str());
			}
			else if (key == "karma") {
				karma = atoi(value.c_str());
			}
		}

		return new Growing_Brain(baby_item, fresh_item, rotten_item, instantiation_time, karma);
	}
	else if (type == "wander_brain") {
		Tokenizer t(data, ',');

		std::string option;

		std::string name;
		int max_distance;
		int delay;
		Point<int> start_pos;

		while ((option = t.next()) != "") {
			Tokenizer t2(option, '=');

			std::string key = t2.next();
			std::string value = t2.next();

			if (key == "name") {
				name = value;
			}
			else if (key == "max_distance") {
				max_distance = atoi(value.c_str());
			}
			else if (key == "delay") {
				delay = atoi(value.c_str());
			}
			else if (key == "start_pos") {
				Tokenizer t3(value, ':');

				std::string x = t3.next();
				std::string y = t3.next();

				start_pos = Point<int>(atoi(x.c_str()), atoi(y.c_str()));
			}
		}

		return new Wander_Brain(name, max_distance, delay, start_pos);
	}

	return 0;
}

struct Choose_Action_Data {
	Map_Entity *initiator;
	Map_Entity *target;
	std::vector<std::string> choices;
};

// Return true if a and b are in adjacent tiles
static bool adjacent(Map_Entity *a, Map_Entity *b)
{
	Point<int> a_pos = a->get_position();
	Point<int> b_pos = b->get_position();
	if ((a_pos.x == b_pos.x && abs(a_pos.y-b_pos.y) == 1) || (a_pos.y == b_pos.y && abs(a_pos.x-b_pos.x) == 1)) {
		return true;
	}
	return false;
}

static void choose_action_callback(void *data)
{
	Multiple_Choice_GUI::Callback_Data *mccd = static_cast<Multiple_Choice_GUI::Callback_Data *>(data);
	Choose_Action_Data *cad = static_cast<Choose_Action_Data *>(mccd->userdata);

	std::string action = cad->choices[mccd->choice];

	if (action == noo.game_t->translate(23) || action == TRANSLATE("Talk")END) {
		noo.map->activate(noo.player);
	}
	else if (action == noo.game_t->translate(114)) {
		Brain *brain = cad->target->get_brain();
		Pick_Pocketable_Brain *ppb = dynamic_cast<Pick_Pocketable_Brain *>(brain);
		if (ppb && adjacent(cad->initiator, cad->target)) {
			ppb->pick_pocket(cad->initiator, cad->target);
		}
	}
}

bool dll_choose_action(Map_Entity *entity)
{
	Choose_Action_Data *data = new Choose_Action_Data;
	data->initiator = noo.player; // FIXME
	data->target = entity;

	Brain *brain = entity->get_brain();

	if (dynamic_cast<Talk_Brain *>(brain) || dynamic_cast<Shop_Brain *>(brain) || dynamic_cast<No_Activate_Shop_Brain *>(brain)) {
		data->choices.push_back(TRANSLATE("Talk")END);
	}
	else {
		data->choices.push_back(noo.game_t->translate(23));
	}

	if (dynamic_cast<Pick_Pocketable_Brain *>(brain)) {
		if (adjacent(noo.player, entity)) {
			data->choices.push_back(noo.game_t->translate(114));
		}
	}

	Multiple_Choice_GUI *gui = new Multiple_Choice_GUI("", data->choices, -2, choose_action_callback, data);
	gui->start();
	noo.guis.push_back(gui);

	return true;
}
