#include "brains.h"
#include "gui.h"

bool Pause_GUI::quitting;
bool Pause_GUI::quit;
bool Pause_GUI::showing_items;
bool Pause_GUI::set_the_labels;

void Pause_GUI::callback(void *data)
{
	if (quitting) {
		quitting = false;
		quit = data != 0;
		if (quit) {
			showing_items = false;
			set_the_labels = false;
		}
	}
	else if (showing_items) {
		showing_items = false;
		set_the_labels = true;
	}
}

Pause_GUI::Pause_GUI() :
	exit_menu(false)
{
	quitting = quit = false;

	noo.game_paused();

	Widget *modal_main_widget = new Widget(1.0f, 1.0f);
	SDL_Colour background_colour = { 0, 0, 0, 192 };
	modal_main_widget->set_background_colour(background_colour);

	Widget_Window *window = new Widget_Window(0.95f, 0.95f);
	window->set_center_x(true);
	window->set_center_y(true);
	window->set_parent(modal_main_widget);

	quit_button = new Widget_Text_Button(noo.t->translate(8), 0.33f, -1);
	quit_button->set_center_x(true);

	save_button = new Widget_Text_Button(noo.t->translate(11), 0.34f, -1);
	save_button->set_center_x(true);
	save_button->set_padding_left(5);
	save_button->set_padding_right(5);

	resume_button = new Widget_Text_Button(noo.t->translate(10), 0.33f, -1);
	resume_button->set_center_x(true);

	Widget *button_container = new Widget(1.0f, quit_button->get_height());
	button_container->set_center_x(true);
	button_container->set_float_bottom(true);
	button_container->set_padding(5);
	button_container->set_parent(window);

	quit_button->set_parent(button_container);
	save_button->set_parent(button_container);
	resume_button->set_parent(button_container);

	TGUI_Widget *pad = new TGUI_Widget(1.0f, 1.0f);
	pad->set_center_x(true);
	pad->set_padding_left(5);
	pad->set_padding_right(5);
	pad->set_padding_top(5);
	pad->set_padding_bottom(5 + quit_button->get_height());
	pad->set_parent(window);

	TGUI_Widget *column1 = new TGUI_Widget(TGUI_Widget::FIT_Y, 0.33f);
	column1->set_center_x(true);
	column1->set_padding_bottom(quit_button->get_height() + 5);
	column1->set_parent(pad);

	TGUI_Widget *column2 = new TGUI_Widget(TGUI_Widget::FIT_Y, 0.34f);
	column2->set_center_x(true);
	column2->set_padding_left(5);
	column2->set_padding_right(5);
	column2->set_padding_bottom(quit_button->get_height() + 5);
	column2->set_parent(pad);

	stats = noo.map->get_entity(0)->get_stats();

	if (stats->profile_pic != 0) {
		profile_image = new Widget_Image(stats->profile_pic, false);
		profile_image->set_padding_bottom(2);
		profile_image->set_parent(column1);
	}

	name = new Widget_Label("", -1);
	name->set_break_line(true);
	name->set_parent(column1);

	alignment_label = new Widget_Label(TRANSLATE("Align")END + ": ", -1);
	alignment_label->set_break_line(true);
	alignment_label->set_padding_top((int)noo.font->get_height()+2);
	alignment_label->set_parent(column1);

	alignment = new Widget_Label("", -1);
	alignment->set_padding_top((int)noo.font->get_height()+2);
	alignment->set_parent(column1);

	sex_label = new Widget_Label(TRANSLATE("Sex")END + ": ", -1);
	sex_label->set_break_line(true);
	sex_label->set_parent(column1);

	sex = new Widget_Label("", -1);
	sex->set_parent(column1);

	gold_label = new Widget_Label(TRANSLATE("Gold")END + ": ", -1);
	gold_label->set_break_line(true);
	gold_label->set_parent(column1);

	gold = new Widget_Label("", -1);
	gold->set_parent(column1);

	hp_label = new Widget_Label(TRANSLATE("HP")END + ": ", -1);
	hp_label->set_break_line(true);
	hp_label->set_padding_top((int)noo.font->get_height()+2);
	hp_label->set_parent(column1);

	hp = new Widget_Label("", -1);
	hp->set_padding_top((int)noo.font->get_height()+2);
	hp->set_parent(column1);

	mp_label = new Widget_Label(TRANSLATE("MP")END + ": ", -1);
	mp_label->set_break_line(true);
	mp_label->set_parent(column1);

	mp = new Widget_Label("", -1);
	mp->set_parent(column1);

	experience_label = new Widget_Label(TRANSLATE("Exp")END + ": ", -1);
	experience_label->set_break_line(true);
	experience_label->set_parent(column1);

	experience = new Widget_Label("", -1);
	experience->set_parent(column1);

	weapon_image = new Widget_Image(new Image("weapon_icon.tga"));
	weapon_image->set_break_line(true);
	weapon_image->set_padding_top((int)noo.font->get_height()+2);
	weapon_image->set_parent(column1);

	weapon = new Widget_Label("", -1);
	weapon->set_padding_top((int)noo.font->get_height()+2);
	weapon->set_parent(column1);

	armour_image = new Widget_Image(new Image("armour_icon.tga"));
	armour_image->set_break_line(true);
	armour_image->set_parent(column1);

	armour = new Widget_Label("", -1);
	armour->set_parent(column1);

	attack_label = new Widget_Label(TRANSLATE("Attack")END + ": ", -1);
	attack_label->set_break_line(true);
	attack_label->set_padding_top(18);
	attack_label->set_parent(column2);

	attack = new Widget_Label("", -1);
	attack->set_padding_top(18);
	attack->set_parent(column2);

	defense_label = new Widget_Label(TRANSLATE("Defense")END + ": ", -1);
	defense_label->set_break_line(true);
	defense_label->set_parent(column2);

	defense = new Widget_Label("", -1);
	defense->set_parent(column2);

	agility_label = new Widget_Label(TRANSLATE("Agility")END + ": ", -1);
	agility_label->set_break_line(true);
	agility_label->set_parent(column2);

	agility = new Widget_Label("", -1);
	agility->set_parent(column2);

	luck_label = new Widget_Label(TRANSLATE("Luck")END + ": ", -1);
	luck_label->set_break_line(true);
	luck_label->set_parent(column2);

	luck = new Widget_Label("", -1);
	luck->set_parent(column2);

	speed_label = new Widget_Label(TRANSLATE("Speed")END + ": ", -1);
	speed_label->set_break_line(true);
	speed_label->set_parent(column2);

	speed = new Widget_Label("", -1);
	speed->set_parent(column2);

	strength_label = new Widget_Label(TRANSLATE("Strength")END + ": ", -1);
	strength_label->set_break_line(true);
	strength_label->set_parent(column2);

	strength = new Widget_Label("", -1);
	strength->set_parent(column2);

	karma_label = new Widget_Label(TRANSLATE("Karma")END + ": ", -1);
	karma_label->set_break_line(true);
	karma_label->set_parent(column2);

	karma = new Widget_Label("", -1);
	karma->set_parent(column2);

	hunger_label = new Widget_Label(TRANSLATE("Hunger")END + ": ", -1);
	hunger_label->set_break_line(true);
	hunger_label->set_parent(column2);

	hunger = new Widget_Label("", -1);
	hunger->set_parent(column2);

	thirst_label = new Widget_Label(TRANSLATE("Thirst")END + ": ", -1);
	thirst_label->set_break_line(true);
	thirst_label->set_parent(column2);

	thirst = new Widget_Label("", -1);
	thirst->set_parent(column2);

	rest_label = new Widget_Label(TRANSLATE("Rest")END + ": ", -1);
	rest_label->set_break_line(true);
	rest_label->set_parent(column2);

	rest = new Widget_Label("", -1);
	rest->set_parent(column2);

	sobriety_label = new Widget_Label(TRANSLATE("Sobriety")END + ": ", -1);
	sobriety_label->set_break_line(true);
	sobriety_label->set_parent(column2);

	sobriety = new Widget_Label("", -1);
	sobriety->set_parent(column2);

	TGUI_Widget *column3 = new TGUI_Widget(0.33f, 1.0f);
	column3->set_center_x(true);
	column3->set_padding_bottom(quit_button->get_height() + 5);
	column3->set_parent(pad);

	items_button = new Widget_Text_Button(TRANSLATE("Items")END, 1.0f, -1);
	items_button->set_parent(column3);

	weapons_button = new Widget_Text_Button(TRANSLATE("Weapons")END, 1.0f, -1);
	weapons_button->set_padding_top(2);
	weapons_button->set_padding_bottom(2);
	weapons_button->set_parent(column3);

	armour_button = new Widget_Text_Button(TRANSLATE("Armour")END, 1.0f, -1);
	armour_button->set_parent(column3);

	gui = new TGUI(modal_main_widget, noo.screen_size.w, noo.screen_size.h);

	gui->set_focus(resume_button);

	set_labels();
}

void Pause_GUI::handle_event(TGUI_Event *event)
{
	if (
		(event->type == TGUI_KEY_DOWN && event->keyboard.code == TGUIK_ESCAPE) ||
		(event->type== TGUI_JOY_DOWN && event->joystick.button == noo.joy_b1)) {
		noo.button_mml->play(false);
		exit_menu = true;
	}
	else {
		GUI::handle_event(event);
	}
}

bool Pause_GUI::update()
{
	if (exit_menu || check_quit() == false || resume_button->pressed()) {
		noo.game_unpaused();
		return do_return(false);
	}
	else if (save_button->pressed()) {
		Save_Load_GUI *save_load_gui = new Save_Load_GUI(true);
		save_load_gui->start();
		noo.guis.push_back(save_load_gui);
	}
	else if (quit_button->pressed()) {
		quitting = true;
		quit = false;

		Yes_No_GUI *yes_no_gui = new Yes_No_GUI(noo.t->translate(9), callback);
		yes_no_gui->start();
		noo.guis.push_back(yes_no_gui);
	}
	else if (items_button->pressed()) {
		showing_items = true;
		set_the_labels = false;
		Items_GUI *items_gui = new Items_GUI(Item::OTHER, callback);
		items_gui->start();
		noo.guis.push_back(items_gui);
	}
	else if (weapons_button->pressed()) {
		showing_items = true;
		set_the_labels = false;
		Items_GUI *items_gui = new Items_GUI(Item::WEAPON, callback);
		items_gui->start();
		noo.guis.push_back(items_gui);
	}
	else if (armour_button->pressed()) {
		showing_items = true;
		set_the_labels = false;
		Items_GUI *items_gui = new Items_GUI(Item::ARMOUR, callback);
		items_gui->start();
		noo.guis.push_back(items_gui);
	}

	if (set_the_labels) {
		set_the_labels = false;
		set_labels();
	}

	return do_return(true);
}

bool Pause_GUI::update_background()
{
	return do_return(check_quit());
}

bool Pause_GUI::check_quit()
{
	if (quit) {
		return false;
	}

	return true;
}

void Pause_GUI::set_labels()
{
	if (stats == 0) {
		return;
	}

	name->set_text(TRANSLATE("Eny")END); // FIXME

	if (stats->alignment == Stats::GOOD) {
		alignment->set_text(TRANSLATE("Good")END);
	}
	else if (stats->alignment == Stats::EVIL) {
		alignment->set_text(TRANSLATE("Evil")END);
	}
	else {
		alignment->set_text(TRANSLATE("Neutral")END);
	}

	if (stats->sex == Stats::MALE) {
		sex->set_text(TRANSLATE("Male")END);
	}
	else if (stats->sex == Stats::FEMALE) {
		sex->set_text(TRANSLATE("Female")END);
	}
	else {
		sex->set_text(TRANSLATE("Unknown")END);
	}

	gold->set_text(string_printf("%d", stats->inventory->gold));

	hp->set_text(string_printf("%d/%d", stats->hp, stats->max_hp));
	mp->set_text(string_printf("%d/%d", stats->mp, stats->max_mp));
	experience->set_text(string_printf("%d", stats->experience));

	if (stats->weapon_index >= 0) {
		weapon->set_text(stats->inventory->items[stats->weapon_index][0]->name);
	}
	else {
		weapon->set_text("");
	}
	if (stats->armour_index >= 0) {
		armour->set_text(stats->inventory->items[stats->armour_index][0]->name);
	}
	else {
		armour->set_text("");
	}

	attack->set_text(string_printf("%d", stats->attack));
	defense->set_text(string_printf("%d", stats->defense));
	agility->set_text(string_printf("%d", stats->agility));
	luck->set_text(string_printf("%d", stats->luck));
	speed->set_text(string_printf("%d", stats->speed));
	strength->set_text(string_printf("%d", stats->strength));

	karma->set_text(string_printf("%d%%", int((((float)stats->karma / 0xffff) * 2.0f - 1.0f) * 100)));
	hunger->set_text(string_printf("%d%%", int((((float)stats->hunger / 0xffff) * 2.0f - 1.0f) * 100)));
	thirst->set_text(string_printf("%d%%", int((((float)stats->thirst / 0xffff) * 2.0f - 1.0f) * 100)));
	rest->set_text(string_printf("%d%%", int((((float)stats->rest / 0xffff) * 2.0f - 1.0f) * 100)));
	sobriety->set_text(string_printf("%d%%", int((((float)stats->sobriety / 0xffff) * 2.0f - 1.0f) * 100)));

	int max_w;

	max_w = 0;
	max_w = MAX(max_w, alignment_label->get_width());
	max_w = MAX(max_w, sex_label->get_width());
	max_w = MAX(max_w, gold_label->get_width());
	alignment_label->set_width(max_w);
	sex_label->set_width(max_w);
	gold_label->set_width(max_w);

	max_w = 0;
	max_w = MAX(max_w, hp_label->get_width());
	max_w = MAX(max_w, mp_label->get_width());
	max_w = MAX(max_w, experience_label->get_width());
	hp_label->set_width(max_w);
	mp_label->set_width(max_w);
	experience_label->set_width(max_w);

	max_w = 0;
	max_w = MAX(max_w, attack_label->get_width());
	max_w = MAX(max_w, defense_label->get_width());
	max_w = MAX(max_w, agility_label->get_width());
	max_w = MAX(max_w, luck_label->get_width());
	max_w = MAX(max_w, speed_label->get_width());
	max_w = MAX(max_w, strength_label->get_width());
	max_w = MAX(max_w, karma_label->get_width());
	max_w = MAX(max_w, hunger_label->get_width());
	max_w = MAX(max_w, thirst_label->get_width());
	max_w = MAX(max_w, rest_label->get_width());
	max_w = MAX(max_w, sobriety_label->get_width());

	attack_label->set_width(max_w);
	defense_label->set_width(max_w);
	agility_label->set_width(max_w);
	luck_label->set_width(max_w);
	speed_label->set_width(max_w);
	strength_label->set_width(max_w);
	karma_label->set_width(max_w);
	hunger_label->set_width(max_w);
	thirst_label->set_width(max_w);
	rest_label->set_width(max_w);
	sobriety_label->set_width(max_w);

	gui->layout();
}

bool Pause_GUI::fade_done(bool fading_in) {
	if (fading_in == false) {
		if (quit) {
			delete noo.map;
			noo.map = 0;
			delete noo.player;
			noo.player = 0;
			noo.last_map_name = "";

			Title_GUI *title_gui = new Title_GUI();
			title_gui->start();
			noo.guis.push_back(title_gui);
		}
	}

	return false;
}

//--

bool Items_GUI::got_number;
int Items_GUI::number;

void Items_GUI::get_number_callback(void *data)
{
	number = (int64_t)data;
	got_number = true;
}

Items_GUI::Items_GUI(Item::Type type, Callback callback) :
	list(0),
	type(type),
	exit_menu(false),
	callback(callback)
{
	got_number = false;

	stats = noo.map->get_entity(0)->get_stats();

	Widget *modal_main_widget = new Widget(1.0f, 1.0f);
	SDL_Colour background_colour = { 0, 0, 0, 192 };
	modal_main_widget->set_background_colour(background_colour);

	Widget_Window *window = new Widget_Window(0.95f, 0.95f);
	window->set_center_x(true);
	window->set_center_y(true);
	window->set_parent(modal_main_widget);

	pad = new TGUI_Widget(1.0f, 1.0f);
	pad->set_center_x(true);
	pad->set_center_y(true);
	pad->set_padding(5);
	pad->set_parent(window);

	set_list();

	TGUI_Widget *info = new TGUI_Widget(0.4f, 1.0f);
	info->set_parent(pad);

	action_label = new Widget_Label(TRANSLATE("Action")END + ":", 70);
	action_label->set_parent(info);

	use_radio = new Widget_Radio_Button(TRANSLATE("Use")END);
	use_radio->set_selected(true);
	use_radio->set_break_line(true);
	use_radio->set_parent(info);

	std::vector<Map_Entity *> colliding = noo.map->get_colliding_entities(-1, noo.player->get_position(), Size<int>(1, 1));
	bool can_drop = true;
	for (size_t i = 0; i < colliding.size(); i++) {
		if (colliding[i] != noo.player) {
			can_drop = false;
			break;
		}
	}
	if (can_drop == false) {
		drop_radio = 0;
	}
	else {
		drop_radio = new Widget_Radio_Button(TRANSLATE("Drop")END);
		drop_radio->set_break_line(true);
		drop_radio->set_parent(info);
	}

	discard_radio = new Widget_Radio_Button(TRANSLATE("Discard")END);
	discard_radio->set_break_line(true);
	discard_radio->set_parent(info);

	Widget_Radio_Button::Group group;
	group.push_back(use_radio);
	if (drop_radio) {
		group.push_back(drop_radio);
	}
	group.push_back(discard_radio);
	use_radio->set_group(group);
	if (drop_radio) {
		drop_radio->set_group(group);
	}
	discard_radio->set_group(group);

	weight_label = new Widget_Label(TRANSLATE("Weight")END + ": -", 70);
	weight_label->set_padding_top(5);
	weight_label->set_break_line(true);
	weight_label->set_parent(info);

	value_label = new Widget_Label(TRANSLATE("Value")END + ": -", 70);
	value_label->set_break_line(true);
	value_label->set_parent(info);

	condition_label = new Widget_Label(TRANSLATE("Condition")END + ": -", 70);
	condition_label->set_break_line(true);
	condition_label->set_parent(info);

	properties_label = new Widget_Label("", 70);
	properties_label->set_break_line(true);
	properties_label->set_parent(info);

	done_button = new Widget_Text_Button(TRANSLATE("Done")END, -1, -1);
	done_button->set_parent(pad);

	gui = new TGUI(modal_main_widget, noo.screen_size.w, noo.screen_size.h);

	if (list) {
		list->set_right_widget(use_radio);
	}
	use_radio->set_right_widget(done_button);
	if (drop_radio) {
		drop_radio->set_right_widget(done_button);
	}
	discard_radio->set_right_widget(done_button);
	done_button->set_left_widget(use_radio);

	if (list) {
		gui->set_focus(list);
	}
	else {
		gui->set_focus(done_button);
	}

	set_labels();

	dropped_items = new Inventory;
}

void Items_GUI::handle_event(TGUI_Event *event)
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

bool Items_GUI::update()
{
	set_labels();

	int pressed;

	if (done_button->pressed() || exit_menu) {
		callback(0);
		handle_dropped_items();
		return do_return(false);
	}
	else if (list && ((pressed = list->pressed()) >= 0)) {
		int index = indices[pressed];

		if (use_radio->is_selected()) {
			if (type == Item::WEAPON) {
				if (stats->weapon_index == index) {
					stats->weapon_index = -1;
					list->set_hilight(index, false);
				}
				else {
					stats->weapon_index = index;
					list->set_hilight(index, true);
				}
			}
			else if (type == Item::ARMOUR) {
				if (stats->armour_index == index) {
					stats->armour_index = -1;
					list->set_hilight(index, false);
				}
				else {
					stats->armour_index = index;
					list->set_hilight(index, true);
				}
			}
			else {
				// FIXME
			}
		}
		else if (drop_radio && drop_radio->is_selected()) {
			int count = stats->inventory->items[pressed].size();
			if (count > 5) {
				got_number = false;
				dropping_index = index;
				Get_Number_GUI *gngui = new Get_Number_GUI(TRANSLATE("Drop how many?")END, count+1, 0, get_number_callback);
				gngui->start();
				noo.guis.push_back(gngui);
			}
			else {
				drop_item(index);
			}
		}
	}

	if (got_number) {
		got_number = false;
		if (number > 0) {
			if (drop_radio->is_selected()) {
				for (int i = 0; i < number; i++) {
					drop_item(dropping_index);
				}
			}
		}
	}

	return do_return(true);
}

void Items_GUI::set_labels()
{
	if (list != 0) {
		int selected = list->get_selected();
		if (selected < 0) {
			weight_label->set_text(TRANSLATE("Weight")END + ": -");
			condition_label->set_text(TRANSLATE("Condition")END + ": -");
			value_label->set_text(TRANSLATE("Value")END + ": -");
			properties_label->set_text("");
		}
		else {
			selected = indices[selected];
			Item *item = stats->inventory->items[selected][0];

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

			int attack = int(item->min_attack + ((condition / 100.0f) * (item->max_attack - item->min_attack)));

			if (item->type == Item::WEAPON) {
				properties_label->set_text(TRANSLATE("Attack")END + ": " + itos(attack));
			}
		}
	}

	gui->layout();
}

void Items_GUI::handle_dropped_items()
{
	if (dropped_items->items.size() > 0) {
		Map_Entity *drop = new Map_Entity("item_drop");
		drop->set_brain(new Item_Drop_Brain(dropped_items));
		drop->load_sprite("item_drop");
		drop->set_position(noo.player->get_position());
		drop->set_solid(false);
		drop->set_low(true);
		noo.map->add_entity(drop);
	}
	else {
		delete dropped_items;
	}
}

void Items_GUI::set_list()
{
	int hilight = -1;

	Inventory *inventory = stats->inventory;
	std::vector< std::vector<Item *> > &items = inventory->items;

	std::vector<std::string> item_list;
	for (size_t i = 0; i < items.size(); i++) {
		int count = items[i].size();
		if (count > 0) {
			if (items[i][0]->type == type) {
				std::string name = items[i][0]->name;
				item_list.push_back(itos(count) + " " + name);
				indices.push_back(i);
				if (type == Item::WEAPON && i == stats->weapon_index) {
					hilight = item_list.size() - 1;
				}
				else if (type == Item::ARMOUR && i == stats->armour_index) {
					hilight = item_list.size() - 1;
				}
			}
		}
	}

	if (list == 0 && item_list.size() == 0) {
		list = 0;
		TGUI_Widget *parent = new TGUI_Widget(0.4f, 1.0f);
		parent->set_parent(pad);
		Widget_Label *label = new Widget_Label("Inventory empty", -1);
		label->set_parent(parent);
	}
	else {
		bool exists = list != 0;
		if (exists == false) {
			list = new Widget_List(0.4f, 1.0f);
			list->set_parent(pad);
		}
		list->set_items(item_list);
		list->set_hilight(hilight, true);
	}
}

void Items_GUI::drop_item(int index)
{
	Item *item = stats->inventory->items[index][0];
	stats->inventory->items[index].erase(stats->inventory->items[index].begin());
	if (stats->inventory->items[index].size() == 0) {
		stats->inventory->items.erase(stats->inventory->items.begin() + index);
	}
	set_list();
	int selected = list->get_selected();
	if (selected >= (int)stats->inventory->items.size()) {
		selected--;
		if (selected < 0) {
			gui->focus_something();
		}
	}
	dropped_items->add(item);

	if (index == stats->weapon_index) {
		stats->weapon_index = -1;
	}
	else if (index == stats->armour_index) {
		stats->armour_index = -1;
	}
}

//--

bool Buy_Sell_GUI::cancel;
bool Buy_Sell_GUI::got_number;
int Buy_Sell_GUI::number;

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

void Buy_Sell_GUI::get_number_callback(void *data)
{
	number = (int64_t)data;
	got_number = true;
}

Buy_Sell_GUI::Buy_Sell_GUI(Inventory *seller_inventory, std::vector<int> &seller_costs, bool is_storage, Callback done_callback, void *callback_data) :
	seller_inventory(seller_inventory),
	seller_costs(seller_costs),
	sell_count(0),
	buy_count(0),
	is_storage(is_storage),
	exit_menu(false),
	done_callback(done_callback),
	callback_data(callback_data)
{
	cancel = false; // statics, can't use initializer list
	got_number = false;

	stats = noo.map->get_entity(0)->get_stats();

	if (stats->weapon_index < 0) {
		start_weapon = 0;
	}
	else {
		start_weapon = stats->inventory->items[stats->weapon_index][0];
	}
	if (stats->armour_index < 0) {
		start_armour = 0;
	}
	else {
		start_armour = stats->inventory->items[stats->armour_index][0];
	}

	for (size_t i = 0; i < seller_inventory->items.size(); i++) {
		for (size_t j = 0; j < seller_inventory->items[i].size(); j++) {
			Item *item = seller_inventory->items[i][j];
			if (is_storage == false) {
				costs[item] = seller_costs[i];
			}
			else {
				costs[item] = 0;
			}
			their_original_inventory.push_back(item);
		}
	}

	for (size_t i = 0; i < stats->inventory->items.size(); i++) {
		for (size_t j = 0; j < stats->inventory->items[i].size(); j++) {
			Item *item = stats->inventory->items[i][j];
			if (is_storage == false) {
				costs[item] = item->get_value();
			}
			else {
				costs[item] = 0;
			}
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

	if (is_storage == false) {
		your_gold_label = new Widget_Label("", 70);
	}
	else {
		your_gold_label = new Widget_Label(TRANSLATE("Your items")END, 70);
	}
	your_gold_label->set_parent(your_column);
	your_list = new Widget_List(1.0f, -1.0f);
	your_list->set_break_line(true);
	your_list->set_parent(your_column);
	
	TGUI_Widget *their_column = new TGUI_Widget(0.4f, 0.75f);
	their_column->set_parent(pad);

	if (is_storage == false) {
		their_gold_label = new Widget_Label("", 70);
	}
	else {
		their_gold_label = new Widget_Label(TRANSLATE("Drop")END, 70);
	}
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

	cha_ching = new MML("buy.mml");
}

Buy_Sell_GUI::~Buy_Sell_GUI()
{
	delete cha_ching;

	stats->weapon_index = -1;
	stats->armour_index = -1;

	for (size_t i = 0; i < stats->inventory->items.size(); i++) {
		for (size_t j = 0; j < stats->inventory->items[i].size(); j++) {
			if (stats->inventory->items[i][j] == start_weapon) {
				stats->weapon_index = i;
			}
			else if (stats->inventory->items[i][j] == start_armour) {
				stats->armour_index = i;
			}
		}
	}
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
		if (done_callback) {
			done_callback(callback_data);
		}
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
			if (is_storage == false) {
				seller_costs.clear();
				for (size_t i = 0; i < seller_inventory->items.size(); i++) {
					seller_costs.push_back(costs[seller_inventory->items[i][0]]);
				}
			}

			stats->inventory->sort();
			seller_inventory->sort();

			set_lists();

			cha_ching->play(false);
		}
	}
	else if (done_button->pressed() || exit_menu) {
		if (is_storage == false && (buy_count > 0 || sell_count > 0)) {
			Yes_No_GUI *gui = new Yes_No_GUI(TRANSLATE("Cancel transaction?")END, confirm_callback);
			gui->start();
			noo.guis.push_back(gui);
		}
		else {
			if (done_callback) {
				done_callback(callback_data);
			}
			return do_return(false);
		}
	}
	else if ((pressed = your_list->pressed()) >= 0) {
		int count = stats->inventory->items[pressed].size();
		if (count > 5) {
			got_number = false;
			getting_for_your_inventory = true;
			getting_for = pressed;
			Get_Number_GUI *gngui = new Get_Number_GUI(TRANSLATE("How many?")END, count+1, 0, get_number_callback);
			gngui->start();
			noo.guis.push_back(gngui);
		}
		else {
			clear_hilights();
			swap_item(pressed, false);
			set_hilights();
		}
	}
	else if ((pressed = their_list->pressed()) >= 0) {
		int count = seller_inventory->items[pressed].size();
		if (count > 5) {
			got_number = false;
			getting_for_your_inventory = false;
			getting_for = pressed;
			Get_Number_GUI *gngui = new Get_Number_GUI(TRANSLATE("How many?")END, count+1, 0, get_number_callback);
			gngui->start();
			noo.guis.push_back(gngui);
		}
		else {
			clear_hilights();
			swap_item(pressed, true);
			set_hilights();
		}
	}

	if (got_number) {
		got_number = false;
		if (number > 0) {
			clear_hilights();
			for (int i = 0; i < number; i++) {
				swap_item(getting_for, !getting_for_your_inventory);
			}
			set_hilights();
		}
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
		if (is_storage == false) {
			if (is_their_list) {
				cost_text = TRANSLATE("Buy price")END + ": ";
			}
			else {
				cost_text = TRANSLATE("Sell price")END + ": ";
			}
			cost_text += itos(get_cost(is_your_list));
		}
		else {
			cost_text = "-";
		}
		cost_label->set_text(cost_text);
	}
	else {
		cost_label->set_text("-");
		weight_label->set_text(TRANSLATE("Weight")END + ": -");
		condition_label->set_text(TRANSLATE("Condition")END + ": -");
		value_label->set_text(TRANSLATE("Value")END + ": -");
		properties_label->set_text("");
	}

	if (is_storage == false) {
		your_gold_label->set_text(itos(stats->inventory->gold) + " gold (You)");
		their_gold_label->set_text(itos(seller_inventory->gold) + " gold (Them)");
	}

	gui->layout();
}

void Buy_Sell_GUI::set_list(Widget_List *list, Inventory *inventory, bool is_your_inventory)
{
	std::vector< std::vector<Item *> > &items = inventory->items;

	std::vector<std::string> item_list;
	for (size_t i = 0; i < items.size(); i++) {
		int count = items[i].size();
		if (count > 0) {
			std::string name = items[i][0]->name;
			item_list.push_back(itos(count) + " " + name);
		}
		else {
			printf("Zero sized item array...\n");
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
			inventory->sort(0, max+1);
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
				inventory->sort(max);
			}
			else {
				inventory->sort(0, max+1);
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
	if (is_storage) {
		return 0;
	}

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

Multiple_Choice_GUI::Multiple_Choice_GUI(std::string caption, std::vector<std::string> choices, Callback callback, void *callback_data) :
	callback(callback),
	callback_data(callback_data)
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
		Callback_Data data;
		data.choice = pressed;
		data.userdata = callback_data;
		callback((void *)&data);
		return do_return(false);
	}

	return do_return(true);
}
