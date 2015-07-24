#include <Nooskewl_Engine/internal.h>
#include <Nooskewl_Engine/map_entity.h>
#include <Nooskewl_Engine/sprite.h>

#include "brains.h"

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