#include <Nooskewl_Engine/Nooskewl_Engine.h>

#include "brains.h"
#include "gui.h"
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
		noo.player->set_input_enabled(true);
	}
}

ML_start::ML_start()
{
	callback_data = 0;
}

void ML_start::start(bool been_here_before)
{
	noo.play_music("town.mml");

	if (!been_here_before) {
		noo.player->set_input_enabled(false);
		noo.player->set_position(Point<int>(11, 24));
		noo.player->set_direction(N);
		noo.map->add_speech("+milestone=Input Help,top,right,name=Eny|" + noo.game_t->translate(0), callback, &callback_data);

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

		Inventory *bartender_inventory = new Inventory();
		std::vector<int> bartender_costs;
		bartender_inventory->gold = 48;
		Buy_Sell_GUI::add_item(bartender_inventory, bartender_costs, "pickled_egg", 1, 12);
		Buy_Sell_GUI::add_item(bartender_inventory, bartender_costs, "beer", 1, 75);
		Buy_Sell_GUI::add_item(bartender_inventory, bartender_costs, "wine", 2, 40);
		bartender_inventory->sort();

		bartender = new Map_Entity("bartender");
		bartender->set_brain(new Shop_Brain(
			TRANSLATE("No loitering, kid! Either get drunk or get out!")END,
			TRANSLATE("Let me see what you've got.")END,
			TRANSLATE("I was just leaving...")END,
			bartender_inventory,
			bartender_costs
		)
		);
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

		int drinker_bottle_milestone = noo.milestone_name_to_number("Drinker's bottle");
		drinker_bottle = new Map_Entity("drinker_bottle");
		drinker_bottle->set_brain(new Item_Brain("bottle", 1, drinker_bottle_milestone));
		drinker_bottle->load_sprite("drinker_bottle");
		drinker_bottle->set_position(Point<int>(13, 8));
		drinker_bottle->set_z_add(1);
		noo.map->add_entity(drinker_bottle);

		Map_Entity *bottle1 = new Map_Entity("bottle");
		bottle1->set_brain(new Item_Brain("bottle", 1));
		bottle1->load_sprite("bottle_tipped");
		bottle1->set_position(Point<int>(1, 11));
		bottle1->set_solid(false);
		noo.map->add_entity(bottle1);

		Map_Entity *bottle2 = new Map_Entity("bottle");
		bottle2->set_brain(new Item_Brain("bottle", 1));
		bottle2->load_sprite("bottle_tipped");
		bottle2->set_position(Point<int>(14, 13));
		bottle2->set_solid(false);
		noo.map->add_entity(bottle2);

		Map_Entity *bottle3 = new Map_Entity("bottle");
		bottle3->set_brain(new Item_Brain("bottle", 1));
		bottle3->load_sprite("bottle_tipped");
		bottle3->set_position(Point<int>(17, 20));
		bottle3->set_z(noo.tile_size);
		bottle3->set_high(true);
		bottle3->set_solid(false);
		noo.map->add_entity(bottle3);

		Map_Entity *bottle4 = new Map_Entity("bottle");
		bottle4->set_brain(new Item_Brain("bottle", 1));
		bottle4->load_sprite("bottle");
		bottle4->set_position(Point<int>(17, 19));
		bottle4->set_z(noo.tile_size);
		bottle4->set_high(true);
		bottle4->set_solid(false);
		noo.map->add_entity(bottle4);

		Map_Entity *fish1 = new Map_Entity("fish");
		fish1->set_brain(new Item_Brain("fish", 1));
		fish1->load_sprite("fish");
		fish1->set_position(Point<int>(9, 7));
		fish1->set_z(noo.tile_size);
		fish1->set_high(true);
		fish1->set_solid(false);
		noo.map->add_entity(fish1);

		Map_Entity *fish2 = new Map_Entity("fish");
		fish2->set_brain(new Item_Brain("fish", 1));
		fish2->load_sprite("fish");
		fish2->set_position(Point<int>(17, 17));
		fish2->set_z(noo.tile_size);
		fish2->set_high(true);
		fish2->set_solid(false);
		noo.map->add_entity(fish2);

		Map_Entity *wineglass1 = new Map_Entity("wineglass");
		wineglass1->set_brain(new Item_Brain("wineglass", 1));
		wineglass1->load_sprite("wineglass");
		wineglass1->set_position(Point<int>(17, 18));
		wineglass1->set_z(noo.tile_size);
		wineglass1->set_high(true);
		wineglass1->set_solid(false);
		noo.map->add_entity(wineglass1);

		Map_Entity *wineglass2 = new Map_Entity("wineglass");
		wineglass2->set_brain(new Item_Brain("wineglass", 1));
		wineglass2->load_sprite("wineglass");
		wineglass2->set_position(Point<int>(3, 21));
		wineglass2->set_z(noo.tile_size);
		wineglass2->set_high(true);
		wineglass2->set_solid(false);
		noo.map->add_entity(wineglass2);

		Map_Entity *wineglass3 = new Map_Entity("wineglass");
		wineglass3->set_brain(new Item_Brain("wineglass", 1));
		wineglass3->load_sprite("wineglass_tipped");
		wineglass3->set_position(Point<int>(10, 16));
		wineglass3->set_z(noo.tile_size);
		wineglass3->set_high(true);
		wineglass3->set_solid(false);
		noo.map->add_entity(wineglass3);

		Map_Entity *wineglass4 = new Map_Entity("wineglass_broken");
		wineglass4->set_brain(new Item_Brain("wineglass_broken", 1));
		wineglass4->load_sprite("wineglass_broken");
		wineglass4->set_position(Point<int>(3, 13));
		wineglass4->set_z(noo.tile_size);
		wineglass4->set_high(true);
		wineglass4->set_solid(false);
		noo.map->add_entity(wineglass4);

		Map_Entity *mug1 = new Map_Entity("mug");
		mug1->set_brain(new Item_Brain("mug", 1));
		mug1->load_sprite("mug_left");
		mug1->set_position(Point<int>(4, 6));
		mug1->set_z(noo.tile_size);
		mug1->set_high(true);
		mug1->set_solid(false);
		noo.map->add_entity(mug1);

		Map_Entity *mug2 = new Map_Entity("mug");
		mug2->set_brain(new Item_Brain("mug", 1));
		mug2->load_sprite("mug_left");
		mug2->set_position(Point<int>(2, 21));
		mug2->set_z(noo.tile_size);
		mug2->set_high(true);
		mug2->set_solid(false);
		noo.map->add_entity(mug2);

		Map_Entity *mug3 = new Map_Entity("mug");
		mug3->set_brain(new Item_Brain("mug", 1));
		mug3->load_sprite("mug_left");
		mug3->set_position(Point<int>(9, 8));
		mug3->set_z(noo.tile_size);
		mug3->set_high(true);
		mug3->set_solid(false);
		noo.map->add_entity(mug3);

		Map_Entity *mug4 = new Map_Entity("mug");
		mug4->set_brain(new Item_Brain("mug", 1));
		mug4->load_sprite("mug_left");
		mug4->set_position(Point<int>(17, 16));
		mug4->set_z(noo.tile_size);
		mug4->set_high(true);
		mug4->set_solid(false);
		noo.map->add_entity(mug4);

		Map_Entity *monsterslayer = new Map_Entity("monsterslayer");
		monsterslayer->set_brain(new Item_Brain("monsterslayer", 1));
		monsterslayer->load_sprite("monsterslayer");
		monsterslayer->set_position(Point<int>(12, 17));
		monsterslayer->set_size(Size<int>(noo.tile_size*2, noo.tile_size*2));
		monsterslayer->set_high(true);
		monsterslayer->set_solid(false);
		noo.map->add_entity(monsterslayer);
	}
}

void ML_start::end()
{
}

void ML_start::trigger(Map_Entity *entity)
{
	Point<int> collide_pos;
	if (entity->get_id() == 0) {
		if (entity->tiles_collide(Point<int>(18, 3), Size<int>(3, 1), collide_pos)) {
			entity->stop();
			noo.map->change_map("start_upper.map", Point<int>(22, 5), S);
		}
		else if (entity->tiles_collide(Point<int>(10, 25), Size<int>(3, 1), collide_pos)) {
			entity->stop();
			noo.map->change_map("town.map", Point<int>(38, 11), S);
		}
	}
}

void ML_start::update()
{
}

void ML_start::activate(Map_Entity *activator, Map_Entity *activated)
{
}
