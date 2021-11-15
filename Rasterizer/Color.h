#pragma once

#include <SDL.h>
#include <iostream>

extern SDL_PixelFormat* WINDOW_PIXEL_FORMAT;

class Color
{
public:
	Uint8 r;
	Uint8 g;
	Uint8 b;
	Uint8 a;
	
	Color(Uint8 r = 255, Uint8 g = 255, Uint8 b = 255, Uint8 a = 255);
	Color(Uint32 rgba);
	~Color();

	Uint32 get();
	void set(Uint32 color);
	static Color lerp(Color* a, Color* b, float v);
	void print();
};
Color operator*(Color& a, float& v);
Color operator+(Color& a, Color& b);
Color operator*(Color& a, Color& b);

inline Uint32 randColor()
{
	Uint32 randomColor = 0;
	randomColor = randomColor | (Uint32) rand();
	randomColor = randomColor << 8;
	randomColor = randomColor | (Uint32) rand();
	randomColor = randomColor << 8;
	randomColor = randomColor | (Uint32) rand();
	randomColor = randomColor << 8;
	randomColor = randomColor | (Uint32) rand();
	
	return randomColor;
}