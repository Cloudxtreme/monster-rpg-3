#include <Nooskewl_Engine/engine.h>

#include "ml_start.h"
#include "tweens.h"

ML_start::ML_start() :
	sat(false),
	yes(0),
	pass(0)
{
}

void ML_start::start()
{
	if (noo.last_map_name == "") { // FIXME: not if loading a game, only starting a new one
		noo.player->set_position(Point<int>(11, 24));
		noo.player->set_direction(N);
		noo.map->add_speech("name=Eny,top|Ahhh, the pub. Nothing beats it. Except maybe the pub in Seaside... but although this one isn't as clean, the people are nice. Hey, there's someone I know!");
	}
	noo.play_music("town.mml");

	Map_Entity *coro = new Map_Entity(NULL);
	coro->load_sprite("coro");
	coro->set_position(Point<int>(3, 19));
	coro->set_direction(S);
	coro->set_sitting(true);
	Map_Entity *sunshine = new Map_Entity(NULL);
	sunshine->load_sprite("sunshine");
	sunshine->set_position(Point<int>(1, 21));
	sunshine->set_direction(E);
	sunshine->set_sitting(true);
	noo.map->add_entity(coro);
	noo.map->add_entity(sunshine);
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
	else if (entity->get_id() == 0 && entity->tiles_collide(Point<int>(8, 7), Size<int>(1, 1), collide_pos)) {
		if (sat == false) {
			int padding = noo.window_image->size.h / 3;
			noo.player->stop();
			noo.player->disable_input();
			MO3_Widget *main_widget = new MO3_Widget(1.0f, 1.0f);
			pass = new MO3_Text_Button("Pass", Size<int>(30, -1));
			pass->set_float_right(true);
			yes = new MO3_Text_Button("Yes", Size<int>(30, -1));
			MO3_Widget *button_container = new MO3_Widget(80, pass->get_height());
			button_container->set_padding_right(5);
			button_container->set_padding_top(noo.screen_size.h - 5 - button_container->get_height());
			button_container->set_float_right(true);
			button_container->set_parent(main_widget);
			pass->set_parent(button_container);
			yes->set_parent(button_container);
			MO3_Window *bubble = new MO3_Window(80, noo.font->get_height()+padding*2);
			bubble->set_padding_right(5);
			bubble->set_padding_top(button_container->get_padding_top()-padding-bubble->get_height());
			bubble->set_float_right(true);
			bubble->set_clear_float_x(true);
			bubble->set_parent(main_widget);
			MO3_Label *label = new MO3_Label("Sit down?", 70);
			label->set_padding(padding);
			label->set_centered_x(true);
			label->set_parent(bubble);
			noo.gui = new TGUI(main_widget, noo.screen_size.w, noo.screen_size.h);
			noo.gui->set_focus(yes);
			printf("pass=%p\n", pass);
			sat = true;
		}
	}
	else {
		sat = false;
	}
}

void ML_start::update()
{
	if (yes && yes->pressed()) {
		delete noo.gui;
		noo.gui = 0;
		yes = 0;
		pass = 0;
		noo.player->set_sitting(true);
		noo.player->enable_input();
	}
	else if (pass && pass->pressed()) {
		delete noo.gui;
		noo.gui = 0;
		yes = 0;
		pass = 0;
		noo.player->enable_input();
	}
}

void ML_start::activate(Map_Entity *activator, Map_Entity *activated)
{
}
