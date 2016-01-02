#ifndef BRAINS_H
#define BRAINS_H

#include <Nooskewl_Engine/Nooskewl_Engine.h>

#include "brain_actions.h"

using namespace Nooskewl_Engine;

bool start_brains();
void end_brains();

class Monster_RPG_3_Brain : public Brain {
public:
	Monster_RPG_3_Brain() : can_pick_pocket(true) {} // even though true, must be the right type of brain

	bool can_pick_pocket;
	std::string caught_pick_pocket_text;
};

class Talk_Brain : public Monster_RPG_3_Brain {
public:
	static void callback(void *data);

	struct Callback_Data {
		Direction direction;
		Map_Entity *entity;
		Callback user_callback;
		void *user_callback_data;
	} callback_data;

	// The user callback is called with a Talk_Brain::Callback_Data which has the user data inside!
	Talk_Brain(std::string name, Callback callback = NULL, void *callback_data = NULL);
	virtual ~Talk_Brain();

	bool activate(Map_Entity *activator);

	bool save(std::string &out);

protected:
	struct Talk {
		bool all_milestones;
		int milestone;
		bool has_status;
		Stats::Status status;
		std::string text;
	};

	static bool compare_milestones(Talk *a, Talk *b);

	std::string get_speech(Map_Entity *activator, Map_Entity *activated);

	std::string name;

	// sorted: highest milestone first
	std::vector<Talk *> sayings;
	Callback user_callback;
	void *user_callback_data;

	bool talking;
};

class Animated_Brain : public Talk_Brain {
public:
	Animated_Brain(std::string name);
	virtual ~Animated_Brain();

	void update();
	bool save(std::string &out);
};

class Talk_Then_Animate_Brain : public Talk_Brain {
public:
	static void speech_callback(void *data);
	static void animation_callback(void *data);

	Talk_Then_Animate_Brain(std::string name);
	virtual ~Talk_Then_Animate_Brain();
	
	bool activate(Map_Entity *activator);

	bool save(std::string &out);

private:
	bool animating;
};

class Door_Brain : public Monster_RPG_3_Brain {
public:
	static bool start();
	static void end();

	Door_Brain(bool open);

	bool activate(Map_Entity *activator);
	void collide(Map_Entity *collider);
	void set_map_entity(Map_Entity *map_entity);

	bool save(std::string &out);

private:
	static MML *mml;

	void do_open();
	void do_close();

	bool open;
};

class Item_Brain : public Monster_RPG_3_Brain {
public:
	Item_Brain(std::string item_name, int quantity, int milestone = -1);

	bool activate(Map_Entity *activator);

	bool save(std::string &out);

protected:
	std::string item_name;
	int quantity;
	int milestone;
};

class Item_Drop_Brain : public Monster_RPG_3_Brain {
public:
	static void callback(void *data);

	Item_Drop_Brain(Inventory *inventory, int drop_time);
	~Item_Drop_Brain();

	bool activate(Map_Entity *activator);
	bool save(std::string &out);
	bool killme();

	Inventory *get_inventory();

private:
	Inventory *inventory;
	int drop_time;

	bool die;
};

class Base_Shop_Brain : public Monster_RPG_3_Brain {
public:
	static void buy_sell_callback(void *data);
	static void callback(void *data);

	Base_Shop_Brain(std::string caption, std::string yes_option, std::string no_option, int multiplier, std::vector<int> costs, Inventory *original_inventory, std::vector<int> original_costs, int last_visit);
	virtual ~Base_Shop_Brain();

	void init();

	bool activate(Map_Entity *activator);

	bool save(std::string &out);

	std::vector<int> &get_costs();

protected:
	void real_save(std::string brain_name, std::string &out);
	int get_multiplier();

	std::string caption;
	std::string yes_option;
	std::string no_option;

	int multiplier; // later divided by 100, so 110 would be 10%
	std::vector<int> costs;
	Inventory *original_inventory;
	std::vector<int> original_costs;
	int last_visit;
	Direction original_direction;
	bool direction_set;
};

class Shop_Brain : public Base_Shop_Brain, public Pick_Pocketable_Brain {
public:
	Shop_Brain(std::string caption, std::string yes_option, std::string no_option, int multiplier, std::vector<int> costs, Inventory *original_inventory, std::vector<int> original_costs, int last_visit);
	virtual ~Shop_Brain();
};

class Growing_Brain : public Monster_RPG_3_Brain {
public:
	static const int STAGE_TIME = 60 * 5;

	static void callback(void *data);

	Growing_Brain(std::string baby_item, std::string fresh_item, std::string rotten_item, int instantiation_time, int karma);

	bool activate(Map_Entity *activator);
	bool save(std::string &out);
	void update();

	int get_instantiation_time();
	void set_instantiation_time(int instantiation_time);

private:
	struct Callback_Data {
		Growing_Brain *brain;
		Map_Entity *activator;
	};

	bool give(Map_Entity *activator);

	std::string baby_item;
	std::string fresh_item;
	std::string rotten_item;

	std::string item_name;

	int instantiation_time;

	int karma;
	Callback_Data callback_data;
};

class Wander_Brain : public Talk_Brain {
public:
	// delay is in ticks and includes time to move between tiles
	Wander_Brain(std::string name, int max_distance, int delay, Point<int> start_pos, Callback callback = NULL, void *callback_data = NULL);
	~Wander_Brain();

	void update();

	bool save(std::string &out);

protected:
	int max_distance;
	int delay;
	Point<int> start_pos;
	int count;
};

class Base_No_Activate_Shop_Brain : public Base_Shop_Brain {
public:
	Base_No_Activate_Shop_Brain(std::string caption, std::string yes_option, std::string no_option, int multiplier, std::vector<int> costs, Inventory *original_inventory, std::vector<int> original_costs, int last_visit);
	virtual ~Base_No_Activate_Shop_Brain();

	void init();

	bool activate(Map_Entity *activator);

	bool save(std::string &out);

	void manual_activate();
};

class No_Activate_Shop_Brain : public Base_No_Activate_Shop_Brain, public Pick_Pocketable_Brain {
public:
	No_Activate_Shop_Brain(std::string caption, std::string yes_option, std::string no_option, int multiplier, std::vector<int> costs, Inventory *original_inventory, std::vector<int> original_costs, int last_visit);
	virtual ~No_Activate_Shop_Brain();
};

class NULL_Brain : public Monster_RPG_3_Brain {
public:
	NULL_Brain(std::string action);

	std::string get_action();

private:
	std::string action;
};

#endif // BRAINS_H
