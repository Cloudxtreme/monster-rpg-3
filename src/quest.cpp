#include <Nooskewl_Engine/Nooskewl_Engine.h>

#include "quest.h"

using namespace Nooskewl_Engine;

std::vector<Quest *> quests;

Quest::Step::Step(std::string description, std::string conditions) :
	description(description)
{
	Tokenizer t(conditions, ',');

	std::string token;

	finished = true;

	while ((token = t.next()) != "") {
		int ms = noo.milestone_name_to_number(token);
		this->conditions.push_back(ms);
		if (finished && noo.check_milestone(ms) == false) {
			finished = false;
		}
	}
}

std::string Quest::Step::get_description()
{
	return description;
}

bool Quest::Step::is_finished()
{
	return finished;
}

void Quest::Step::update()
{
	finished = true;

	for (size_t i = 0; i < conditions.size(); i++) {
		if (noo.check_milestone(conditions[i]) == false) {
			finished = false;
			break;
		}
	}
}

Quest::Quest(Step *start_step, std::vector<Step *> steps) :
	start_step(start_step),
	steps(steps)
{
	is_finished(); // ignore return value
}

Quest::~Quest()
{
	for (size_t i = 0; i < steps.size(); i++) {
		delete steps[i];
	}
}

Quest::Step *Quest::get_start_step()
{
	return start_step;
}

std::vector<Quest::Step *> Quest::get_steps()
{
	return steps;
}

bool Quest::is_finished()
{
	if (start_step->is_finished() == true) {
		finished = true;

		for (size_t i = 0; i < steps.size(); i++) {
			if (steps[i]->is_finished() == false) {
				finished = false;
				break;
			}
		}
	}
	else {
		finished = false;
	}
	
	return finished;
}

void Quest::update()
{
	bool started_finished = is_finished();

	start_step->update();

	for (size_t i = 0; i < steps.size(); i++) {
		steps[i]->update();
	}

	if (!started_finished) {
		if (is_finished()) {
			noo.add_notification(TRANSLATE("Quest Complete")END);
		}
	}
}

void init_quests()
{
	for (size_t i = 0; i < quests.size(); i++) {
		delete quests[i];
	}

	quests.clear();

	Quest::Step *step;
	std::vector<Quest::Step *> steps;

	steps.clear();
	step = new Quest::Step(TRANSLATE("Find Tiggy")END, "Get Pie");
	steps.push_back(step);
	step = new Quest::Step(TRANSLATE("Get a Chicken Pie for Tiggy")END, "Got Pie");
	steps.push_back(step);
	step = new Quest::Step(TRANSLATE("Recruit Tiggy")END, "");
	quests.push_back(new Quest(step, steps));

	steps.clear();
	step = new Quest::Step(TRANSLATE("Capture a Jumbo Rooster for Earl")END, "Got Jumbo Rooster");
	steps.push_back(step);
	step = new Quest::Step(TRANSLATE("Jumbo Rooster")END, "Rooster Quest");
	quests.push_back(new Quest(step, steps));
}
