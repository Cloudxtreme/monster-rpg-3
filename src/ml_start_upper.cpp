#include <Nooskewl_Engine/Nooskewl_Engine.h>

#include "brains.h"
#include "ml_start_upper.h"

void ML_start_upper::start()
{
	noo.play_music("town.mml");

	if (noo.last_map_name != "--LOADED--") {
		Map_Entity *door1 = new Map_Entity("door");
		Door_Brain *door1_brain = new Door_Brain();
		door1->set_brain(door1_brain);
		door1->load_sprite("door");
		door1->set_position(Point<int>(3, 7));
		door1->set_shadow_type(Map_Entity::SHADOW_TRANSLUCENT_COPY);
		door1->set_solid(false);

		Map_Entity *door2 = new Map_Entity("door");
		Door_Brain *door2_brain = new Door_Brain();
		door2->set_brain(door2_brain);
		door2->load_sprite("door");
		door2->set_position(Point<int>(9, 7));
		door2->set_shadow_type(Map_Entity::SHADOW_TRANSLUCENT_COPY);
		door2->set_solid(false);

		Map_Entity *door3 = new Map_Entity("door");
		Door_Brain *door3_brain = new Door_Brain();
		door3->set_brain(door3_brain);
		door3->load_sprite("door");
		door3->set_position(Point<int>(15, 7));
		door3->set_shadow_type(Map_Entity::SHADOW_TRANSLUCENT_COPY);
		door3->set_solid(false);

		Map_Entity *door4 = new Map_Entity("door");
		Door_Brain *door4_brain = new Door_Brain();
		door4->set_brain(door4_brain);
		door4->load_sprite("door");
		door4->set_position(Point<int>(3, 12));
		door4->set_shadow_type(Map_Entity::SHADOW_TRANSLUCENT_COPY);
		door4->set_solid(false);

		Map_Entity *door5 = new Map_Entity("door");
		Door_Brain *door5_brain = new Door_Brain();
		door5->set_brain(door5_brain);
		door5->load_sprite("door");
		door5->set_position(Point<int>(21, 12));
		door5->set_shadow_type(Map_Entity::SHADOW_TRANSLUCENT_COPY);
		door5->set_solid(false);

		noo.map->add_entity(door1);
		noo.map->add_entity(door2);
		noo.map->add_entity(door3);
		noo.map->add_entity(door4);
		noo.map->add_entity(door5);
	}
}

void ML_start_upper::trigger(Map_Entity *entity)
{
	Point<int> collide_pos;
	if (entity->get_id() == 0 && entity->tiles_collide(Point<int>(22, 3), Size<int>(2, 2), collide_pos)) {
		entity->stop();
		noo.map->change_map("start.map", Point<int>(19, 4), S);
	}
}