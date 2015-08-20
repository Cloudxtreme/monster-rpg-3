#include "gui.h"

bool Buy_Sell_GUI::cancel;

void Buy_Sell_GUI::add_item(Inventory *inventory, std::vector<int> &costs, std::string name, int cost, int quantity)
{
	std::vector<Item *> items;
	for (int i = 0; i < quantity; i++) {
		items.push_back(new Item(name));
	}
	inventory->items.push_back(items);
	costs.push_back(cost);
}

void Buy_Sell_GUI::confirm_callback(void *data)
{
	if (data != 0) {
		cancel = true;
	}
}

Buy_Sell_GUI::Buy_Sell_GUI(Inventory *seller_inventory, std::vector<int> &seller_costs) :
	seller_inventory(seller_inventory),
	seller_costs(seller_costs),
	sell_count(0),
	buy_count(0),
	exit_menu(false)
{
	cancel = false; // static, can't use initializer list

	stats = noo.map->get_entity(0)->get_stats();

	for (size_t i = 0; i < seller_inventory->items.size(); i++) {
		for (size_t j = 0; j < seller_inventory->items[i].size(); j++) {
			Item *item = seller_inventory->items[i][j];
			costs[item] = seller_costs[i];
			their_original_inventory.push_back(item);
		}
	}

	for (size_t i = 0; i < stats->inventory->items.size(); i++) {
		for (size_t j = 0; j < stats->inventory->items[i].size(); j++) {
			Item *item = stats->inventory->items[i][j];
			costs[item] = item->get_value();
			your_original_inventory.push_back(item);
		}
	}

	Widget *modal_main_widget = new Widget(1.0f, 1.0f);
	SDL_Colour background_colour = { 0, 0, 0, 192 };
	modal_main_widget->set_background_colour(background_colour);

	Widget_Window *window = new Widget_Window(0.95f, 0.95f);
	window->set_center_x(true);
	window->set_center_y(true);
	window->set_parent(modal_main_widget);

	TGUI_Widget *pad = new TGUI_Widget(1.0f, 1.0f);
	pad->set_center_x(true);
	pad->set_center_y(true);
	pad->set_padding(5);
	pad->set_parent(window);

	TGUI_Widget *your_column = new TGUI_Widget(0.4f, 0.75f);
	your_column->set_parent(pad);

	your_gold_label = new Widget_Label("", 70);
	your_gold_label->set_parent(your_column);
	your_list = new Widget_List(1.0f, -1.0f);
	your_list->set_break_line(true);
	your_list->set_parent(your_column);
	
	TGUI_Widget *their_column = new TGUI_Widget(0.4f, 0.75f);
	their_column->set_parent(pad);

	their_gold_label = new Widget_Label("", 70);
	their_gold_label->set_parent(their_column);
	their_list = new Widget_List(1.0f, -1.0f);
	their_list->set_break_line(true);
	their_list->set_parent(their_column);

	TGUI_Widget *button_column = new TGUI_Widget(0.2f, 0.75f);
	button_column->set_parent(pad);

	accept_button = new Widget_Text_Button(TRANSLATE("Accept")END, -1, -1);
	accept_button->set_padding_top(int(noo.font->get_height() + 2));
	accept_button->set_parent(button_column);
	their_list->set_right_widget(accept_button); // feels better like this

	done_button = new Widget_Text_Button(TRANSLATE("Done")END, -1, -1);
	done_button->set_padding_top(int(noo.font->get_height() + 2));
	done_button->set_break_line(true);
	done_button->set_parent(button_column);

	TGUI_Widget *info_column = new TGUI_Widget(1.0f, 0.25f);
	info_column->set_parent(pad);

	cost_label = new Widget_Label("-", 100);
	cost_label->set_relative_position(0, 0);
	cost_label->set_parent(info_column);

	weight_label = new Widget_Label(TRANSLATE("Weight")END + ": -", 100);
	weight_label->set_relative_position(0, int(noo.font->get_height() + 2));
	weight_label->set_parent(info_column);

	value_label = new Widget_Label(TRANSLATE("Value")END + ": -", 100);
	value_label->set_relative_position(0, 2 * int(noo.font->get_height() + 2));
	value_label->set_parent(info_column);

	condition_label = new Widget_Label(TRANSLATE("Condition")END + ": -", 100);
	condition_label->set_relative_position(100, int(noo.font->get_height() + 2));
	condition_label->set_parent(info_column);

	properties_label = new Widget_Label("", 100);
	properties_label->set_relative_position(100, 2 * int(noo.font->get_height() + 2));
	properties_label->set_parent(info_column);

	set_lists();

	gui = new TGUI(modal_main_widget, noo.screen_size.w, noo.screen_size.h);

	gui->set_focus(done_button);

	set_labels();
}

void Buy_Sell_GUI::handle_event(TGUI_Event *event)
{
	if ((event->type == TGUI_KEY_DOWN && event->keyboard.code == TGUIK_ESCAPE) ||
		(event->type== TGUI_JOY_DOWN && event->joystick.button == noo.joy_b1)) {

		noo.button_mml->play(false);
		exit_menu = true;
	}
	else {
		GUI::handle_event(event);
	}
}

bool Buy_Sell_GUI::update()
{
	if (cancel) {
		return_items();
		return do_return(false);
	}

	set_labels();

	int pressed;

	if (accept_button->pressed()) {
		if (stats->inventory->gold < 0) {
			Notification_GUI *gui = new Notification_GUI(TRANSLATE("You don't have enough gold for this transaction.")END);
			gui->start();
			noo.guis.push_back(gui);
		}
		else if (seller_inventory->gold < 0) {
			Notification_GUI *gui = new Notification_GUI(TRANSLATE("Seller doesn't have enough gold for this transaction.")END);
			gui->start();
			noo.guis.push_back(gui);
		}
		else {
			move_original(stats->inventory, buy_count, their_original_inventory, your_original_inventory);
			move_original(seller_inventory, sell_count, your_original_inventory, their_original_inventory);
			merge(stats->inventory, buy_count);
			merge(seller_inventory, sell_count);
			buy_count = 0;
			sell_count = 0;
			clear_hilights();
			seller_costs.clear();
			for (size_t i = 0; i < seller_inventory->items.size(); i++) {
				seller_costs.push_back(costs[seller_inventory->items[i][0]]);
			}

			set_lists();

			// FIXME: play sound
		}
	}
	else if (done_button->pressed() || exit_menu) {
		if (buy_count > 0 || sell_count > 0) {
			Yes_No_GUI *gui = new Yes_No_GUI(TRANSLATE("Cancel transaction?")END, confirm_callback);
			gui->start();
			noo.guis.push_back(gui);
		}
		else {
			return do_return(false);
		}
	}
	else if ((pressed = your_list->pressed()) >= 0) {
		clear_hilights();
		swap_item(pressed, false);
		set_hilights();
	}
	else if ((pressed = their_list->pressed()) >= 0) {
		clear_hilights();
		swap_item(pressed, true);
		set_hilights();
	}

	return do_return(true);
}

void Buy_Sell_GUI::set_labels()
{
	bool is_your_list = your_list == gui->get_focus();
	bool is_their_list = is_your_list ? false : their_list ==  gui->get_focus();

	if (is_your_list || is_their_list) {
		Widget_List *list;
		Inventory *inventory;

		if (is_your_list) {
			list = your_list;
			inventory = stats->inventory;
		}
		else {
			list = their_list;
			inventory = seller_inventory;
		}
		int selected = list->get_selected();
		Item *item = inventory->items[selected][0];

		int condition = 100 * item->condition / 0xffff;

		weight_label->set_text(TRANSLATE("Weight")END + ": " + itos(item->weight));

		if (item->type != Item::OTHER) {
			condition_label->set_text(TRANSLATE("Condition")END + ": " + itos(condition) + "%");
		}
		else {
			condition_label->set_text(TRANSLATE("Condition")END + ": -");
		}

		int value = item->get_value();

		value_label->set_text(TRANSLATE("Value")END + ": " + itos(value));

		std::string cost_text;
		if (is_their_list) {
			cost_text = TRANSLATE("Buy price")END + ": ";
		}
		else {
			cost_text = TRANSLATE("Sell price")END + ": ";
		}
		cost_text += itos(get_cost(is_your_list));
		cost_label->set_text(cost_text);
	}
	else {
		cost_label->set_text("-");
		weight_label->set_text(TRANSLATE("Weight")END + ": -");
		condition_label->set_text(TRANSLATE("Condition")END + ": -");
		value_label->set_text(TRANSLATE("Value")END + ": -");
		properties_label->set_text("");
	}

	your_gold_label->set_text(itos(stats->inventory->gold) + " gold (You)");
	their_gold_label->set_text(itos(seller_inventory->gold) + " gold (Them)");

	gui->layout();
}

void Buy_Sell_GUI::set_list(Widget_List *list, Inventory *inventory, bool is_your_inventory)
{
	std::vector< std::vector<Item *> > &items = inventory->items;

	std::vector<std::string> item_list;
	for (size_t i = 0; i < items.size(); i++) {
		int count = items[i].size();
		if (count > 0) {
			// Don't allow selling equipped stuff
			if (
				(i == stats->weapon_index) ||
				(i == stats->armour_index)) {
				continue;
			}
			std::string name = items[i][0]->name;
			item_list.push_back(itos(count) + " " + name);
		}
	}

	list->set_items(item_list);
}

void Buy_Sell_GUI::set_lists()
{
	set_list(your_list, stats->inventory, true);
	set_list(their_list, seller_inventory, false);
}

Item *Buy_Sell_GUI::remove_item(int index, bool buying)
{
	Inventory *inventory;

	if (buying) {
		inventory = seller_inventory;
	}
	else {
		inventory = stats->inventory;
	}

	Item *item = inventory->items[index][0];
	int count = inventory->items[index].size();

	if (count == 1) {
		if (index == inventory->items.size()-1) {
			if (buying) {
				their_list->set_selected(index-1);
			}
			else {
				your_list->set_selected(index-1);
			}
		}
		inventory->items.erase(inventory->items.begin() + index);
		if (buying && index < sell_count) {
			sell_count--;
		}
		else if (!buying && index < buy_count) {
			buy_count--;
		}
	}
	else {
		inventory->items[index].erase(inventory->items[index].begin());
	}

	if (buying) {
		stats->inventory->gold -= costs[item];
		seller_inventory->gold += costs[item];
	}
	else {
		stats->inventory->gold += costs[item];
		seller_inventory->gold -= costs[item];
	}

	return item;
}

void Buy_Sell_GUI::add_item(Item *item, bool buying)
{
	Inventory *inventory;

	int max;

	if (buying) {
		inventory = stats->inventory;
		max = buy_count;
	}
	else {
		inventory = seller_inventory;
		max = sell_count;
	}

	int index = -1;

	for (size_t i = 0; i < inventory->items.size() && (int)i < max; i++) {
		if (inventory->items[i][0]->name == item->name) {
			index = i;
			break;
		}
	}

	if (index < 0) {
		bool new_item;
		if (buying) {
			if (std::find(your_original_inventory.begin(), your_original_inventory.end(), item) == your_original_inventory.end()) {
				new_item = true;
			}
			else {
				new_item = false;
			}
		}
		else {
			if (std::find(their_original_inventory.begin(), their_original_inventory.end(), item) == their_original_inventory.end()) {
				new_item = true;
			}
			else {
				new_item = false;
			}
		}
		int offset;
		if (buying) {
			offset = buy_count;
		}
		else {
			offset = sell_count;
		}
		std::vector<Item *> v;
		v.push_back(item);
		if (new_item) {
			inventory->items.insert(inventory->items.begin() + offset, v);
		}
		else {
			bool found = false;
			for (size_t i = 0; i < inventory->items.size(); i++) {
				if (inventory->items[i][0]->name == item->name) {
					found = true;
					inventory->items[i].push_back(item);
					break;
				}
			}
			if (found == false) {
				inventory->items.push_back(v);
			}
		}
		if (buying) {
			if (new_item) {
				buy_count++;
			}
		}
		else {
			if (new_item) {
				sell_count++;
			}
		}
	}
	else {
		inventory->items[index].push_back(item);
	}
}

void Buy_Sell_GUI::swap_item(int index, bool buying)
{
	Item *item = remove_item(index, buying);
	add_item(item, buying);

	set_lists();

	bool your_empty = gui->get_focus() == your_list && stats->inventory->items.size() == 0;
	bool their_empty = gui->get_focus() == their_list && seller_inventory->items.size() == 0;

	if (your_empty) {
		gui->focus_something();
	}
	else if (their_empty) {
		gui->focus_something();
	}

	if (stats->inventory->items.size() == 1) {
		your_list->set_selected(0);
	}
	if (seller_inventory->items.size() == 1) {
		their_list->set_selected(0);
	}
}

void Buy_Sell_GUI::return_items()
{
	int num_buy = buy_count;
	int num_sell = sell_count;

	for (int i = 0; i < num_buy; i++) {
		int count = stats->inventory->items[0].size();
		for (int j = 0; j < count; j++) {
			swap_item(0, false);
		}
	}

	for (int i = 0; i < num_sell; i++) {
		int count = seller_inventory->items[0].size();
		for (int j = 0; j < count; j++) {
			swap_item(0, true);
		}
	}
}

int Buy_Sell_GUI::get_cost(bool player)
{
	Widget_List *list;

	if (player) {
		list = your_list;
	}
	else {
		list = their_list;
	}

	int selected = list->get_selected();
	if (selected >= 0) {
		Item *item;
		if (player) {
			item = stats->inventory->items[selected][0];
		}
		else {
			item = seller_inventory->items[selected][0];
		}

		return costs[item];
	}
	else {
		return -1;
	}
}

void Buy_Sell_GUI::clear_hilights()
{
	for (size_t i = 0; i < stats->inventory->items.size(); i++) {
		your_list->set_hilight(i, false);
	}
	for (size_t i = 0; i < seller_inventory->items.size(); i++) {
		their_list->set_hilight(i, false);
	}
}

void Buy_Sell_GUI::set_hilights()
{
	for (int i = 0; i < buy_count; i++) {
		your_list->set_hilight(i, true);
	}
	for (int i = 0; i < sell_count; i++) {
		their_list->set_hilight(i, true);
	}
}

void Buy_Sell_GUI::merge(Inventory *inventory, int count)
{
	int index = 0;
	for (int i = 0; i < count; i++) {
		std::string name = inventory->items[index][0]->name;
		bool found = false;
		for (size_t j = 0; j < inventory->items.size(); j++) {
			if (index == (int)j) {
				continue;
			}
			if (inventory->items[j][0]->name == name) {
				inventory->items[j].insert(inventory->items[j].begin(), inventory->items[index].begin(), inventory->items[index].end());
				inventory->items.erase(inventory->items.begin() + index);
				found = true;
				break;
			}
		}
		if (found == false) {
			index++;
		}
	}
}

void Buy_Sell_GUI::move_original(Inventory *inventory, int count, std::vector<Item *> &move_from, std::vector<Item *> &move_to)
{
	for (int i = 0; i < count; i++) {
		for (size_t j = 0; j < inventory->items[i].size(); j++) {
			Item *item = inventory->items[i][j];
			std::vector<Item *>::iterator it = std::find(move_from.begin(), move_from.end(), item);
			if (it != move_from.end()) {
				move_from.erase(it);
			}
			move_to.push_back(item);
		}
	}
}

//--

Multiple_Choice_GUI::Multiple_Choice_GUI(std::string caption, std::vector<std::string> choices, Callback callback) :
	callback(callback)
{
	int w = 150;

	bool full;
	int num_lines;
	int width;

	// Get caption # lines
	noo.font->draw_wrapped(noo.white, caption, Point<float>(0, 0), w, (int)noo.font->get_height() + 3, -1, 0, 0, true, full, num_lines, width);

	int h = int((noo.font->get_height() + 3) * (3 + num_lines + 1));

	TGUI_Widget *modal_main_widget = new TGUI_Widget(1.0f, 1.0f);

	Widget_Window *window = new Widget_Window(w + 10, h + 10);
	window->set_float_right(true);
	window->set_float_bottom(true);
	window->set_padding_right(10);
	window->set_padding_bottom(10);
	window->set_parent(modal_main_widget);

	TGUI_Widget *pad = new TGUI_Widget(1.0f, 1.0f);
	pad->set_padding(5);
	pad->set_parent(window);

	caption_label = new Widget_Label(caption, w);
	caption_label->set_center_x(true);
	caption_label->set_parent(pad);

	list = new Widget_List(1.0f, int(h - (num_lines + 1) * (noo.font->get_height() + 3)));
	list->set_items(choices);
	list->set_padding_top(int(h - (3 * (3 + noo.font->get_height()))));
	list->set_parent(pad);

	gui = new TGUI(modal_main_widget, noo.screen_size.w, noo.screen_size.h);

	gui->set_focus(list);
}

bool Multiple_Choice_GUI::update()
{
	int pressed;
	if ((pressed = list->pressed()) >= 0) {
		callback((void *)pressed);
		return do_return(false);
	}

	return do_return(true);
}
