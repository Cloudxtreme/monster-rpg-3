#include <Nooskewl_Engine/engine.h>

#include "ml_start.h"
#include "tweens.h"

ML_start::ML_start() :
	sat(false),
	yes(0),
	pass(0)
{
}

void ML_start::start(Map *map)
{
}

void ML_start::end(Map *map)
{
}

void ML_start::trigger(Map *map, Map_Entity *entity)
{
	Point<int> collide_pos;
	if (entity->get_id() == 0 && entity->tiles_collide(Point<int>(14, 14), Size<int>(1, 1), collide_pos)) {
		if (sat == false) {
			noo.player->disable_input();
			main_widget = new SS_Widget(1.0f, 1.0f);
			pass = new SS_Text_Button("Pass", Size<int>(40, -1));
			pass->set_padding(10, 20, 100, 0);
			pass->set_float_right(true);
			pass->set_parent(main_widget);
			yes = new SS_Text_Button("Yes", Size<int>(40, -1));
			yes->set_padding(0, 0, 100, 0);
			yes->set_float_right(true);
			yes->set_parent(main_widget);
			noo.gui = new TGUI(main_widget, noo.screen_w, noo.screen_h);
			noo.gui->set_focus(yes);
			sat = true;
		}
	}
	else {
		sat = false;
	}
}

void ML_start::update(Map *map)
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

void ML_start::activate(Map *map, Map_Entity *activator, Map_Entity *activated)
{
}
