#include <Nooskewl_Engine/engine.h>
#include <Nooskewl_Engine/talk_brain.h>

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
		noo.map->add_speech("top,right,name=Coro|Well, look who decided to show up! You know it doesn't count as \"sleeping in\" if the sun goes down again... There's another name for it.");
		noo.map->add_speech("top,right,name=Eny|Oh yeah, what's that?");
		noo.map->add_speech("top,right,name=Coro|Didn't say I know. Now sit down, Sunshine and I have exciting news!", callback, data);
	}
	NEXT_STAGE {
		noo.player->set_path(noo.map->find_path(noo.player->get_position(), Point<int>(6, 21)), callback, data);
	}
	NEXT_STAGE {
		noo.player->set_direction(W);
		noo.player->set_sitting(true);
		noo.map->add_speech("top,right,name=Eny|Ok, so what is it?", callback, data);
	}
	NEXT_STAGE {
		noo.map->add_speech("top,right,name=Sunshine|Well we're not really sure, but...", callback, data);
	}
	NEXT_STAGE {
		noo.map->add_speech("top,right,name=Coro|Of course we're sure... We're sure! Now, you remember the stories I used to tell you when you were younger about the Sky Tower, right?", callback, data);
	}
	NEXT_STAGE {
		noo.map->add_speech("top,right,name=Eny|Who could forget? And besides, everyone knows about Sky Tower... and you can even see it if you walk outside!", callback, data);
	}
	NEXT_STAGE {
		noo.map->add_speech("top,right,name=Coro|The important part is that it has been locked as long as anyone can remember...", callback, data);
	}
	NEXT_STAGE {
		noo.map->add_speech("top,right,name=Sunshine|My grampy says he's been inside before.", callback, data);
	}
	NEXT_STAGE {
		noo.map->add_speech("top,right,name=Coro|Hogwash! Your grampy says he met the queen, too and we all know THAT'S not true. Anyway, the Tower has been locked as long as anyone can remember... but I've found out how to open it!", callback, data);
	}
	NEXT_STAGE {
		noo.map->add_speech("top,right,name=Eny|Sky Tower is full of gold and jewels and countless riches! Are we going to be rich?", callback, data);
	}
	NEXT_STAGE {
		noo.map->add_speech("top,right,name=Coro|Would you love your father more if he were rich? But yes, yes we are!", callback, data);
	}
	NEXT_STAGE {
		noo.map->add_speech("top,right,name=Sunshine|Tell her the catch.", callback, data);
	}
	NEXT_STAGE {
		noo.map->add_speech("top,right,name=Coro|Well... it's RUMOURED that there are some... monsters guarding the floors below the treasure. But that's not confirmed!", callback, data);
	}
	NEXT_STAGE {
		noo.map->add_speech("top,right,name=Eny|Oi! Monsters! I don't need more monsters in my life... count me out!", callback, data);
	}
	NEXT_STAGE {
		noo.map->add_speech("top,right,name=Coro|Ok, ok... I just thought you would like to go down in history as the woman who conquered the Sky Tower.", callback, data);
	}
	NEXT_STAGE {
		noo.map->add_speech("top,right,name=Sunshine|Check. Mate.", callback, data);
	}
	NEXT_STAGE {
		noo.map->add_speech("top,right,name=Eny|History, eh? Oh I love history... I've always wanted to go down in history! Count me in!", callback, data);
	}
	NEXT_STAGE {
		noo.map->add_speech("top,right,name=Coro|That's the spirit! Now go and rustle up that friend of yours, Tiggy... we will need some black magic where we're going!", callback, data);
	}
	NEXT_STAGE {
		noo.map->add_speech("top,right,name=Eny|Tiggy, yeah... that might be easier said than done, but I'll try...", callback, data);
	}
}

ML_start::ML_start()
{
}

void ML_start::start()
{
	noo.play_music("town.mml");

	if (noo.last_map_name == "") { // FIXME: not if loading a game, only starting a new one
		noo.player->set_position(Point<int>(11, 24));
		noo.player->set_direction(N);
		noo.map->add_speech("name=Eny,top,right|Ahhh, the pub. Nothing beats it. Except maybe the pub in Seaside... but although this one isn't as clean, the people are nice. Hey, there's someone I know!", callback, &callback_data);
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
		bartender->get_sprite()->start();
		sitting_lady = new Map_Entity("sitting_lady");
		sitting_lady->set_brain(new Talk_Brain("sitting_lady"));
		sitting_lady->load_sprite("sitting_lady");
		sitting_lady->set_position(Point<int>(16, 13));
		sitting_lady->set_direction(E);
		sitting_lady->set_sitting(true);
		noo.map->add_entity(coro);
		noo.map->add_entity(sunshine);
		noo.map->add_entity(business_man);
		noo.map->add_entity(legendary_warrior);
		noo.map->add_entity(bartender);
		noo.map->add_entity(sitting_lady);
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