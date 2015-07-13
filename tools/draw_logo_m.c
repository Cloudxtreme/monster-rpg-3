#include <allegro5/allegro.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

// width, height, thickness
void draw_logo_m(float W, float H, float T)
{
	ALLEGRO_COLOR fg = al_color_name("red");

	float verts[] = {
		T+T/2, H-T/2,
		W/4, T+T/2,
		W/2, H/2,
		W*3/4, T+T/2,
		W-T-T/2, H-T/2
	};

	#define COUNT 5

	al_draw_polyline(
		verts,
		sizeof(float)*2,
		COUNT,
		ALLEGRO_LINE_JOIN_MITER,
		ALLEGRO_LINE_CAP_NONE,
		fg,
		T,
		4
	);

	float cap1[] = {
		0, H-T/2,
		T*3, H-T/2
	};

	float cap2[] = {
		W, H-T/2,
		W-T*3, H-T/2
	};

	#define COUNT2 2

	al_draw_polyline(
		cap1,
		sizeof(float)*2,
		COUNT2,
		ALLEGRO_LINE_JOIN_NONE,
		ALLEGRO_LINE_CAP_NONE,
		fg,
		T,
		0
	);

	al_draw_polyline(
		cap2,
		sizeof(float)*2,
		COUNT2,
		ALLEGRO_LINE_JOIN_NONE,
		ALLEGRO_LINE_CAP_NONE,
		fg,
		T,
		0
	);
}

int main(void)
{
	int W = 64;
	int H = 64;
	int T = 8;

	al_init();
	al_init_image_addon();
	al_init_primitives_addon();

	ALLEGRO_BITMAP *bitmap = al_create_bitmap(W, H);
	al_set_target_bitmap(bitmap);
	al_clear_to_color(al_map_rgba(0, 0, 0, 0));

	draw_logo_m(W, H, T);

	al_save_bitmap("logo_m.png", bitmap);
}