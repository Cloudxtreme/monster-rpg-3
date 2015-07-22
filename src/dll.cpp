#include <Nooskewl_Engine/Nooskewl_Engine.h>

#include "brains.h"
#include "dll.h"
#include "ml_start.h"
#include "ml_start_upper.h"

#ifdef MONSTERRPG3_WINDOWS
static bool started = false;

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	if (started == false) {
		started = true;

		noo.window_title = "Monster RPG 3";
	}

	return TRUE;
}
#endif

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

	if (type == "animated_brain") {
		std::string name = t.next();
		return new Animated_Brain(name);
	}

	return 0;
}