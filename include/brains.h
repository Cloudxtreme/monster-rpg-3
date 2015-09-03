#ifndef BRAINS_H
#define BRAINS_H

#include <Nooskewl_Engine/Nooskewl_Engine.h>

using namespace Nooskewl_Engine;

bool start_brains();
void end_brains();

class Talk_Brain : public Brain {
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
	~Talk_Brain();

	bool activate(Map_Entity *activator);

	bool save(std::string &out);

protected:
	struct Talk {
		int milestone;
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

	void update();
	bool save(std::string &out);
};

class Talk_Then_Animate_Brain : public Talk_Brain {
public:
	static void speech_callback(void *data);
	static void animation_callback(void *data);

	Talk_Then_Animate_Brain(std::string name);
	
	bool activate(Map_Entity *activator);

	bool save(std::string &out);

private:
	bool animating;
};

class Door_Brain : public Brain {
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

class Item_Brain : public Brain {
public:
	Item_Brain(std::string item_name, int quantity, int milestone = -1);

	bool activate(Map_Entity *activator);

	bool save(std::string &out);

protected:
	std::string item_name;
	int quantity;
	int milestone;
};

class Item_Drop_Brain : public Brain {
public:
	static void callback(void *data);

	Item_Drop_Brain(Inventory *inventory);
	~Item_Drop_Brain();

	bool activate(Map_Entity *activator);

	bool save(std::string &out);

	Inventory *get_inventory();

private:
	Inventory *inventory;
};

class Shop_Brain : public Brain {
public:
	static void callback(void *data);

	Shop_Brain(std::string caption, std::string yes_option, std::string no_option, Inventory *inventory, std::vector<int> costs);
	~Shop_Brain();

	bool activate(Map_Entity *activator);

	bool save(std::string &out);

	Inventory *get_inventory();
	std::vector<int> &get_costs();

private:
	std::string caption;
	std::string yes_option;
	std::string no_option;

	Inventory *inventory;
	std::vector<int> costs;
};

class Growing_Brain : public Brain {
public:
	static const int STAGE_TIME = 60 * 5;

	Growing_Brain(std::string baby_item, std::string fresh_item, std::string rotten_item, int instantiation_time);

	bool activate(Map_Entity *activator);
	bool save(std::string &out);
	void update();

	int get_instantiation_time();
	void set_instantiation_time(int instantiation_time);

private:
	std::string baby_item;
	std::string fresh_item;
	std::string rotten_item;

	std::string item_name;

	int instantiation_time;
};

#endif // BRAINS_H
