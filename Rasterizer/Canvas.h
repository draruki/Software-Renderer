#pragma once

#include <SDL.h>

// z buffer precision
typedef Uint16 UintZ;
#define MAX_DEPTH_VALUE UINT16_MAX

class Canvas
{
public:
	Canvas();
	~Canvas();

	SDL_Surface* framebuffer;
	int width;
	int height;

	void initialize(int width, int height);
	Uint32 getPixel(int x,int y);
	void setPixel(int x,int y, Uint32 color);
	UintZ getDepth(int x, int y);
	void setDepth(int x, int y, UintZ z);
	void resetDepthBuffer();
	void randomizePixels();

private:
	UintZ* zbuffer;
	
	void createDepthBuffer();
	void destroy();
};