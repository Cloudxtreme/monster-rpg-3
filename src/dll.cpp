#include <Nooskewl_Engine/Nooskewl_Engine.h>

#include "dll.h"

#include "ml_start.h"

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

Map_Logic *get_map_logic(std::string map_name)
{
	Map_Logic *ml = NULL;

	if (map_name == "start.map") {
		ml = new ML_start();
	}

	return ml;
}