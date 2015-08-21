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

	void activate(Map_Entity *activator);

	bool save(SDL_RWops *file);

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
	bool save(SDL_RWops *file);
};

class Talk_Then_Animate_Brain : public Talk_Brain {
public:
	static void speech_callback(void *data);
	static void animation_callback(void *data);

	Talk_Then_Animate_Brain(std::string name);

	bool save(SDL_RWops *file);
};

class Door_Brain : public Brain {
public:
	static bool start();
	static void end();

	Door_Brain(bool open);

	void activate(Map_Entity *activator);
	void collide(Map_Entity *collider);
	void set_map_entity(Map_Entity *map_entity);

	bool save(SDL_RWops *file);

private:
	static MML *mml;

	void do_open();
	void do_close();

	bool open;
};

class Item_Brain : public Brain {
public:
	Item_Brain(std::string item_name, int quantity, int milestone);

	void activate(Map_Entity *activator);

	bool save(SDL_RWops *file);

private:
	std::string item_name;
	int quantity;
	int milestone;
};

class Item_Drop_Brain : public Brain {
public:
	static void callback(void *data);

	Item_Drop_Brain(Inventory *inventory);
	~Item_Drop_Brain();

	void activate(Map_Entity *activator);

	bool save(SDL_RWops *file);

	Inventory *get_inventory();

private:
	Inventory *inventory;
};

#endif // BRAINS_H
