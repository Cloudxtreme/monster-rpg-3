#include <Nooskewl_Engine/Nooskewl_Engine.h>

#include "brains.h"
#include "dll.h"
#include "gui.h"
#include "ml_start.h"
#include "ml_start_upper.h"
#include "ml_town.h"

bool dll_start()
{
	noo.window_title = "Monster RPG 3";
	return start_brains();
}

void dll_end()
{
	end_brains();
}

Map_Logic *dll_get_map_logic(std::string map_name)
{
	Map_Logic *ml = NULL;

	if (map_name == "start.map") {
		ml = new ML_start();
	}
	else if (map_name == "start_upper.map") {
		ml = new ML_start_upper();
	}
	else if (map_name == "town.map") {
		ml = new ML_town();
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

		inventory->from_string(data);

		return new Item_Drop_Brain(inventory);
	}
	else if (type == "shop") {
		Tokenizer t(data, '\n');

		std::string line1 = t.next();

		Tokenizer t2(line1, ',');

		int len = 0;

		std::string caption = t2.next();
		len += caption.length() + 1;
		caption = unescape_string(caption);

		std::string yes_option = t2.next();
		len += yes_option.length() + 1;
		yes_option = unescape_string(yes_option);

		std::string no_option = t2.next();
		len += no_option.length() + 1;
		no_option = unescape_string(no_option);

		std::string inv_size_s = t2.next();
		len += inv_size_s.length() + 1;

		int inv_size = atoi(inv_size_s.c_str());

		std::vector<int> costs;

		for (int i = 0; i < inv_size; i++) {
			std::string cost = t2.next();
			len += cost.length() + 1;

			costs.push_back(atoi(cost.c_str()));
		}

		std::string inventory_s = data.substr(line1.length());

		trim(inventory_s);

		Inventory *inventory = new Inventory();
		inventory->from_string(inventory_s);

		return new Shop_Brain(caption, yes_option, no_option, inventory, costs);
	}

	return 0;
}

void dll_pause()
{
	Pause_GUI *gui = new Pause_GUI();
	gui->start();
	noo.guis.push_back(gui);
}
