#include <Nooskewl_Engine/Nooskewl_Engine.h>

using namespace Nooskewl_Engine;

#include "quest.h"
#include "widgets.h"

Widget_Quest_Details::Widget_Quest_Details(Widget_List *list, std::vector<int> indices) :
	Widget(1.0f, 1.0f),
	list(list),
	indices(indices),
	offset(0)
{
	accepts_focus = true;

	current_quest = list->get_selected();

	minus = new Image("minus.tga");
	check = new Image("check.tga");
}

Widget_Quest_Details::~Widget_Quest_Details()
{
	delete minus;
	delete check;
}

void Widget_Quest_Details::handle_event(TGUI_Event *event)
{
	bool focussed = gui->get_focus() == this;

	if (focussed && event->type == TGUI_FOCUS) {
		if (event->focus.type == TGUI_FOCUS_UP) {
			if (offset > 0) {
				offset--;
			}
		}
		else if (event->focus.type == TGUI_FOCUS_DOWN) {
			offset++;
			// set in range in draw
		}
	}
}

void Widget_Quest_Details::draw()
{
	// Draw focus
	bool focussed = gui->get_focus() == this;

	if (focussed) {
		enable_focus_shader(true);
	}

	if (current_quest != list->get_selected()) {
		current_quest = list->get_selected();
		offset = 0;
	}

	Quest *quest = quests[indices[current_quest]];

	std::vector<Quest::Step *> steps = quest->get_steps();

	int steps_to_display = 0;
	bool display_unfinished_step;
	bool display_ellipses;

	for (size_t i = 0; i < steps.size(); i++) {
		if (steps[i]->is_finished()) {
			steps_to_display++;
		}
		else {
			break;
		}
	}

	if (steps_to_display < (int)steps.size()) {
		display_unfinished_step = true;
	}
	else {
		display_unfinished_step = false;
	}

	if (steps_to_display < (int)steps.size()-1) {
		display_ellipses = true;
	}
	else {
		display_ellipses = false;
	}

	int total_lines = 0;
	int line_height = int(noo.font->get_height() + 3);

	for (int i = 0; i < steps_to_display + (display_unfinished_step ? 1 : 0); i++) {
		bool full;
		int num_lines;
		int width;
		noo.font->draw_wrapped(noo.black, "  " + steps[i]->get_description(), Point<float>(0.0f, 0.0f), calculated_w - 10, line_height, -1, -1, 0, true, full, num_lines, width);

		total_lines += num_lines;
	}

	total_lines += steps_to_display - 1;

	if (display_unfinished_step) {
		total_lines++;
	}

	if (display_ellipses) {
		total_lines += 2;
	}

	int lines_on_screen = calculated_h / line_height;

	int max_offset = total_lines - lines_on_screen;
	if (max_offset < 0) {
		max_offset = 0;
	}

	if (offset > max_offset) {
		offset = max_offset;
	}

	int y = calculated_y - (offset * line_height);

	for (int i = 0; i < steps_to_display + (display_unfinished_step ? 1 : 0); i++) {
		bool full;
		int num_lines;
		int width;

		if (display_unfinished_step && i == steps_to_display) {
			minus->draw_single(Point<int>(calculated_x + 5, y));
		}
		else {
			check->draw_single(Point<int>(calculated_x + 5, y));
		}

		noo.font->draw_wrapped(noo.black, "  " + steps[i]->get_description(), Point<float>(calculated_x + 5.0f, (float)y), calculated_w - 10, line_height, -1, -1, 0, false, full, num_lines, width);

		y += line_height * (num_lines+1);
	}

	if (display_ellipses) {
		noo.font->draw(noo.black, "...", Point<float>(calculated_x + 5.0f, (float)y));
	}

	if (focussed) {
		enable_focus_shader(false);
	}

}
