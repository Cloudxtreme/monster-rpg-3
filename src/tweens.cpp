#include <Nooskewl_Engine/Nooskewl_Engine.h>

#include "tweens.h"

bool Move_Tween::update(Tween *tween)
{
	Uint32 now = SDL_GetTicks();
	Uint32 elapsed = now - tween->start_time;
	float elapsed_f = (float)elapsed / 1000.0f;

	int pixels_moved = int(elapsed_f * speed);

	Point<int> dist = destination - start_pos;
	Size<int> dist_pixels = Size<int>(dist.x*noo.tile_size, dist.y*noo.tile_size);

	if (dist_pixels.length() <= pixels_moved) {
		entity->set_position(destination);
		entity->set_offset(Point<float>(0.0f, 0.0f));
		return false;
	}

	float angle = dist_pixels.angle();

	Point<int> new_pos = Point<int>(int(cos(angle) * pixels_moved), int(sin(angle) * pixels_moved)) + start_pos * noo.tile_size;
	Point<int> new_tile = new_pos / 8;
	Point<int> offset_pix = new_pos - (new_tile * noo.tile_size);
	Point<float> offset = offset_pix;
	offset /= (float)noo.tile_size;

	entity->set_position(new_tile);
	entity->set_offset(offset);

	return true;
}