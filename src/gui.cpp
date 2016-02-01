#include "monster-rpg-3.h"
#include "brains.h"
#include "gui.h"
#include "quest.h"
#include "widgets.h"

static bool use_item(Stats *stats, int index)
{
	Item *item = stats->inventory->items[index][0];

	bool remove = true;

	if (item->id == "apple") {
		DEC_HUNGER(stats, 0xffff/4);
	}
	else if (item->id == "beer") {
		DEC_THIRST(stats, 0xffff/2);
		DEC_SOBRIETY(stats, 0xffff/4);
	}
	else if (item->id == "cabbage") {
		DEC_HUNGER(stats, 0xffff/6);
	}
	else if (item->id == "red_cabbage") {
		DEC_HUNGER(stats, 0xffff);
	}
	else if (item->id == "fish") {
		DEC_HUNGER(stats, 0xffff/3);
	}
	else if (item->id == "pickled_egg") {
		DEC_HUNGER(stats, 0xffff/8);
	}
	else if (item->id == "rotten_cabbage") {
		DEC_HUNGER(stats, 0xffff/20);
		stats->set_status(Stats::SICK);
	}
	else if (item->id == "wine") {
		DEC_THIRST(stats, 0xffff/5);
		DEC_SOBRIETY(stats, 0xffff/4);
	}
	else if (item->id == "baby_cabbage") {
		DEC_HUNGER(stats, 0xffff/12);
	}
	else if (item->id == "chicken") {
		DEC_HUNGER(stats, 0xffff);
	}
	else {
		remove = false;
	}

	return remove;
}

//--

bool Pause_GUI::quitting;
bool Pause_GUI::quit;
bool Pause_GUI::showing_items;
bool Pause_GUI::set_the_labels;

void Pause_GUI::callback(void *data)
{
	if (quitting) {
		quitting = false;
		quit = ((Yes_No_GUI::Callback_Data *)data)->choice;
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

void Pause_GUI::crafting_callback(void *data)
{
	Multiple_Choice_GUI::Callback_Data *d = static_cast<Multiple_Choice_GUI::Callback_Data *>(data);

	if (d->choice == 0) {
		Crafting_GUI *gui = new Crafting_GUI(Item::OTHER);
		gui->start();
		noo.guis.push_back(gui);
	}
	else if (d->choice == 1) {
		Crafting_GUI *gui = new Crafting_GUI(Item::WEAPON);
		gui->start();
		noo.guis.push_back(gui);
	}
	else if (d->choice == 2) {
		Crafting_GUI *gui = new Crafting_GUI(Item::ARMOUR);
		gui->start();
		noo.guis.push_back(gui);
	}
	else {
		Crafting_GUI *gui = new Crafting_GUI(Item::OTHER, true);
		gui->start();
		noo.guis.push_back(gui);
	}

	set_the_labels = true;
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

	TGUI_Widget *profile_container = new TGUI_Widget();
	profile_container->set_parent(column1);

	profile_image = new Widget_Image(stats->profile_pic, false);
	profile_image->set_padding_bottom(2);
	profile_image->set_parent(profile_container);

	status = new Widget_Label("", -1);
	status->set_center_y(true);
	status->set_padding_left(2);
	status->set_parent(profile_container);

	name = new Widget_Label("", -1);
	name->set_break_line(true);
	name->set_parent(column1);

	alignment_label = new Widget_Label(noo.game_t->translate(26) + ": ", -1);
	alignment_label->set_break_line(true);
	alignment_label->set_padding_top((int)noo.font->get_height()+2);
	alignment_label->set_parent(column1);

	alignment = new Widget_Label("", -1);
	alignment->set_padding_top((int)noo.font->get_height()+2);
	alignment->set_parent(column1);

	sex_label = new Widget_Label(noo.game_t->translate(121) + ": ", -1);
	sex_label->set_break_line(true);
	sex_label->set_parent(column1);

	sex = new Widget_Label("", -1);
	sex->set_parent(column1);

	gold_label = new Widget_Label(noo.game_t->translate(74) + ": ", -1);
	gold_label->set_break_line(true);
	gold_label->set_parent(column1);

	gold = new Widget_Label("", -1);
	gold->set_parent(column1);

	hp_label = new Widget_Label(noo.game_t->translate(85) + ": ", -1);
	hp_label->set_break_line(true);
	hp_label->set_padding_top((int)noo.font->get_height()+2);
	hp_label->set_parent(column1);

	hp = new Widget_Label("", -1);
	hp->set_padding_top((int)noo.font->get_height()+2);
	hp->set_parent(column1);

	mp_label = new Widget_Label(noo.game_t->translate(102) + ": ", -1);
	mp_label->set_break_line(true);
	mp_label->set_parent(column1);

	mp = new Widget_Label("", -1);
	mp->set_parent(column1);

	experience_label = new Widget_Label(noo.game_t->translate(69) + ": ", -1);
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

	attack_label = new Widget_Label(noo.game_t->translate(49) + ": ", -1);
	attack_label->set_break_line(true);
	attack_label->set_padding_top(18);
	attack_label->set_parent(column2);

	attack = new Widget_Label("", -1);
	attack->set_padding_top(18);
	attack->set_parent(column2);

	defense_label = new Widget_Label(noo.game_t->translate(59) + ": ", -1);
	defense_label->set_break_line(true);
	defense_label->set_parent(column2);

	defense = new Widget_Label("", -1);
	defense->set_parent(column2);

	agility_label = new Widget_Label(noo.game_t->translate(25) + ": ", -1);
	agility_label->set_break_line(true);
	agility_label->set_parent(column2);

	agility = new Widget_Label("", -1);
	agility->set_parent(column2);

	luck_label = new Widget_Label(noo.game_t->translate(99) + ": ", -1);
	luck_label->set_break_line(true);
	luck_label->set_parent(column2);

	luck = new Widget_Label("", -1);
	luck->set_parent(column2);

	speed_label = new Widget_Label(noo.game_t->translate(125) + ": ", -1);
	speed_label->set_break_line(true);
	speed_label->set_parent(column2);

	speed = new Widget_Label("", -1);
	speed->set_parent(column2);

	strength_label = new Widget_Label(noo.game_t->translate(126) + ": ", -1);
	strength_label->set_break_line(true);
	strength_label->set_parent(column2);

	strength = new Widget_Label("", -1);
	strength->set_parent(column2);

	karma_label = new Widget_Label(noo.game_t->translate(96) + ": ", -1);
	karma_label->set_break_line(true);
	karma_label->set_parent(column2);

	karma = new Widget_Label("", -1);
	karma->set_parent(column2);

	hunger_label = new Widget_Label(noo.game_t->translate(86) + ": ", -1);
	hunger_label->set_break_line(true);
	hunger_label->set_parent(column2);

	hunger = new Widget_Label("", -1);
	hunger->set_parent(column2);

	thirst_label = new Widget_Label(noo.game_t->translate(132) + ": ", -1);
	thirst_label->set_break_line(true);
	thirst_label->set_parent(column2);

	thirst = new Widget_Label("", -1);
	thirst->set_parent(column2);

	rest_label = new Widget_Label(noo.game_t->translate(118) + ": ", -1);
	rest_label->set_break_line(true);
	rest_label->set_parent(column2);

	rest = new Widget_Label("", -1);
	rest->set_parent(column2);

	sobriety_label = new Widget_Label(noo.game_t->translate(124) + ": ", -1);
	sobriety_label->set_break_line(true);
	sobriety_label->set_parent(column2);

	sobriety = new Widget_Label("", -1);
	sobriety->set_parent(column2);

	TGUI_Widget *column3 = new TGUI_Widget(0.33f, 1.0f);
	column3->set_center_x(true);
	column3->set_padding_bottom(quit_button->get_height() + 5);
	column3->set_parent(pad);

	items_button = new Widget_Text_Button(noo.game_t->translate(92), 1.0f, -1);
	items_button->set_parent(column3);

	weapons_button = new Widget_Text_Button(noo.game_t->translate(139), 1.0f, -1);
	weapons_button->set_padding_top(2);
	weapons_button->set_parent(column3);

	armour_button = new Widget_Text_Button(noo.game_t->translate(48), 1.0f, -1);
	armour_button->set_padding_top(2);
	armour_button->set_parent(column3);

	spells_button = new Widget_Text_Button(TRANSLATE("Spells")END, 1.0f, -1);
	spells_button->set_padding_top(2);
	spells_button->set_parent(column3);

	crafting_button = new Widget_Text_Button(TRANSLATE("Crafting")END, 1.0f, -1);
	crafting_button->set_padding_top(2);
	crafting_button->set_parent(column3);

	quests_button = new Widget_Text_Button(noo.game_t->translate(116), 1.0f, -1);
	quests_button->set_padding_top(2);
	quests_button->set_parent(column3);

	time_label = new Widget_Label(get_day_time_string(), -1);
	time_label->set_padding_top(2);
	time_label->set_float_left(true);

	Widget *time_and_audio_toggle_container = new Widget(1.0f, (int)time_label->get_height());
	time_and_audio_toggle_container->set_parent(column3);

	time_label->set_parent(time_and_audio_toggle_container);

	Widget *audio_toggle_container = new Widget(1.0f, 5);
	Sprite *audio_toggle_sprite = new Sprite("audio_toggle");
	if (noo.mute) {
		audio_toggle_sprite->set_animation("off");
	}
	else {
		audio_toggle_sprite->set_animation("on");
	}
	audio_toggle = new Widget_Sprite_Toggle(audio_toggle_sprite, !noo.mute, true);
	audio_toggle->set_padding_top(2);
	audio_toggle->set_float_right(true);
	audio_toggle->set_parent(audio_toggle_container);
	audio_toggle_container->set_parent(time_and_audio_toggle_container);

	quests_button->set_down_widget(audio_toggle);
	resume_button->set_up_widget(audio_toggle);

	gui = new TGUI(modal_main_widget, noo.screen_size.w, noo.screen_size.h);

	gui->set_focus(resume_button);

	set_labels();
}

void Pause_GUI::handle_event(TGUI_Event *event)
{
	if (
		(event->type == TGUI_KEY_DOWN && event->keyboard.code == noo.key_b2) ||
		(event->type== TGUI_JOY_DOWN && event->joystick.button == noo.joy_b2)) {
		noo.button_mml->play(false);
		exit_menu = true;
	}
	else {
		GUI::handle_event(event);
	}
}

void Pause_GUI::update()
{
	if (check_quit() == false || resume_button->pressed() || exit_menu) {
		noo.game_unpaused();
		exit();
		return;
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
	else if (spells_button->pressed()) {
		Spells_GUI *gui = new Spells_GUI();
		gui->start();
		noo.guis.push_back(gui);
	}
	else if (crafting_button->pressed()) {
		std::vector<std::string> choices;
		choices.push_back(TRANSLATE("Items")END);
		choices.push_back(TRANSLATE("Weapons")END);
		choices.push_back(TRANSLATE("Armour")END);
		choices.push_back(TRANSLATE("Spells")END);
		Multiple_Choice_GUI *gui = new Multiple_Choice_GUI(TRANSLATE("Craft what?")END, choices, -2, crafting_callback, 0);
		gui->start();
		noo.guis.push_back(gui);
	}
	else if (quests_button->pressed()) {
		Quests_GUI *quests_gui = new Quests_GUI();
		quests_gui->start();
		noo.guis.push_back(quests_gui);
	}
	if (audio_toggle->get_value() == noo.mute) { // if toggle changed
		noo.mute = !noo.mute;
		if (noo.mute) {
			audio_toggle->set_animation("off");
			MML::pause_all();
		}
		else {
			audio_toggle->set_animation("on");
			noo.map->start_audio();
		}
	}

	if (set_the_labels) {
		set_the_labels = false;
		set_labels();
	}
}

void Pause_GUI::update_background()
{
	if (check_quit() == false) {
		exit();
	}
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

	name->set_text(noo.game_t->translate(67)); // FIXME

	if (stats->status == Stats::SICK) {
		status->set_text(noo.game_t->translate(122));
	}
	else if (stats->status == Stats::DRUNK) {
		status->set_text(noo.game_t->translate(66));
	}
	else {
		status->set_text("");
	}

	if (stats->alignment == Stats::GOOD) {
		alignment->set_text(noo.game_t->translate(76));
	}
	else if (stats->alignment == Stats::EVIL) {
		alignment->set_text(noo.game_t->translate(68));
	}
	else {
		alignment->set_text(noo.game_t->translate(104));
	}

	if (stats->sex == Stats::MALE) {
		sex->set_text(noo.game_t->translate(100));
	}
	else if (stats->sex == Stats::FEMALE) {
		sex->set_text(noo.game_t->translate(70));
	}
	else {
		sex->set_text(noo.game_t->translate(136));
	}

	gold->set_text(string_printf("%d", stats->inventory->gold));

	hp->set_text(string_printf("%d/%d", stats->hp, stats->characteristics.get_modified_max_hp(stats)));
	mp->set_text(string_printf("%d/%d", stats->mp, stats->characteristics.get_modified_max_mp(stats)));
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

	attack->set_text(string_printf("%d", stats->characteristics.get_modified_attack(stats)));
	defense->set_text(string_printf("%d", stats->characteristics.get_modified_defense(stats)));
	agility->set_text(string_printf("%d", stats->characteristics.get_modified_agility(stats)));
	luck->set_text(string_printf("%d", stats->characteristics.get_modified_luck(stats)));
	speed->set_text(string_printf("%d", stats->characteristics.get_modified_speed(stats)));
	strength->set_text(string_printf("%d", stats->characteristics.get_modified_strength(stats)));

	karma->set_text(string_printf("%d%%", int((((float)stats->karma / 0xffff) * 2.0f - 1.0f) * 100)));
	hunger->set_text(string_printf("%d%%", int(((float)stats->hunger / 0xffff) * 100)));
	thirst->set_text(string_printf("%d%%", int(((float)stats->thirst / 0xffff) * 100)));
	rest->set_text(string_printf("%d%%", int(((float)stats->rest / 0xffff) * 100)));
	sobriety->set_text(string_printf("%d%%", int(((float)stats->sobriety / 0xffff) * 100)));

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

	time_label->set_text(get_day_time_string());

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
		}
	}

	return false;
}

//--

bool Items_GUI::got_number;
int Items_GUI::number;
bool Items_GUI::discard_item;

void Items_GUI::get_number_callback(void *data)
{
	number = (int64_t)data;
	got_number = true;
}

void Items_GUI::discard_callback(void *data)
{
	discard_item = data != 0;
}

Items_GUI::Items_GUI(Item::Type type, Callback callback) :
	list(0),
	type(type),
	exit_menu(false),
	callback(callback)
{
	got_number = false;
	discard_item = false;

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

	Widget *title_container = new Widget(1.0f, int(noo.font->get_height() + 5));
	title_container->set_parent(pad);
	std::string title_text;
	if (type == Item::OTHER) {
		title_text = noo.game_t->translate(92);
	}
	else if (type == Item::WEAPON) {
		title_text = noo.game_t->translate(139);
	}
	else if (type == Item::ARMOUR) {
		title_text = noo.game_t->translate(48);
	}
	Widget_Label *title = new Widget_Label(title_text, 100);
	title->set_center_x(true);
	title->set_parent(title_container);

	weight_header = new Widget(1.0f, int(noo.font->get_height() + 5));
	weight_header->set_padding_bottom(2);
	weight_header->set_parent(pad);
	carrying_label = new Widget_Label("", 100);
	carrying_label->set_padding(1);
	carrying_label->set_parent(weight_header);
	capacity_label = new Widget_Label("", 100);
	capacity_label->set_padding(1);
	capacity_label->set_float_right(true);
	capacity_label->set_parent(weight_header);

	set_list();

	TGUI_Widget *info = new TGUI_Widget(0.4f, -1.0f);
	info->set_parent(pad);

	action_label = new Widget_Label(noo.game_t->translate(22) + ":", 70);
	action_label->set_parent(info);

	use_radio = new Widget_Radio_Button(noo.game_t->translate(137));
	use_radio->set_selected(true);
	use_radio->set_break_line(true);
	use_radio->set_parent(info);

	std::vector<Map_Entity *> colliding = noo.map->get_colliding_entities(-1, noo.player->get_position(), Size<int>(1, 1));
	bool can_drop = true;
	Item_Drop_Brain *drop_brain = 0;
	created_dropped_items = false;
	for (size_t i = 0; i < colliding.size(); i++) {
		if (colliding[i] != noo.player) {
			Brain *brain = colliding[i]->get_brain();
			if (dynamic_cast<Item_Drop_Brain *>(brain)) {
				drop_brain = (Item_Drop_Brain *)brain;
				break;
			}
			else {
				can_drop = false;
				break;
			}
		}
	}
	if (can_drop == false) {
		drop_radio = 0;
	}
	else {
		drop_radio = new Widget_Radio_Button(noo.game_t->translate(64));
		drop_radio->set_break_line(true);
		drop_radio->set_parent(info);

		if (drop_brain) {
			dropped_items = drop_brain->get_inventory();
		}
		else {
			dropped_items = new Inventory;
			created_dropped_items = true;
		}
	}

	discard_radio = new Widget_Radio_Button(noo.game_t->translate(61));
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

	weight_label = new Widget_Label(noo.game_t->translate(140) + ": -", 70);
	weight_label->set_padding_top(5);
	weight_label->set_break_line(true);
	weight_label->set_parent(info);

	value_label = new Widget_Label(noo.game_t->translate(138) + ": -", 70);
	value_label->set_break_line(true);
	value_label->set_parent(info);

	condition_label = new Widget_Label(noo.game_t->translate(58) + ": -", 70);
	condition_label->set_break_line(true);
	condition_label->set_parent(info);

	properties_label = new Widget_Label("", 70);
	properties_label->set_break_line(true);
	properties_label->set_parent(info);

	modifiers_label = new Widget_Label("", 70);
	modifiers_label->set_break_line(true);
	modifiers_label->set_padding_top(5);
	modifiers_label->set_parent(info);

	TGUI_Widget *column3 = new TGUI_Widget(0.19f, 1.0f);
	column3->set_parent(pad);

	done_button = new Widget_Text_Button(noo.game_t->translate(63), -1, -1);
	done_button->set_parent(column3);

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
}

void Items_GUI::handle_event(TGUI_Event *event)
{
	if ((event->type == TGUI_KEY_DOWN && event->keyboard.code == noo.key_b2) ||
		(event->type== TGUI_JOY_DOWN && event->joystick.button == noo.joy_b2)) {

		noo.button_mml->play(false);
		exit_menu = true;
	}
	else {
		GUI::handle_event(event);
	}
}

void Items_GUI::update()
{
	int pressed;

	if (got_number) {
		got_number = false;
		if (number > 0) {
			if (drop_radio->is_selected()) {
				for (int i = 0; i < number; i++) {
					remove_item(dropping_index, true);
				}
			}
		}
	}

	if (discard_item) {
		discard_item = false;
		remove_item(discard_index, false);
	}

	if (done_button->pressed() || exit_menu) {
		callback(0);
		handle_dropped_items();
		exit();
		return;
	}
	else if (list && ((pressed = list->pressed()) >= 0)) {
		int index = indices[pressed];

		if (use_radio->is_selected()) {
			if (type == Item::WEAPON) {
				if (stats->weapon_index >= 0 && stats->weapon_index == index) {
					stats->weapon_index = -1;
					list->set_hilight(pressed, false);
				}
				else {
					clear_highlights();
					stats->weapon_index = index;
					list->set_hilight(pressed, true);
				}
			}
			else if (type == Item::ARMOUR) {
				if (stats->armour_index >= 0 && stats->armour_index == index) {
					stats->armour_index = -1;
					list->set_hilight(pressed, false);
				}
				else {
					clear_highlights();
					stats->armour_index = index;
					list->set_hilight(pressed, true);
				}
			}
			else {
				if (use_item(stats, index)) {
					remove_item(index, false);
				}
			}
		}
		else if (drop_radio && drop_radio->is_selected()) {
			int count = stats->inventory->items[pressed].size();
			if (count > 5) {
				got_number = false;
				dropping_index = index;
				Get_Number_GUI *gngui = new Get_Number_GUI(noo.game_t->translate(65), count+1, 0, get_number_callback);
				gngui->start();
				noo.guis.push_back(gngui);
			}
			else {
				remove_item(index, true);
			}
		}
		else {
			// discard selected
			discard_index = index;

			int milestone = noo.milestone_name_to_number("Always Discard");

			if (noo.check_milestone(milestone) == true) {
				discard_item = true;
			}
			else {
				Yes_No_Always_GUI *gui = new Yes_No_Always_GUI(noo.game_t->translate(62), milestone, discard_callback);
				gui->start();
				noo.guis.push_back(gui);
			}
		}
	}

	set_labels();
}

void Items_GUI::set_labels()
{
	if (list != 0) {
		int selected = list->get_selected();
		if (selected < 0) {
			weight_label->set_text(noo.game_t->translate(140) + ": -");
			condition_label->set_text(noo.game_t->translate(58) + ": -");
			value_label->set_text(noo.game_t->translate(138) + ": -");
			properties_label->set_text("");
			modifiers_label->set_text("");
		}
		else {
			selected = indices[selected];
			Item *item = stats->inventory->items[selected][0];

			int condition = 100 * item->condition / 0xffff;

			weight_label->set_text(noo.game_t->translate(140) + ": " + Inventory::decimal_to_string(item->weight));

			if (item->type != Item::OTHER) {
				condition_label->set_text(noo.game_t->translate(58) + ": " + itos(condition) + "%");
			}
			else {
				condition_label->set_text(noo.game_t->translate(58) + ": -");
			}

			int value = item->get_value();

			value_label->set_text(noo.game_t->translate(138) + ": " + Inventory::decimal_to_string(value));

			if (item->type == Item::WEAPON) {
				int attack = int(item->min_attack + ((condition / 100.0f) * (item->max_attack - item->min_attack)));
				properties_label->set_text(noo.game_t->translate(49) + ": " + itos(attack));
			}
			else if (item->type == Item::ARMOUR) {
				int defense = int(item->min_defense + ((condition / 100.0f) * (item->max_defense - item->min_defense)));
				properties_label->set_text(TRANSLATE("Defense")END + ": " + itos(defense));
			}

			std::string modifiers_text;

			Stats::Characteristics &mods = item->modifiers;

			int m_max_hp = int((((float)mods.get_max_hp() - (0xffff/2)) / (0xffff/2)) * 100);
			int m_max_mp = int((((float)mods.get_max_mp() - (0xffff/2)) / (0xffff/2)) * 100);
			int m_attack = int((((float)mods.get_attack() - (0xffff/2)) / (0xffff/2)) * 100);
			int m_defense = int((((float)mods.get_defense() - (0xffff/2)) / (0xffff/2)) * 100);
			int m_agility = int((((float)mods.get_agility() - (0xffff/2)) / (0xffff/2)) * 100);
			int m_luck = int((((float)mods.get_luck() - (0xffff/2)) / (0xffff/2)) * 100);
			int m_speed = int((((float)mods.get_speed() - (0xffff/2)) / (0xffff/2)) * 100);
			int m_strength = int((((float)mods.get_strength() - (0xffff/2)) / (0xffff/2)) * 100);

			if (m_max_hp != 0) {
				modifiers_text += TRANSLATE("Max. HP")END + " " + (m_max_hp > 0 ? "+" : "") + itos(m_max_hp) + "%$";
			}
			if (m_max_mp != 0) {
				modifiers_text += TRANSLATE("Max. MP")END + " " + (m_max_mp > 0 ? "+" : "") + itos(m_max_mp) + "%$";
			}
			if (m_attack != 0) {
				modifiers_text += TRANSLATE("Attack")END + " " + (m_attack > 0 ? "+" : "") + itos(m_attack) + "%$";
			}
			if (m_defense != 0) {
				modifiers_text += TRANSLATE("Defense")END + " " + (m_defense > 0 ? "+" : "") + itos(m_defense) + "%$";
			}
			if (m_agility != 0) {
				modifiers_text += TRANSLATE("Agility")END + " " + (m_agility > 0 ? "+" : "") + itos(m_agility) + "%$";
			}
			if (m_luck != 0) {
				modifiers_text += TRANSLATE("Luck")END + " " + (m_luck > 0 ? "+" : "") + itos(m_luck) + "%$";
			}
			if (m_speed != 0) {
				modifiers_text += TRANSLATE("Speed")END + " " + (m_speed > 0 ? "+" : "") + itos(m_speed) + "%$";
			}
			if (m_strength != 0) {
				modifiers_text += TRANSLATE("Strength")END + " " + (m_strength > 0 ? "+" : "") + itos(m_strength) + "%$";
			}

			if (modifiers_text != "") {
				modifiers_text = "Modifiers:$" + modifiers_text;
				modifiers_label->set_text(modifiers_text);
			}
		}
	}

	int carrying = stats->inventory->get_total_weight();
	int capacity = stats->characteristics.get_modified_strength(stats) * 1000;

	carrying_label->set_text(noo.game_t->translate(56) + " " + Inventory::decimal_to_string(carrying));
	capacity_label->set_text(Inventory::decimal_to_string(capacity) + " " + noo.game_t->translate(54));

	if (carrying > capacity) {
		SDL_Colour red = { 255, 0, 0, 255 };
		weight_header->set_background_colour(red);
	}
	else {
		SDL_Colour transparent = { 0, 0, 0, 0 };
		weight_header->set_background_colour(transparent);
	}

	gui->layout();
}

void Items_GUI::handle_dropped_items()
{
	if (drop_radio && dropped_items) {
		if (dropped_items->items.size() > 0) {
			if (created_dropped_items) {
				Map_Entity *drop = new Map_Entity("item_drop");
				drop->set_brain(new Item_Drop_Brain(dropped_items, noo.get_play_time()));
				drop->load_sprite("item_drop");
				drop->set_position(noo.player->get_position());
				drop->set_solid(false);
				drop->set_low(true);
				noo.map->add_entity(drop);
			}
		}
		else if (created_dropped_items) {
			delete dropped_items;
			dropped_items = 0;
		}
	}
}

void Items_GUI::set_list()
{
	int hilight = -1;

	Inventory *inventory = stats->inventory;
	std::vector< std::vector<Item *> > &items = inventory->items;

	std::vector<std::string> item_list;

	indices.clear();

	for (size_t i = 0; i < items.size(); i++) {
		int count = items[i].size();
		if (count > 0) {
			if (items[i][0]->type == type) {
				std::string name = items[i][0]->name;
				item_list.push_back(itos(count) + " " + name);
				indices.push_back(i);
				if (type == Item::WEAPON && stats->weapon_index >= 0 && i == stats->weapon_index) {
					hilight = item_list.size() - 1;
				}
				else if (type == Item::ARMOUR && stats->armour_index >= 0 && i == stats->armour_index) {
					hilight = item_list.size() - 1;
				}
			}
		}
	}

	if (list == 0 && item_list.size() == 0) {
		TGUI_Widget *parent = new TGUI_Widget(0.4f, -1.0f);
		parent->set_parent(pad);
		Widget_Label *label = new Widget_Label("Inventory empty", -1);
		label->set_parent(parent);
	}
	else {
		bool exists = list != 0;
		if (exists == false) {
			list = new Widget_List(0.4f, -1.0f);
			list->set_parent(pad);
		}
		list->set_items(item_list);
		list->set_hilight(hilight, true);
	}
}

void Items_GUI::remove_item(int index, bool drop)
{
	int selected = list->get_selected();

	bool erased;

	Item *item = stats->inventory->items[index][0];
	stats->inventory->items[index].erase(stats->inventory->items[index].begin());
	if (stats->inventory->items[index].size() == 0) {
		stats->inventory->items.erase(stats->inventory->items.begin() + index);
		indices.erase(indices.begin() + selected);
		for (size_t i = selected; i < indices.size(); i++) {
			indices[i]--;
		}
		erased = true;
	}
	else {
		erased = false;
	}

	if (selected >= (int)indices.size()) {
		if (selected == 0) {
			gui->focus_something();
		}
		else {
			list->set_selected(selected-1);
		}
	}

	if (drop) {
		dropped_items->add(item);
	}
	else {
		delete item;
	}

	if (stats->weapon_index >= 0 && index == stats->weapon_index) {
		if (erased) {
			stats->weapon_index = -1;
			clear_highlights();
		}
	}
	else if (stats->armour_index >= 0 && index == stats->armour_index) {
		if (erased) {
			stats->armour_index = -1;
			clear_highlights();
		}
	}
	else if (erased && stats->weapon_index >= 0 && stats->weapon_index > index) {
		stats->weapon_index--;
	}
	else if (erased && stats->armour_index >= 0 && stats->armour_index > index) {
		stats->armour_index--;
	}

	set_list();
}

void Items_GUI::clear_highlights()
{
	if (list != 0) {
		int size = list->get_size();
		for (int i = 0; i < size; i++) {
			list->set_hilight(i, false);
		}
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
	if (((Yes_No_GUI::Callback_Data *)data)->choice == true) {
		cancel = true;
	}
}

void Buy_Sell_GUI::get_number_callback(void *data)
{
	number = (int64_t)data;
	got_number = true;
}

Buy_Sell_GUI::Buy_Sell_GUI(int seller_multiplier, Inventory *seller_inventory, std::vector<int> &seller_costs, bool is_storage, Callback done_callback, void *callback_data) :
	seller_inventory(seller_inventory),
	seller_costs(seller_costs),
	sell_count(0),
	buy_count(0),
	is_storage(is_storage),
	exit_menu(false),
	done_callback(done_callback),
	callback_data(callback_data),
	transaction(0),
	seller_multiplier(seller_multiplier)
{
	cancel = false; // statics, can't use initializer list
	got_number = false;

	stats = noo.map->get_entity(0)->get_stats();

	if (stats->weapon_index == -1) {
		start_weapon = 0;
	}
	else {
		start_weapon = stats->inventory->items[stats->weapon_index][0];
	}
	if (stats->armour_index == -1) {
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
		your_gold_label = new Widget_Label(noo.game_t->translate(152), 70);
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
		their_gold_label = new Widget_Label(noo.game_t->translate(64), 70);
	}
	their_gold_label->set_parent(their_column);
	their_list = new Widget_List(1.0f, -1.0f);
	their_list->set_highlight_colour(noo.colours[52]); // cyan
	their_list->set_break_line(true);
	their_list->set_parent(their_column);

	TGUI_Widget *button_column = new TGUI_Widget(0.2f, 0.75f);
	button_column->set_parent(pad);

	accept_button = new Widget_Text_Button(noo.game_t->translate(21), -1, -1);
	accept_button->set_padding_top(int(noo.font->get_height() + 2));
	accept_button->set_parent(button_column);
	their_list->set_right_widget(accept_button); // feels better like this

	done_button = new Widget_Text_Button(noo.game_t->translate(63), -1, -1);
	done_button->set_padding_top(int(noo.font->get_height() + 2));
	done_button->set_break_line(true);
	done_button->set_parent(button_column);

	TGUI_Widget *info_column = new TGUI_Widget(1.0f, 0.25f);
	info_column->set_parent(pad);

	cost_label = new Widget_Label("-", 100);
	cost_label->set_relative_position(0, 0);
	cost_label->set_parent(info_column);

	weight_label = new Widget_Label(noo.game_t->translate(140) + ": -", 100);
	weight_label->set_relative_position(0, int(noo.font->get_height() + 2));
	weight_label->set_parent(info_column);

	value_label = new Widget_Label(noo.game_t->translate(138) + ": -", 100);
	value_label->set_relative_position(0, 2 * int(noo.font->get_height() + 2));
	value_label->set_parent(info_column);

	condition_label = new Widget_Label(noo.game_t->translate(58) + ": -", 100);
	condition_label->set_relative_position(100, int(noo.font->get_height() + 2));
	condition_label->set_parent(info_column);

	properties_label = new Widget_Label("", 100);
	properties_label->set_relative_position(100, 2 * int(noo.font->get_height() + 2));
	properties_label->set_parent(info_column);

	gui = new TGUI(modal_main_widget, noo.screen_size.w, noo.screen_size.h);

	set_lists();

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
	if ((event->type == TGUI_KEY_DOWN && event->keyboard.code == noo.key_b2) ||
		(event->type== TGUI_JOY_DOWN && event->joystick.button == noo.joy_b2)) {

		noo.button_mml->play(false);
		maybe_confirm();
	}
	else {
		GUI::handle_event(event);
	}
}

void Buy_Sell_GUI::update()
{
	if (cancel) {
		return_items();
		if (done_callback) {
			done_callback(callback_data);
		}
		exit();
		return;
	}

	set_labels();

	int pressed;

	if (accept_button->pressed()) {
		if (get_your_gold() < 0) {
			Notification_GUI *gui = new Notification_GUI(noo.game_t->translate(149));
			gui->start();
			noo.guis.push_back(gui);
		}
		else if (get_their_gold() < 0) {
			Notification_GUI *gui = new Notification_GUI(noo.game_t->translate(120));
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
			stats->inventory->sort();
			seller_inventory->sort();

			if (is_storage == false) {
				seller_costs.clear();
				for (size_t i = 0; i < seller_inventory->items.size(); i++) {
					seller_costs.push_back(costs[seller_inventory->items[i][0]]);
				}
			}

			set_lists();

			cha_ching->play(false);

			stats->inventory->gold = get_your_gold();
			seller_inventory->gold = get_their_gold();

			transaction = 0;
		}
	}
	else if (done_button->pressed()) {
		maybe_confirm();
	}
	else if ((pressed = your_list->pressed()) >= 0) {
		int count = stats->inventory->items[pressed].size();
		if (count > 5) {
			got_number = false;
			getting_for_your_inventory = true;
			getting_for = pressed;
			Get_Number_GUI *gngui = new Get_Number_GUI(noo.game_t->translate(83), count+1, 0, get_number_callback);
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
			Get_Number_GUI *gngui = new Get_Number_GUI(noo.game_t->translate(83), count+1, 0, get_number_callback);
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

		weight_label->set_text(noo.game_t->translate(140) + ": " + Inventory::decimal_to_string(item->weight));

		if (item->type != Item::OTHER) {
			condition_label->set_text(noo.game_t->translate(58) + ": " + itos(condition) + "%");
		}
		else {
			condition_label->set_text(noo.game_t->translate(58) + ": -");
		}

		int value = item->get_value();

		value_label->set_text(noo.game_t->translate(138) + ": " + Inventory::decimal_to_string(value));

		std::string cost_text;
		if (is_storage == false) {
			if (is_their_list) {
				cost_text = noo.game_t->translate(51) + ": ";
			}
			else {
				cost_text = noo.game_t->translate(119) + ": ";
			}
			cost_text += Inventory::decimal_to_string(
				is_your_list ? get_cost(is_your_list) : get_cost(is_your_list) * seller_multiplier / 100);
		}
		else {
			cost_text = "-";
		}
		cost_label->set_text(cost_text);
	}
	else {
		cost_label->set_text("-");
		weight_label->set_text(noo.game_t->translate(140) + ": -");
		condition_label->set_text(noo.game_t->translate(58) + ": -");
		value_label->set_text(noo.game_t->translate(138) + ": -");
		properties_label->set_text("");
	}

	if (is_storage == false) {
		your_gold_label->set_text(itos(get_your_gold()) + " gold (You)");
		their_gold_label->set_text(itos(get_their_gold()) + " gold (Them)");
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
	bool is_original = false;

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
			is_original = true;
		}
		else if (!buying && index < buy_count) {
			buy_count--;
			is_original = true;
		}
	}
	else {
		inventory->items[index].erase(inventory->items[index].begin());
		if (buying && index < sell_count) {
			is_original = true;
		}
		else if (!buying && index < buy_count) {
			is_original = true;
		}
	}

	if (buying) {
		if (is_original) {
			transaction -= costs[item];
		}
		else {
			transaction -= costs[item] * seller_multiplier / 100;
		}
	}
	else {
		if (is_original) {
			transaction += costs[item] * seller_multiplier / 100;
		}
		else {
			transaction += costs[item];
		}
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
			inventory->sort(0, max);
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
				inventory->sort(0, max);
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

void Buy_Sell_GUI::maybe_confirm()
{
	if (buy_count > 0 || sell_count > 0) {
		Yes_No_GUI *gui;
		if (is_storage) {
			gui = new Yes_No_GUI(noo.game_t->translate(53), confirm_callback);
		}
		else {
			gui = new Yes_No_GUI(noo.game_t->translate(52), confirm_callback);
		}
		gui->start();
		noo.guis.push_back(gui);
	}
	else {
		if (done_callback) {
			done_callback(callback_data);
		}
		exit();
	}
}

int Buy_Sell_GUI::get_your_gold()
{
	int gold = stats->inventory->gold * 100;

	if (transaction < 0) {
		gold = int(floor((gold + transaction) / 100.0f));
	}
	else {
		gold = (gold + transaction) / 100;
	}

	return gold;
}

int Buy_Sell_GUI::get_their_gold()
{
	int gold = seller_inventory->gold * 100;

	gold = int(ceil((gold - transaction) / 100.0f));

	return gold;
}

//--

Multiple_Choice_GUI::Multiple_Choice_GUI(std::string caption, std::vector<std::string> choices, int escape_choice, Callback callback, void *callback_data) :
	callback(callback),
	callback_data(callback_data),
	exit_menu(false),
	escape_choice(escape_choice)
{
	int w = 150;
	int h;

	bool full;
	int num_lines;
	int width;

	// Get caption # lines
	if (caption == "") {
		num_lines = -1;
		h = int((noo.font->get_height() + 3) * 3);
	}
	else {
		noo.font->draw_wrapped(noo.white, caption, Point<float>(0, 0), w, (int)noo.font->get_height() + 3, -1, 0, 0, true, full, num_lines, width);
		h = int((noo.font->get_height() + 3) * (3 + num_lines + 1));
	}

	TGUI_Widget *modal_main_widget = new TGUI_Widget(1.0f, 1.0f);

	Widget_Window *window = new Widget_Window(w + 10, h + 10);
	window->set_9patch_image(noo.speech_window_image);
	window->set_float_right(true);
	window->set_float_bottom(true);
	window->set_padding_right(10);
	window->set_padding_bottom(10);
	window->set_parent(modal_main_widget);

	TGUI_Widget *pad = new TGUI_Widget(1.0f, 1.0f);
	pad->set_padding(5);
	pad->set_parent(window);

	if (caption != "") {
		caption_label = new Widget_Label(caption, w);
		caption_label->set_center_x(true);
		caption_label->set_parent(pad);
	}

	list = new Widget_List(1.0f, int(h - (num_lines + 1) * (noo.font->get_height() + 3)));
	list->set_selected_colour(noo.colours[13]); // blue
	list->set_items(choices);
	list->set_padding_top(int(h - (3 * (3 + noo.font->get_height()))));
	list->set_parent(pad);

	gui = new TGUI(modal_main_widget, noo.screen_size.w, noo.screen_size.h);

	gui->set_focus(list);
}

void Multiple_Choice_GUI::update()
{
	int pressed;
	if ((pressed = list->pressed()) >= 0) {
		Callback_Data data;
		data.choice = pressed;
		data.userdata = callback_data;
		callback((void *)&data);
		exit();
	}
}

void Multiple_Choice_GUI::handle_event(TGUI_Event *event)
{
	if (escape_choice >= 0 && ((event->type == TGUI_KEY_DOWN && event->keyboard.code == noo.key_b2) ||
		(event->type== TGUI_JOY_DOWN && event->joystick.button == noo.joy_b2))) {
		noo.button_mml->play(false);
		Callback_Data data;
		data.choice = escape_choice;
		data.userdata = callback_data;
		callback((void *)&data);
		exit();
	}
	if (escape_choice == -2 && ((event->type == TGUI_KEY_DOWN && event->keyboard.code == noo.key_b2) ||
		(event->type== TGUI_JOY_DOWN && event->joystick.button == noo.joy_b2))) {
		noo.button_mml->play(false);
		exit();
	}
	else {
		GUI::handle_event(event);
	}
}

//--

Quests_GUI::Quests_GUI() :
	exit_menu(false)
{
	for (size_t i = 0; i < quests.size(); i++) {
		quests[i]->update();
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

	Widget *title_container = new Widget(1.0f, int(noo.font->get_height() + 5));
	title_container->set_parent(pad);
	Widget_Label *title = new Widget_Label(noo.game_t->translate(116), 100);
	title->set_center_x(true);
	title->set_parent(title_container);

	std::vector<std::string> list_strings;

	for (size_t i = 0; i < quests.size(); i++) {
		Quest::Step *step = quests[i]->get_start_step();
		if (step->is_finished()) {
			indices.push_back(i);
			list_strings.push_back(step->get_description());
		}
	}

	list = new Widget_List(0.4f, -1.0f);
	list->set_parent(pad);
	list->set_items(list_strings);

	TGUI_Widget *w = new TGUI_Widget(0.4f, -1.0f);
	w->set_parent(pad);
	// Widget_Quest_Details takes all the space it can, so we need a container for it
	Widget_Quest_Details *qd = new Widget_Quest_Details(list, indices);
	qd->set_parent(w);

	done_button = new Widget_Text_Button(noo.game_t->translate(63), -1, -1);
	done_button->set_parent(pad);

	gui = new TGUI(modal_main_widget, noo.screen_size.w, noo.screen_size.h);

	gui->set_focus(list);
}

void Quests_GUI::handle_event(TGUI_Event *event)
{
	if ((event->type == TGUI_KEY_DOWN && event->keyboard.code == noo.key_b2) ||
		(event->type== TGUI_JOY_DOWN && event->joystick.button == noo.joy_b2)) {

		noo.button_mml->play(false);
		exit_menu = true;
	}
	else {
		GUI::handle_event(event);
	}
}

void Quests_GUI::update()
{
	if (done_button->pressed() || exit_menu) {
		exit();
		return;
	}
}

//--

Crafting_GUI::Crafting_GUI(Item::Type type, bool craft_spells) :
	type(type),
	crafting_spells(craft_spells),
	exit_menu(false)
{
	if (crafting_spells) {
		sample = new Sample("spell_craft.wav");
	}
	else if (type == Item::OTHER) {
		sample = new MML("craft_item.mml");
	}
	else {
		sample = new Sample("craft.wav");
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

	std::string crafting_type_s;

	if (crafting_spells) {
		crafting_type_s = "Spells";
	}
	else {
		switch (type) {
			case Item::WEAPON:
				crafting_type_s = "Weapons";
				break;
			case Item::ARMOUR:
				crafting_type_s = "Armour";
				break;
			case Item::OTHER:
				crafting_type_s = "Items";
				break;
		}
	}

	Widget *title_container = new Widget(1.0f, int(noo.font->get_height() + 5));
	title_container->set_parent(pad);
	Widget_Label *title = new Widget_Label(TRANSLATE("Crafting")END + " - " + crafting_type_s, 100);
	title->set_center_x(true);
	title->set_parent(title_container);

	std::vector<std::string> list_strings;

	std::vector<std::string> filenames = noo.cpa->get_all_filenames();

	for (size_t i = 0; i < filenames.size(); i++) {
		std::string fn = filenames[i];
		if (crafting_spells) {
			if (fn.substr(0, 7) == "spells/") {
				std::string name = fn.substr(7); // chop spells/
				name = name.substr(0, name.length()-4); // chop .xml
				
				Spell *spell = new Spell(name);

				item_ids.push_back(spell->get_id());
				list_strings.push_back(spell->get_name());

				std::vector<std::string> &ingredients = spell->get_ingredients();

				components.push_back(ingredients);

				std::vector<std::string> names;

				for (size_t i =  0; i < ingredients.size(); i++) {
					Item *item = new Item(ingredients[i]);
					names.push_back(item->name);
					delete item;
				}

				component_names.push_back(names);

				delete spell;

				can_disassemble.push_back(false);
			}
		}
		else {
			if (fn.substr(0, 6) == "items/") {
				std::string name = fn.substr(6); // chop item/
				name = name.substr(0, name.length()-4); // chop .xml
				Item *item = new Item(name);

				if (item->components != "" && type == item->type) {
					item_ids.push_back(item->id);
					list_strings.push_back(item->name);

					Tokenizer t(item->components, ':');
					std::string s;

					std::vector<std::string> v;
					std::vector<std::string> v2;

					while ((s = t.next()) != "") {
						v.push_back(s);

						Item *item = new Item(s);
						v2.push_back(item->name);
						delete item;
					}

					components.push_back(v);
					component_names.push_back(v2);

					can_disassemble.push_back(item->can_disassemble);
				}

				delete item;
			}
		}
	}

	list = new Widget_List(0.4f, -1.0f);
	list->set_parent(pad);
	list->set_items(list_strings);

	hilight_list_items();

	column2 = new TGUI_Widget(TGUI_Widget::FIT_Y, 0.4f);
	column2->set_padding_left(5);
	column2->set_padding_right(5);
	column2->set_parent(pad);

	column2_inner = 0;
	num_crafted_label = new Widget_Label("", -1);

	if (crafting_spells) {
		craft_button = new Widget_Text_Button(TRANSLATE("Brew")END, -1, -1);
	}
	else {
		teardown_button = new Widget_Text_Button(TRANSLATE("Disassemble")END, -1, -1);
		teardown_button->set_sound_enabled(false);
		craft_button = new Widget_Text_Button(TRANSLATE("Combine")END, -1, -1);
	}
	craft_button->set_sound_enabled(false);

	done_button = new Widget_Text_Button(noo.game_t->translate(63), -1, -1);
	done_button->set_parent(pad);

	gui = new TGUI(modal_main_widget, noo.screen_size.w, noo.screen_size.h);

	gui->set_focus(list);

	set_labels();
}

Crafting_GUI::~Crafting_GUI()
{
	delete sample;
}

void Crafting_GUI::handle_event(TGUI_Event *event)
{
	if ((event->type == TGUI_KEY_DOWN && event->keyboard.code == noo.key_b2) ||
		(event->type== TGUI_JOY_DOWN && event->joystick.button == noo.joy_b2)) {

		noo.button_mml->play(false);
		exit_menu = true;
	}
	else {
		GUI::handle_event(event);
	}
}

void Crafting_GUI::update()
{
	if (done_button->pressed() || exit_menu) {
		exit();
		return;
	}
	else if (crafting_spells == false && teardown_button->pressed()) {
		sample->play(false);

		SDL_Delay(1000);

		Stats *stats = noo.player->get_stats();

		std::string weapon_id = stats->weapon_index >= 0 ? stats->inventory->items[stats->weapon_index][0]->id : "";
		std::string armour_id = stats->armour_index >= 0 ? stats->inventory->items[stats->armour_index][0]->id : "";

		int selected = list->get_selected();
		std::string id = item_ids[selected];
		int index = stats->inventory->find(id);
		if (index >= 0) {
			uint16_t condition = stats->inventory->items[index][0]->condition;
			stats->inventory->remove(id);
			for (size_t i = 0; i < components[selected].size(); i++) {
				Item *item = new Item(components[selected][i]);
				item->condition = condition;
				stats->inventory->add(item);
			}
		}
		set_labels();

		if (teardown_button->get_accepts_focus() == false) {
			gui->set_focus(done_button);
		}

		verify_equipment(weapon_id, armour_id);
	}
	else if (craft_button->pressed()) {
		sample->play(false);

		SDL_Delay(1000);

		Stats *stats = noo.player->get_stats();

		int selected = list->get_selected();
		std::string id = item_ids[selected];
		uint32_t total_condition = 0;
		for (size_t i = 0; i < components[selected].size(); i++) {
			int index = stats->inventory->find(components[selected][i]);
			Item *item = stats->inventory->items[index][0];
			total_condition += item->condition;
			stats->inventory->remove(item);
		}

		if (crafting_spells) {
			Spell *spell = new Spell(id);
			stats->spells.push_back(spell);
		}
		else {
			Item *item = new Item(id);
			item->condition = total_condition / components[selected].size();
			stats->inventory->add(item);

			std::string weapon_id = stats->weapon_index >= 0 ? stats->inventory->items[stats->weapon_index][0]->id : "";
			std::string armour_id = stats->armour_index >= 0 ? stats->inventory->items[stats->armour_index][0]->id : "";

			verify_equipment(weapon_id, armour_id);
		}

		set_labels();
		
		if (craft_button->get_accepts_focus() == false) {
			gui->set_focus(done_button);
		}
	}
}

void Crafting_GUI::set_labels()
{
	if (column2_inner) {
		column2_inner->set_parent(0); // clear state
		delete column2_inner;
	}

	int selected = list->get_selected();

	column2_inner = new TGUI_Widget(1.0f, 1.0f);
	column2_inner->set_parent(column2);

	std::vector<bool> found;

	if (crafting_spells) {
		Stats *stats = noo.player->get_stats();
		bool have_spell = false;

		for (size_t i = 0; i < stats->spells.size(); i++) {
			if (stats->spells[i]->get_id() == item_ids[selected]) {
				have_spell = true;
				break;
			}
		}
		
		bool have_everything = have_components(selected, found);

		if (have_spell) {
			num_crafted_label->set_text(TRANSLATE("In spellbook")END);
			craft_button->set_enabled(false);
		}
		else {
			num_crafted_label->set_text(TRANSLATE("Not learned")END);

			if (have_everything) {
				craft_button->set_enabled(true);
			}
			else {
				craft_button->set_enabled(false);
			}
		}
		
		num_crafted_label->set_parent(column2_inner);
	}
	else {
		int num_crafted = have_crafted(selected);

		num_crafted_label->set_text(TRANSLATE("Have")END + ": " + itos(num_crafted));
		num_crafted_label->set_parent(column2_inner);

		teardown_button->set_enabled(num_crafted > 0 && can_disassemble[selected]);
		teardown_button->set_padding_top(2);
		teardown_button->set_break_line(true);
		teardown_button->set_parent(column2_inner);

		craft_button->set_enabled(have_components(selected, found));
	}

	Widget_Label *components_label = new Widget_Label(TRANSLATE("Needed")END + ":", 100);
	components_label->set_padding_top(5);
	if (crafting_spells) {
		components_label->set_break_line(true);
	}
	components_label->set_parent(column2_inner);

	std::vector<std::string> &needed = components[selected];
	std::vector<std::string> &needed_names = component_names[selected];

	for (size_t i = 0; i < needed.size(); i++) {
		Widget_Label *label = new Widget_Label(needed_names[i], 100);
		if (found[i] == true) {
			label->set_colour(noo.colours[40]); // green
		}
		if (i == 0) {
			label->set_padding_top(2);
		}
		label->set_break_line(true);
		label->set_parent(column2_inner);
	}

	craft_button->set_padding_top(2);
	craft_button->set_break_line(true);
	craft_button->set_parent(column2_inner);

	if (crafting_spells) {
		craft_button->set_right_widget(done_button);

		if (craft_button->get_accepts_focus()) {
			list->set_right_widget(craft_button);
			done_button->set_left_widget(craft_button);
		}
		else {
			list->set_right_widget(done_button);
			done_button->set_left_widget(list);
		}
	}
	else {
		teardown_button->set_right_widget(done_button);
		craft_button->set_right_widget(done_button);

		if (teardown_button->get_accepts_focus()) {
			list->set_right_widget(teardown_button);
			done_button->set_left_widget(teardown_button);
		}
		else if (craft_button->get_accepts_focus()) {
			list->set_right_widget(craft_button);
			done_button->set_left_widget(craft_button);
		}
		else {
			list->set_right_widget(done_button);
			done_button->set_left_widget(list);
		}
	}

	gui->layout();
}

void Crafting_GUI::hilight_list_items()
{
	for (size_t i = 0; i < item_ids.size(); i++) {
		std::vector<bool> found;
		if (have_crafted(i) > 0 || have_components(i, found)) {
			list->set_hilight(i, true);
		}
		else {
			list->set_hilight(i, false);
		}
	}
}

int Crafting_GUI::have_crafted(int index)
{
	Inventory *inv = noo.player->get_stats()->inventory;

	int count = 0;

	for (size_t i = 0; i < inv->items.size(); i++) {
		for (size_t j = 0; j < inv->items[i].size(); j++) {
			if (inv->items[i][j]->id == item_ids[index]) {
				count++;
			}
		}
	}

	return count;
}

bool Crafting_GUI::have_components(int index, std::vector<bool> &found)
{
	Inventory *inv = noo.player->get_stats()->inventory;

	std::vector<std::string> &needed = components[index];

	found.clear();
	for (size_t i = 0; i < needed.size(); i++) {
		found.push_back(false);
	}

	std::vector< std::pair<int, int> > used;

	for (size_t i = 0; i < inv->items.size(); i++) {
		for (size_t j = 0; j < inv->items[i].size(); j++) {
			std::pair<int, int> p;
			p.first = i;
			p.second = j;

			if (std::find(used.begin(), used.end(), p) != used.end()) {
				continue;
			}

			Item *item = inv->items[i][j];
			int found_index = -1;

			for (size_t k = 0; k < needed.size(); k++) {
				if (found[k] == false && item->id == needed[k]) {
					found_index = k;
					break;
				}
			}

			if (found_index >= 0) {
				found[found_index] = true;
				used.push_back(p);
			}
		}
	}

	bool all_found = true;

	for (size_t i = 0; i < found.size(); i++) {
		if (found[i] == false) {
			all_found = false;
			break;
		}
	}

	if (all_found) {
		return true;
	}

	return false;
}

void Crafting_GUI::verify_equipment(std::string weapon_id, std::string armour_id)
{
	// Check if we tore down or built with our weapon or armour

	Stats *stats = noo.player->get_stats();

	if (type == Item::WEAPON && stats->weapon_index >= 0) {
		int weapon_index = stats->weapon_index;
		if ((int)stats->inventory->items.size() <= weapon_index) {
			stats->weapon_index = -1;
		}
		else if (stats->inventory->items[weapon_index].size() == 0) {
			stats->weapon_index = -1;
		}
		else if (stats->inventory->items[weapon_index][0]->id != weapon_id) {
			stats->weapon_index = -1;
		}
	}
	if (type == Item::ARMOUR && stats->armour_index >= 0) {
		int armour_index = stats->armour_index;
		if ((int)stats->inventory->items.size() <= armour_index) {
			stats->armour_index = -1;
		}
		else if (stats->inventory->items[armour_index].size() == 0) {
			stats->armour_index = -1;
		}
		else if (stats->inventory->items[armour_index][0]->id != armour_id) {
			stats->armour_index = -1;
		}
	}
}

//--

Spells_GUI::Spells_GUI() :
	list(0),
	exit_menu(false)
{
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

	Widget *title_container = new Widget(1.0f, int(noo.font->get_height() + 5));
	title_container->set_parent(pad);
	Widget_Label *title = new Widget_Label(TRANSLATE("Spells")END, 100);
	title->set_center_x(true);
	title->set_parent(title_container);

	set_list();

	Widget *middle_column = new Widget(0.4f, 1.0f);
	middle_column->set_parent(pad);

	cost_label = new Widget_Label("", -1);
	cost_label->set_parent(middle_column);

	done_button = new Widget_Text_Button(noo.game_t->translate(63), -1, -1);
	done_button->set_parent(pad);

	gui = new TGUI(modal_main_widget, noo.screen_size.w, noo.screen_size.h);

	if (list) {
		gui->set_focus(list);
	}
	else {
		gui->set_focus(done_button);
	}
}

void Spells_GUI::handle_event(TGUI_Event *event)
{
	if ((event->type == TGUI_KEY_DOWN && event->keyboard.code == noo.key_b2) ||
		(event->type== TGUI_JOY_DOWN && event->joystick.button == noo.joy_b2)) {

		noo.button_mml->play(false);
		exit_menu = true;
	}
	else {
		GUI::handle_event(event);
	}
}

void Spells_GUI::update()
{
	if (done_button->pressed() || exit_menu) {
		exit();
		return;
	}

	if (list) {
		Spell *spell = noo.player->get_stats()->spells[list->get_selected()];
		cost_label->set_text(TRANSLATE("Cost")END + ": " + itos(spell->get_cost()));
	}
}

void Spells_GUI::set_list()
{
	std::vector<std::string> spell_list;

	for (size_t i = 0; i < stats->spells.size(); i++) {
		spell_list.push_back(stats->spells[i]->get_name());
	}

	if (spell_list.size() == 0) {
		TGUI_Widget *parent = new TGUI_Widget(0.4f, -1.0f);
		parent->set_parent(pad);
		Widget_Label *label = new Widget_Label("No spells", -1);
		label->set_parent(parent);
	}
	else {
		list = new Widget_List(0.4f, -1.0f);
		list->set_parent(pad);
		list->set_items(spell_list);
	}
}
