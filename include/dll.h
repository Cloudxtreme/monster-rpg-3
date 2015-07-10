#ifndef DLL_H
#define DLL_H

#include "Nooskewl_Engine/map.h"
#include "Nooskewl_Engine/map_logic.h"

using namespace Nooskewl_Engine;

extern "C" {
	__declspec(dllexport) Map_Logic *get_map_logic(Map *map);
}

#endif // DLL_H