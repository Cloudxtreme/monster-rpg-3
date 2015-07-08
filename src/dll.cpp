#include "dll.h"

#include "ml_test.h"
#include "ml_test2.h"

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