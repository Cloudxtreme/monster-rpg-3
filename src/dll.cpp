#include <Nooskewl_Engine/Nooskewl_Engine.h>

#include "brains.h"
#include "dll.h"
#include "ml_start.h"
#include "ml_start_upper.h"

bool dll_start()
{
	noo.window_title = "Monster RPG 3";
	return start_brains();
}

void dll_end()
{
	end_brains();
}

Map_Logic *dll_get_map_logic(std::string map_name)
{
	Map_Logic *ml = NULL;

	if (map_name == "start.map") {
		ml = new ML_start();
	}
	else if (map_name == "start_upper.map") {
		ml = new ML_start_upper();
	}

	return ml;
}

Brain *dll_get_brain(std::string options)
{
	Tokenizer t(options, ',');

	std::string type = t.next();

	if (type == "talk_brain") {
		std::string name = t.next();
		return new Talk_Brain(name);
	}
	else if (type == "animated_brain") {
		std::string name = t.next();
		return new Animated_Brain(name);
	}
	else if (type == "talk_then_animate_brain") {
		std::string name = t.next();
		return new Talk_Then_Animate_Brain(name);
	}
	else if (type == "door_brain") {
		return new Door_Brain();
	}

	return 0;
}