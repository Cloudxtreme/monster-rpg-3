#include <Nooskewl_Engine/Nooskewl_Engine.h>

#include "brains.h"
#include "dll.h"
#include "gui.h"
#include "ml_cabbagetown.h"
#include "ml_start.h"
#include "ml_start_upper.h"

bool dll_start()
{
	noo.window_title = "Monster RPG 3";
	return start_brains();
}

void dll_end()
{
	end_brains();
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
	else if (type == "shop") {
		Tokenizer t(data, '\n');

		std::string line1 = t.next();

		Tokenizer t2(line1, ',');

		std::string caption = t2.next();
		caption = unescape_string(caption);

		std::string yes_option = t2.next();
		yes_option = unescape_string(yes_option);

		std::string no_option = t2.next();
		no_option = unescape_string(no_option);

		std::string inv_size_s = t2.next();
		int inv_size = atoi(inv_size_s.c_str());

		std::string original_inv_size_s = t2.next();
		int original_inv_size = atoi(original_inv_size_s.c_str());

		std::string last_visit_s = t2.next();
		int last_visit = atoi(last_visit_s.c_str());

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

		std::string inventory_s = t.next() + "\n";
		std::string num_items_s = t.next() + "\n";
		inventory_s += num_items_s;
		int num_items = atoi(num_items_s.c_str());
		for (int i = 0; i < num_items; i++) {
			inventory_s += t.next() + "\n";
		}

		std::string original_inventory_s = t.next() + "\n";
		std::string original_num_items_s = t.next() + "\n";
		original_inventory_s += original_num_items_s;
		int original_num_items = atoi(original_num_items_s.c_str());
		for (int i = 0; i < original_num_items; i++) {
			original_inventory_s += t.next() + "\n";
		}

		trim(inventory_s);
		trim(original_inventory_s);

		Inventory *inventory = new Inventory();
		inventory->from_string(inventory_s);

		Inventory *original_inventory = new Inventory();
		original_inventory->from_string(original_inventory_s);

		return new Shop_Brain(caption, yes_option, no_option, inventory, costs, original_inventory, original_costs, last_visit);
	}
	else if (type == "growing_brain") {
		Tokenizer t(data, ',');

		std::string option;

		std::string baby_item = "";
		std::string fresh_item = "";
		std::string rotten_item = "";
		int instantiation_time = noo.get_play_time();

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
		}

		return new Growing_Brain(baby_item, fresh_item, rotten_item, instantiation_time);
	}

	return 0;
}

void dll_pause()
{
	Pause_GUI *gui = new Pause_GUI();
	gui->start();
	noo.guis.push_back(gui);
}
