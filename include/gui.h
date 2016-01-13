#ifndef M3_GUI_H
#define M3_GUI_H

#include <Nooskewl_Engine/Nooskewl_Engine.h>

using namespace Nooskewl_Engine;

class Pause_GUI : public GUI {
public:
	static void callback(void *data);
	static void crafting_callback(void *data);

	Pause_GUI();

	void handle_event(TGUI_Event *event);
	void update();
	void update_background();

	bool fade_done(bool fading_in);

private:
	bool check_quit();
	void set_labels();

	static bool quitting;
	static bool quit;
	static bool showing_items;
	static bool set_the_labels;

	Widget_Text_Button *quit_button;
	Widget_Text_Button *save_button;
	Widget_Text_Button *resume_button;

	Stats *stats;

	Widget_Image *profile_image;
	Widget_Label *alignment_label;
	Widget_Label *sex_label;
	Widget_Label *gold_label;
	Widget_Label *hp_label;
	Widget_Label *mp_label;
	Widget_Label *experience_label;

	Widget_Image *weapon_image;
	Widget_Image *armour_image;

	Widget_Label *attack_label;
	Widget_Label *defense_label;
	Widget_Label *agility_label;
	Widget_Label *karma_label;
	Widget_Label *luck_label;
	Widget_Label *speed_label;
	Widget_Label *strength_label;
	Widget_Label *hunger_label;
	Widget_Label *thirst_label;
	Widget_Label *rest_label;
	Widget_Label *sobriety_label;

	Widget_Label *name;
	Widget_Label *status;
	Widget_Label *alignment;
	Widget_Label *sex;
	Widget_Label *gold;
	Widget_Label *hp;
	Widget_Label *mp;
	Widget_Label *experience;

	Widget_Label *weapon;
	Widget_Label *armour;

	Widget_Label *attack;
	Widget_Label *defense;
	Widget_Label *agility;
	Widget_Label *karma;
	Widget_Label *luck;
	Widget_Label *speed;
	Widget_Label *strength;
	Widget_Label *hunger;
	Widget_Label *thirst;
	Widget_Label *rest;
	Widget_Label *sobriety;

	Widget_Text_Button *items_button;
	Widget_Text_Button *weapons_button;
	Widget_Text_Button *armour_button;
	Widget_Text_Button *crafting_button;
	Widget_Text_Button *quests_button;

	Widget_Label *time_label;
	Widget_Sprite_Toggle *audio_toggle;

	bool exit_menu;
};

class Items_GUI : public GUI {
public:
	static void get_number_callback(void *data);
	static void discard_callback(void *data);

	Items_GUI(Item::Type type, Callback callback);

	void handle_event(TGUI_Event *event);
	void update();

private:
	static bool got_number;
	static int number;
	static bool discard_item;

	void set_labels();
	void handle_dropped_items();
	void set_list();
	void remove_item(int index, bool drop);
	void clear_highlights();

	Stats *stats;

	TGUI_Widget *pad;

	Widget_List *list;
	Widget_Text_Button *done_button;

	Widget_Label *action_label;
	Widget_Radio_Button *use_radio;
	Widget_Radio_Button *drop_radio;
	Widget_Radio_Button *discard_radio;

	Widget_Label *weight_label;
	Widget_Label *value_label;
	Widget_Label *condition_label;
	Widget_Label *properties_label;

	std::vector<int> indices; // index into inventory

	Item::Type type;
	bool exit_menu;

	Callback callback;

	bool created_dropped_items;
	Inventory *dropped_items;

	int dropping_index;

	Widget *weight_header;
	Widget_Label *carrying_label;
	Widget_Label *capacity_label;

	int discard_index;
};

class Buy_Sell_GUI : public GUI {
public:
	static void add_item(Inventory *inventory, std::vector<int> &costs, std::string name, int cost, int quantity);
	static void confirm_callback(void *data);
	static void get_number_callback(void *data);

	Buy_Sell_GUI(int seller_multiplier, Inventory *seller_inventory, std::vector<int> &seller_costs, bool is_storage, Callback done_callback = 0, void *callback_data = 0);
	virtual ~Buy_Sell_GUI();

	void handle_event(TGUI_Event *event);
	void update();

private:
	static bool cancel;
	static bool got_number;
	static int number;

	void set_labels();
	void set_list(Widget_List *list, Inventory *inventory, bool is_your_inventory);
	void set_lists();
	Item *remove_item(int index, bool buying);
	void add_item(Item *item, bool buying);
	void swap_item(int index, bool buying);
	void return_items();
	int get_cost(bool player);
	void clear_hilights();
	void set_hilights();
	void merge(Inventory *inventory, int count);
	void move_original(Inventory *inventory, int count, std::vector<Item *> &move_from, std::vector<Item *> &move_to);
	void maybe_confirm();
	int get_your_gold();
	int get_their_gold();

	Widget_Label *your_gold_label;
	Widget_List *your_list;
	Widget_Label *their_gold_label;
	Widget_List *their_list;

	Widget_Text_Button *accept_button;
	Widget_Text_Button *done_button;

	Widget_Label *cost_label;
	Widget_Label *weight_label;
	Widget_Label *value_label;
	Widget_Label *condition_label;
	Widget_Label *properties_label;

	Stats *stats;

	Inventory *seller_inventory;
	std::vector<int> &seller_costs;

	std::map<Item *, int> costs; // cost of everything, buyer and seller

	std::vector<Item *> your_original_inventory;
	std::vector<Item *> their_original_inventory;

	int sell_count;
	int buy_count;
	std::vector<int> swapped_out_costs;

	MML *cha_ching;

	bool getting_for_your_inventory;
	int getting_for;

	bool is_storage;

	bool exit_menu;

	Callback done_callback;
	void *callback_data;

	Item *start_weapon;
	Item *start_armour;

	int seller_multiplier;

	int transaction;
};

class Multiple_Choice_GUI : public GUI {
public:
	struct Callback_Data {
		int choice;
		void *userdata;
	};

	Multiple_Choice_GUI(std::string caption, std::vector<std::string> choices, int escape_choice, Callback callback, void *callback_data);

	void handle_event(TGUI_Event *event);
	void update();

private:
	Widget_Label *caption_label;
	Widget_List *list;

	Callback callback;
	void *callback_data;

	bool exit_menu;

	int escape_choice; // escape activates this choice. If it's -1, escape does nothing. If it's -2, escape dismisses the dialog with no action.
};

class Quests_GUI : public GUI {
public:
	Quests_GUI();

	void handle_event(TGUI_Event *event);
	void update();

private:
	Widget_List *list;

	Widget_Label *active_label_label;
	Widget_Label *active_label;
	Widget_Label *complete_label_label;
	Widget_Label *complete_label;
	Widget_Text_Button *done_button;

	std::vector<int> indices; // where list items fall into quests vector

	bool exit_menu;
};

class Crafting_GUI : public GUI {
public:
	Crafting_GUI(Item::Type type);

	void handle_event(TGUI_Event *event);
	void update();

private:
	void set_labels();
	void hilight_list_items();
	int have_crafted(int index);
	bool have_components(int index, std::vector<bool> &found);
	void verify_equipment(std::string weapon_id, std::string armour_id);

	Item::Type type;

	std::vector<std::string> item_ids;
	std::vector< std::vector<std::string> > components;
	std::vector< std::vector<std::string> > component_names;

	Widget_List *list;

	Widget_Text_Button *done_button;

	TGUI_Widget *column2;
	TGUI_Widget *column2_inner;
	Widget_Label *num_crafted_label;
	Widget *components_parent;
	Widget_Text_Button *teardown_button;
	Widget_Text_Button *craft_button;

	bool exit_menu;
};

#endif // M3_GUI_H
