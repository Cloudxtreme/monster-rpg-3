#include <Nooskewl_Engine/Nooskewl_Engine.h>

#include "brains.h"

bool start_brains()
{
	if (Door_Brain::start() == 0) {
		return false;
	}

	return true;
}

void end_brains()
{
	Door_Brain::end();
}

void Talk_Brain::callback(void *data)
{
	Callback_Data *d = (Callback_Data *)data;

	d->entity->set_direction(d->direction);

	if (d->user_callback) {
		d->user_callback(data);
	}
}

Talk_Brain::Talk_Brain(std::string name, Callback callback, void *callback_data) :
	name(name),
	user_callback(callback),
	user_callback_data(callback_data)
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

void Talk_Brain::activate(Map_Entity *activator)
{
	std::string text = get_speech(activator, map_entity);
	if (text != "") {
		noo.map->add_speech(text, callback, &callback_data);
	}
}

bool Talk_Brain::compare_milestones(Talk *a, Talk *b)
{
	return a->milestone >= b->milestone;
}

std::string Talk_Brain::get_speech(Map_Entity *activator, Map_Entity *activated)
{
	for (size_t i = 0; i < sayings.size(); i++) {
		Talk *t = sayings[i];
		if (t->milestone < 0 || noo.check_milestone(t->milestone)) {
			callback_data.direction = activated->get_direction();
			callback_data.entity = activated;
			callback_data.user_callback = user_callback;
			callback_data.user_callback_data = user_callback_data;
			Point<int> offset = activator->get_position() - activated->get_position();
			if (offset.x < 0) {
				activated->set_direction(W);
			}
			else if (offset.x > 0) {
				activated->set_direction(E);
			}
			else if (offset.y < 0) {
				activated->set_direction(N);
			}
			else {
				activated->set_direction(S);
			}
			Point<float> entity_pos = (activated->get_position() + activated->get_offset()) * noo.tile_size + noo.map->get_offset();
			int pipe = t->text.find('|');
			if (pipe != std::string::npos) {
				std::string options = t->text.substr(0, pipe);
				Tokenizer tokenizer(options, ',');
				std::string s;
				bool x_set = false;
				bool y_set = false;
				while ((s = tokenizer.next()) != "") {
					if (s == "top" || s == "bottom") {
						y_set = true;
					}
					else if (s == "left" || s == "right") {
						x_set = true;
					}
				}
				if (x_set == false && entity_pos.x <= noo.screen_size.w/2) {
					t->text = std::string("right,") + t->text;
				}
				if (y_set == false && entity_pos.y > noo.screen_size.h/2) {
					t->text = std::string("top,") + t->text;
				}
			}
			return t->text;
		}
	}

	return "";
}

bool Talk_Brain::save(SDL_RWops *file)
{
	SDL_fprintf(file, "brain=talk_brain,%s\n", name.c_str());
	return true;
}

//--

Animated_Brain::Animated_Brain(std::string name) :
	Talk_Brain(name)
{
}

void Animated_Brain::update()
{
	if (map_entity == 0) {
		return;
	}

	Sprite *sprite = map_entity->get_sprite();

	if (!sprite->is_started()) {
		sprite->start();
	}

	if (name == "laughing_man") {
		Uint32 ticks = SDL_GetTicks() % 5000;
		if (ticks < 600) {
			if (sprite->get_animation() != "laugh") {
				sprite->reset();
			}
			sprite->set_animation("laugh");
		}
		else {
			sprite->set_animation("sit_s");
		}
	}
}

bool Animated_Brain::save(SDL_RWops *file)
{
	SDL_fprintf(file, "brain=animated_brain,%s\n", name.c_str());
	return true;
}

//--

void Talk_Then_Animate_Brain::animation_callback(void *data)
{
	Map_Entity *entity = (Map_Entity *)data;
	Sprite *sprite = entity->get_sprite();
	sprite->set_animation(sprite->get_previous_animation());
}

void Talk_Then_Animate_Brain::speech_callback(void *data)
{
	Talk_Brain::Callback_Data *d = (Talk_Brain::Callback_Data *)data;
	Map_Entity *entity = d->entity;
	std::string name = entity->get_name();
	Sprite *sprite = entity->get_sprite();

	if (name == "drinker") {
		sprite->set_animation("drinking", animation_callback, (void *)entity);
		sprite->reset();
	}
}

Talk_Then_Animate_Brain::Talk_Then_Animate_Brain(std::string name) :
	Talk_Brain(name, speech_callback)
{
}

bool Talk_Then_Animate_Brain::save(SDL_RWops *file)
{
	SDL_fprintf(file, "brain=talk_then_animate_brain,%s\n", name.c_str());
	return true;
}

//--

MML *Door_Brain::mml;

bool Door_Brain::start()
{
	mml = new MML("door.mml");
	return true;
}

void Door_Brain::end()
{
	delete mml;
}

void Door_Brain::activate(Map_Entity *activator)
{
	collide(activator);
}

void Door_Brain::collide(Map_Entity *collider)
{
	if (collider == noo.player) {
		mml->play(false);
		noo.map->schedule_destroy(map_entity);
	}
}

bool Door_Brain::save(SDL_RWops *file)
{
	SDL_fprintf(file, "brain=door_brain\n");
	return true;
}