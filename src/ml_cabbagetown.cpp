#include <Nooskewl_Engine/Nooskewl_Engine.h>

#include "brains.h"
#include "gui.h"
#include "ml.h"
#include "ml_cabbagetown.h"
#include "monster-rpg-3.h"
#include "tweens.h"

const int num_cabbages = 68;

static void add_cabbage(int x, int y)
{
	Map_Entity *cabbage = new Map_Entity("cabbage");
	cabbage->load_sprite("cabbage");
	cabbage->set_brain(new Growing_Brain("baby_cabbage", "cabbage", "rotten_cabbage", -1, 0));
	cabbage->set_position(Point<int>(x, y));
	cabbage->set_low(true);
	noo.map->add_entity(cabbage);
}

static void add_red_cabbage(int x, int y)
{
	Map_Entity *cabbage = new Map_Entity("red_cabbage");
	cabbage->load_sprite("red_cabbage");
	cabbage->set_brain(new Growing_Brain("baby_cabbage", "red_cabbage", "rotten_cabbage", -1, 250));
	cabbage->set_position(Point<int>(x, y));
	cabbage->set_low(true);
	noo.map->add_entity(cabbage);
}

ML_cabbagetown::ML_cabbagetown(int last_visited_time)
{
	int now = noo.get_play_time();
	int diff = now - last_visited_time;
	cabbages_to_regrow = int((diff / 1800.0f) * (num_cabbages/2));
}

void ML_cabbagetown::start_audio()
{
	noo.play_music("cabbagetown.mml");
}

void ML_cabbagetown::start(bool been_here_before)
{
	if (!been_here_before) {
		for (int y = 17; y <= 17; y++) {
			for (int x = 27; x <= 32; x++) {
				add_cabbage(x, y);
			}
		}
		for (int y = 13; y <= 16; y++) {
			for (int x = 32; x <= 32; x++) {
				add_cabbage(x, y);
			}
		}
		for (int y = 13; y <= 17; y++) {
			for (int x = 36; x <= 36; x++) {
				add_cabbage(x, y);
			}
		}
		for (int y = 13; y <= 17; y++) {
			for (int x = 41; x <= 41; x++) {
				add_cabbage(x, y);
			}
		}
		for (int y = 17; y <= 17; y++) {
			for (int x = 37; x <= 40; x++) {
				add_cabbage(x, y);
			}
		}
		for (int y = 17; y <= 17; y++) {
			for (int x = 46; x <= 50; x++) {
				add_cabbage(x, y);
			}
		}
		for (int y = 13; y <= 17; y++) {
			for (int x = 45; x <= 45; x++) {
				add_cabbage(x, y);
			}
		}
		for (int y = 26; y <= 26; y++) {
			for (int x = 27; x <= 32; x++) {
				add_cabbage(x, y);
			}
		}
		for (int y = 27; y <= 30; y++) {
			for (int x = 32; x <= 32; x++) {
				add_cabbage(x, y);
			}
		}
		for (int y = 26; y <= 30; y++) {
			for (int x = 36; x <= 36; x++) {
				add_cabbage(x, y);
			}
		}
		for (int y = 26; y <= 30; y++) {
			for (int x = 41; x <= 41; x++) {
				add_cabbage(x, y);
			}
		}
		for (int y = 26; y <= 26; y++) {
			for (int x = 37; x <= 40; x++) {
				add_cabbage(x, y);
			}
		}
		for (int y = 26; y <= 30; y++) {
			for (int x = 45; x <= 45; x++) {
				add_cabbage(x, y);
			}
		}
		for (int y = 26; y <= 26; y++) {
			for (int x = 46; x <= 50; x++) {
				add_cabbage(x, y);
			}
		}

		// red cabbages
		for (int y = 36; y <= 37; y++) {
			for (int x = 56; x <= 60; x++) {
				add_red_cabbage(x, y);
			}
		}

		// people
		Map_Entity *alfred = new Map_Entity("alfred");
		alfred->load_sprite("alfred");
		alfred->set_position(Point<int>(29, 10));
		alfred->set_direction(S);
		alfred->set_sitting(true);
		noo.map->add_entity(alfred);

		Map_Entity *little_old_lady = new Map_Entity("little_old_lady");
		little_old_lady->load_sprite("little_old_lady");
		little_old_lady->set_position(Point<int>(34, 24));
		little_old_lady->set_brain(new Wander_Brain("little_old_lady", 7, 120, little_old_lady->get_position()));
		little_old_lady->set_direction(S);
		little_old_lady->set_speed(0.045f);
		noo.map->add_entity(little_old_lady);

		Map_Entity *chicken1 = new Map_Entity("chicken");
		chicken1->load_sprite("chicken");
		chicken1->set_position(Point<int>(30, 36));
		chicken1->set_brain(new Wander_Brain("chicken", 5, 60, chicken1->get_position()));
		noo.map->add_entity(chicken1);

		Map_Entity *chicken2 = new Map_Entity("chicken");
		chicken2->load_sprite("chicken");
		chicken2->set_position(Point<int>(33, 37));
		chicken2->set_brain(new Wander_Brain("chicken", 5, 80, chicken2->get_position()));
		noo.map->add_entity(chicken2);

		Map_Entity *chicken3 = new Map_Entity("chicken");
		chicken3->load_sprite("chicken");
		chicken3->set_position(Point<int>(32, 40));
		chicken3->set_brain(new Wander_Brain("chicken", 5, 100, chicken3->get_position()));
		noo.map->add_entity(chicken3);

		Map_Entity *horse1 = new Map_Entity("horse");
		horse1->load_sprite("horse");
		horse1->set_position(Point<int>(32, 46));
		horse1->set_brain(new Talk_Brain("horse"));
		horse1->set_size(Size<int>(noo.tile_size*3, noo.tile_size));
		horse1->set_direction(E);
		horse1->set_should_face_activator(false);
		noo.map->add_entity(horse1);

		Map_Entity *horse2 = new Map_Entity("horse");
		horse2->load_sprite("horse");
		horse2->set_position(Point<int>(48, 48));
		horse2->set_brain(new Talk_Brain("horse"));
		horse2->set_size(Size<int>(noo.tile_size*3, noo.tile_size));
		horse2->set_direction(W);
		horse2->set_should_face_activator(false);
		noo.map->add_entity(horse2);

		Map_Entity *earl = new Map_Entity("earl");
		earl->load_sprite("earl");
		earl->set_position(Point<int>(38, 36));
		earl->set_brain(new Talk_Brain("earl"));
		earl->set_direction(S);
		noo.map->add_entity(earl);
	}
}

void ML_cabbagetown::end()
{
}

void ML_cabbagetown::trigger(Map_Entity *entity)
{
	Point<int> collide_pos;
	if (entity->get_id() == 0 && entity->tiles_collide(Point<int>(38, 10), Size<int>(2, 1), collide_pos)) {
		entity->stop();
		noo.map->change_map("start.map", Point<int>(11, 24), N);
	}
}

void ML_cabbagetown::update()
{
}

struct Apple_Callback_Data {
	int ms1;
	int ms2;
	bool ms1_complete;
	bool ms2_complete;
	Map_Entity *alfred;
};

static Apple_Callback_Data acd;

static void face_alfred_south(void *data)
{
	Map_Entity *alfred = static_cast<Map_Entity *>(data);
	alfred->set_direction(S);
}

static void apple_callback(void *data)
{
	Multiple_Choice_GUI::Callback_Data *d = static_cast<Multiple_Choice_GUI::Callback_Data *>(data);
	Apple_Callback_Data *acd = static_cast<Apple_Callback_Data *>(d->userdata);
	if (d->choice == 0) { // yes
		Stats *stats = noo.player->get_stats();
		stats->inventory->remove("apple");

		if (acd->ms1_complete) {
			noo.set_milestone(acd->ms2, true);
			give_item(noo.player, "sleeping_bag", 1, -1);
		}
		else {
			noo.set_milestone(acd->ms1, true);
		}
	}
	face_alfred_south(acd->alfred);
}

void ML_cabbagetown::activate(Map_Entity *activator, Map_Entity *activated)
{
	if (activator == noo.player && activated->get_name() == "alfred") {
		activated->set_direction(get_facing_direction(activator, activated));

		acd.ms1 = noo.milestone_name_to_number("alfred apple 1");
		acd.ms2 = noo.milestone_name_to_number("alfred apple 2");
		acd.ms1_complete = noo.check_milestone(acd.ms1);
		acd.ms2_complete = noo.check_milestone(acd.ms2);
		acd.alfred = activated;

		Stats *stats = noo.player->get_stats();
		int apple_index = stats->inventory->find("apple");

		std::vector<std::string> choices;
		choices.push_back(TRANSLATE("Yes")END);
		choices.push_back(TRANSLATE("No")END);

		if (acd.ms2_complete) {
			noo.map->add_speech("name=Alfred|" + TRANSLATE("I'm not starving.")END, face_alfred_south, activated);
		}
		else if (acd.ms1_complete) {
			if (apple_index < 0) {
				noo.map->add_speech("name=Alfred|" + TRANSLATE("Hey hey! Got any apples? I love apples.")END, face_alfred_south, activated);
			}
			else {
				Multiple_Choice_GUI *gui = new Multiple_Choice_GUI(TRANSLATE("Got another apple? I'll make it worth it.")END, choices, apple_callback, &acd);
				gui->start();
				noo.guis.push_back(gui);
			}
		}
		else {
			if (apple_index < 0) {
				noo.map->add_speech("name=Alfred|" + TRANSLATE("Hey hey! Got any apples? I love apples.")END, face_alfred_south, activated);
			}
			else {
				Multiple_Choice_GUI *gui = new Multiple_Choice_GUI(TRANSLATE("Oh! Ohhh! Can I have that apple?")END, choices, apple_callback, &acd);
				gui->start();
				noo.guis.push_back(gui);
			}
		}
	}
}
