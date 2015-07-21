#include "Nooskewl_Engine/engine.h"
#include "Nooskewl_Engine/map.h"

#include "talk_brain.h"

Talk_Brain::Talk_Brain(std::string name, Callback callback, void *callback_data) :
	name(name),
	callback(callback)
{
	std::string text = noo.load_text("speech/" + name + ".utf8");

	int offset = 0;
	bool done = false;

	while (done == false) {
		while (text.c_str()[offset] != 0 && isspace(text.c_str()[offset])) {
			offset++;
		}
		if (text.c_str()[offset] == 0) {
			done = true;
		}

		int nlpos = text.find('\n', offset);
		int crpos = text.find('\r', offset);
		std::string line;

		if (nlpos == std::string::npos && crpos == std::string::npos) {
			done = true;
			line = text.substr(offset);
		}
		else {
			int less = MIN(nlpos, crpos);
			line = text.substr(offset, less-offset);
		}

		int pipepos = line.find('|');

		if (pipepos == std::string::npos) {
			printf("Malformated character speech file: %s.utf8\n", name.c_str());
		}
		else {
			std::string milestone_s = line.substr(0, pipepos);
			std::string speech = line.substr(pipepos+1);
			Talk *t = new Talk;
			t->milestone = atoi(milestone_s.c_str());
			t->text = speech;
			sayings.push_back(t);
		}

		offset = MAX(nlpos, crpos);
	}

	std::sort(sayings.begin(), sayings.end(), &Talk_Brain::compare_milestones);
}

Talk_Brain::~Talk_Brain()
{
	for (size_t i = 0; i < sayings.size(); i++) {
		delete sayings[i];
	}
}

void Talk_Brain::activate(Map_Entity *activator, Map_Entity *activated)
{
	for (size_t i = 0; i < sayings.size(); i++) {
		Talk *t = sayings[i];
		if (t->milestone < 0 || noo.check_milestone(t->milestone)) {
			noo.map->add_speech(t->text, callback, callback_data);
			return;
		}
	}
}

bool Talk_Brain::compare_milestones(Talk *a, Talk *b)
{
	return a->milestone >= b->milestone;
}