#include <Nooskewl_Engine/Nooskewl_Engine.h>

#include "brains.h"
#include "ml.h"
#include "ml_start.h"
#include "tweens.h"

int ML_start::callback_data;
Map_Entity *ML_start::coro;

void ML_start::callback(void *data)
{
	START_CALLBACK

	NEXT_STAGE {
		noo.player->set_path(noo.map->find_path(noo.player->get_position(), Point<int>(5, 19)), callback, data);
	}
	NEXT_STAGE {
		noo.player->set_direction(W);
		coro->set_direction(E);
		noo.map->add_speech("right,name=Coro|" + TRANSLATE("Well, look who decided to show up! You know it doesn't count as \"sleeping in\" if the sun goes down again... There's another name for it.")END);
		noo.map->add_speech("right,name=Eny|" + noo.game_t->translate(7));
		noo.map->add_speech("right,name=Coro|" + noo.game_t->translate(2), callback, data);
	}
	NEXT_STAGE {
		noo.player->set_path(noo.map->find_path(noo.player->get_position(), Point<int>(6, 21)), callback, data);
	}
	NEXT_STAGE {
		noo.player->set_direction(W);
		noo.player->set_sitting(true);
		noo.map->add_speech("right,name=Eny|" + noo.game_t->translate(10), callback, data);
	}
	NEXT_STAGE {
		noo.map->add_speech("right,name=Sunshine|" + noo.game_t->translate(16), callback, data);
	}
	NEXT_STAGE {
		noo.map->add_speech("right,name=Coro|" + noo.game_t->translate(6), callback, data);
	}
	NEXT_STAGE {
		noo.map->add_speech("right,name=Eny|" + noo.game_t->translate(19), callback, data);
	}
	NEXT_STAGE {
		noo.map->add_speech("right,name=Coro|" + noo.game_t->translate(14), callback, data);
	}
	NEXT_STAGE {
		noo.map->add_speech("right,name=Sunshine|" + noo.game_t->translate(5), callback, data);
	}
	NEXT_STAGE {
		noo.map->add_speech("right,name=Coro|" + noo.game_t->translate(4), callback, data);
	}
	NEXT_STAGE {
		noo.map->add_speech("right,name=Eny|" + noo.game_t->translate(11), callback, data);
	}
	NEXT_STAGE {
		noo.map->add_speech("right,name=Coro|" + noo.game_t->translate(20), callback, data);
	}
	NEXT_STAGE {
		noo.map->add_speech("right,name=Sunshine|" + noo.game_t->translate(12), callback, data);
	}
	NEXT_STAGE {
		noo.map->add_speech("right,name=Coro|" + noo.game_t->translate(18), callback, data);
	}
	NEXT_STAGE {
		noo.map->add_speech("right,name=Eny|" + noo.game_t->translate(8), callback, data);
	}
	NEXT_STAGE {
		noo.map->add_speech("right,name=Coro|" + noo.game_t->translate(9), callback, data);
	}
	NEXT_STAGE {
		noo.map->add_speech("right,name=Sunshine|" + noo.game_t->translate(1), callback, data);
	}
	NEXT_STAGE {
		noo.map->add_speech("right,name=Eny|" + noo.game_t->translate(3), callback, data);
	}
	NEXT_STAGE {
		noo.map->add_speech("right,name=Coro|" + noo.game_t->translate(13), callback, data);
	}
	NEXT_STAGE {
		noo.map->add_speech("+milestone=First Scene,right,name=Eny|" + noo.game_t->translate(15), callback, data);
	}
	NEXT_STAGE {
		coro->set_direction(S);
	}
}

ML_start::ML_start()
{
	callback_data = 0;
}

void ML_start::start()
{
	noo.play_music("town.mml");

	if (noo.last_map_name != "--LOADED--") {
		if (noo.check_milestone("First Scene") == false) {
			noo.player->set_position(Point<int>(11, 24));
			noo.player->set_direction(N);
			noo.map->add_speech("top,right,name=Eny|" + noo.game_t->translate(0), callback, &callback_data);
		}
		coro = new Map_Entity("coro");
		coro->set_brain(new Talk_Brain("coro"));
		coro->load_sprite("coro");
		coro->set_position(Point<int>(3, 19));
		coro->set_direction(S);
		coro->set_sitting(true);
		sunshine = new Map_Entity("sunshine");
		sunshine->set_brain(new Talk_Brain("sunshine"));
		sunshine->load_sprite("sunshine");
		sunshine->set_position(Point<int>(1, 21));
		sunshine->set_direction(E);
		sunshine->set_sitting(true);
		business_man = new Map_Entity("business_man");
		business_man->set_brain(new Talk_Brain("business_man"));
		business_man->load_sprite("business_man");
		business_man->set_position(Point<int>(16, 19));
		business_man->set_direction(E);
		legendary_warrior = new Map_Entity("legendary_warrior");
		legendary_warrior->set_brain(new Talk_Brain("legendary_warrior"));
		legendary_warrior->load_sprite("legendary_warrior");
		legendary_warrior->set_position(Point<int>(12, 15));
		legendary_warrior->set_direction(S);
		legendary_warrior->set_sitting(true);
		bartender = new Map_Entity("bartender");
		bartender->set_brain(new Talk_Brain("bartender"));
		bartender->load_sprite("bartender");
		bartender->set_position(Point<int>(19, 19));
		sitting_lady = new Map_Entity("sitting_lady");
		sitting_lady->set_brain(new Talk_Brain("sitting_lady"));
		sitting_lady->load_sprite("sitting_lady");
		sitting_lady->set_position(Point<int>(16, 13));
		sitting_lady->set_direction(E);
		sitting_lady->set_sitting(true);
		laughing_man = new Map_Entity("laughing_man");
		laughing_man->set_brain(new Animated_Brain("laughing_man"));
		laughing_man->load_sprite("laughing_man");
		laughing_man->set_position(Point<int>(8, 7));
		laughing_man->set_direction(S);
		laughing_man->set_sitting(true);
		drinker = new Map_Entity("drinker");
		drinker->set_brain(new Talk_Then_Animate_Brain("drinker"));
		drinker->load_sprite("drinker");
		drinker->set_position(Point<int>(14, 8));
		noo.map->add_entity(coro);
		noo.map->add_entity(sunshine);
		noo.map->add_entity(business_man);
		noo.map->add_entity(legendary_warrior);
		noo.map->add_entity(bartender);
		noo.map->add_entity(sitting_lady);
		noo.map->add_entity(laughing_man);
		noo.map->add_entity(drinker);
	}
}

void ML_start::end()
{
}

void ML_start::trigger(Map_Entity *entity)
{
	Point<int> collide_pos;
	if (entity->get_id() == 0 && entity->tiles_collide(Point<int>(18, 3), Size<int>(3, 1), collide_pos)) {
		entity->stop();
		noo.map->change_map("start_upper.map", Point<int>(22, 5), S);
	}
}

void ML_start::update()
{
}

void ML_start::activate(Map_Entity *activator, Map_Entity *activated)
{
}
