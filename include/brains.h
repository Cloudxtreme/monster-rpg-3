#ifndef BRAINS_H
#define BRAINS_H

#include <Nooskewl_Engine/main.h>
#include <Nooskewl_Engine/brain.h>

using namespace Nooskewl_Engine;

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
	void collide(Map_Entity *collider);

	bool save(SDL_RWops *file);
};

#endif // BRAINS_H