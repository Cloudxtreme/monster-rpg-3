#include <Nooskewl_Engine/Nooskewl_Engine.h>

#include "dll.h"

#include "ml_test.h"
#include "ml_test2.h"

#ifdef MONSTERRPG3_WINDOWS
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	noo.window_title = "Monster RPG 3";

	return TRUE;
}
#endif

Map_Logic *get_map_logic(Map *map)
{
	Map_Logic *ml = NULL;

	std::string map_name = map->get_map_name();

	if (map_name == "test.map") {
		ml = new ML_Test();
	}
	else if (map_name == "test2.map") {
		ml = new ML_Test2();
	}

	if (ml) {
		ml->start(map);
	}

	return ml;
}
