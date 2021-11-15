#include "Color.h"
#include <iostream>
#include "MathUtils.h"

SDL_PixelFormat* WINDOW_PIXEL_FORMAT = NULL;

Color::Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}

Color::Color(Uint32 rgba)
{
	set(rgba);
}

Color::~Color()
{
}

Uint32 Color::get()
{
	return SDL_MapRGBA(WINDOW_PIXEL_FORMAT, r, g, b, a);
}

void Color::set(Uint32 color)
{
	SDL_GetRGBA(color, WINDOW_PIXEL_FORMAT, &r, &g, &b, &a);
}

Color Color::lerp(Color* a, Color* b, float v)
{
	Color c;
	
	c.r = a->r + (b->r - a->r) * v;
	c.g = a->g + (b->g - a->g) * v;
	c.b = a->b + (b->b - a->b) * v;
	c.a = a->a + (b->a - a->a) * v;

	return c;
}

void Color::print()
{
	printf("color is  r: %d  g: %d  b: %d  a: %d\n", r, g, b, a);
}

Color operator*(Color & a, float & v)
{
	return Color(a.r * v, a.g * v, a.b * v, a.a * v);
}
Color operator+(Color& a, Color& b)
{
	Uint8 rr = min(((int)a.r / 1) + ((int)b.r / 1), 255);
	Uint8 gg = min(((int)a.g / 1) + ((int)b.g / 1), 255);
	Uint8 bb = min(((int)a.b / 1) + ((int)b.b / 1), 255);
	Uint8 aa = min(((int)a.a / 1) + ((int)b.a / 1), 255);

	Color c = Color(rr, gg, bb, aa);

	return c;
}
Color operator*(Color& a, Color& b)
{
	Uint8 rr = min(((int)a.r / 2) * ((int)b.r / 2), 255);
	Uint8 gg = min(((int)a.g / 2) * ((int)b.g / 2), 255);
	Uint8 bb = min(((int)a.b / 2) * ((int)b.b / 2), 255);
	Uint8 aa = min(((int)a.a / 2) * ((int)b.a / 2), 255);

	Color c = Color(rr, gg, bb, aa);

	return c;
}