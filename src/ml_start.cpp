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
	noo.map->add_speech("Sunshine|Cans o' juice, plenty of em! And big bags of onion sandwhiches... plenty of those and also some large old bags of meats and meat products. Sounding good to you? Well there's plenty to go around, don't be shy! Oh and pot roasts... we got loads of those! Come on over for a pizza party some time!");
}

void ML_start::end()
{
}

void ML_start::trigger(Map_Entity *entity)
{
	Point<int> collide_pos;
	if (entity->get_id() == 0 && entity->tiles_collide(Point<int>(14, 14), Size<int>(1, 1), collide_pos)) {
		if (sat == false) {
			int padding = noo.window_image->size.w / 3;
			noo.player->stop();
			noo.player->disable_input();
			MO3_Widget *main_widget = new MO3_Widget(1.0f, 1.0f);
			pass = new MO3_Text_Button("Pass", Size<int>(50, -1));
			pass->set_padding(5, 20, noo.screen_size.h-20-pass->get_height(), 0);
			pass->set_float_right(true);
			pass->set_parent(main_widget);
			yes = new MO3_Text_Button("Yes", Size<int>(50, -1));
			yes->set_padding(0, 0, noo.screen_size.h-20-yes->get_height(), 0);
			yes->set_float_right(true);
			yes->set_parent(main_widget);
			MO3_Window *bubble = new MO3_Window(105, noo.font->get_height()+padding*2);
			bubble->set_padding(0, 20, pass->get_padding_top()-5-bubble->get_height(), 0);
			bubble->set_float_right(true);
			bubble->set_clear_floats(true);
			bubble->set_parent(main_widget);
			MO3_Label *label = new MO3_Label("Sit down?", 70);
			label->set_padding(padding, padding, padding, padding);
			label->set_centered_x(true);
			label->set_parent(bubble);
			noo.gui = new TGUI(main_widget, noo.screen_size.w, noo.screen_size.h);
			noo.gui->set_focus(yes);
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
