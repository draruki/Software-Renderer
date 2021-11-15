#include <iostream>
#include "Canvas.h"
#include "Color.h"

Canvas::Canvas()
{
	framebuffer = NULL;
	zbuffer = NULL;
	width = 0;
	height = 0;
}

Canvas::~Canvas()
{
	destroy();
}

void Canvas::destroy()
{
	if (zbuffer != NULL)
	{
		delete[] zbuffer;
	}
	if (framebuffer != NULL)
	{
		SDL_FreeSurface(framebuffer);
	}

	zbuffer = NULL;
	framebuffer = NULL;
}

void Canvas::initialize(int width, int height)
{
	destroy();
	this->width = width;
	this->height = height;
    framebuffer = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
	createDepthBuffer();
	resetDepthBuffer();
}

Uint32 Canvas::getPixel(int x,int y)
{
	if (x < 0 || y < 0 || x >= width || y >= height)
	{
		return 0;
	}

	Uint32* pixels = (Uint32*) framebuffer->pixels;
	return pixels[y * framebuffer->w + x];
}

void Canvas::setPixel(int x,int y, Uint32 color)
{
	if (x < 0 || y < 0 || x >= width || y >= height)
	{
		return;
	}

	Uint32* pixels = (Uint32*) framebuffer->pixels;
	pixels[y * framebuffer->w + x] = color;
}

void Canvas::createDepthBuffer()
{
	if (zbuffer != NULL)
	{
		printf("reinitialized depth buffer\n");
		delete[] zbuffer;
	}

	zbuffer = new UintZ[width * height];
}

UintZ Canvas::getDepth(int x, int y)
{
	if (x < 0 || y < 0 || x >= width || y >= height)
	{
		return 0;
	}

	return zbuffer[y * framebuffer->w + x];
}

void Canvas::setDepth(int x, int y, UintZ z)
{
	if (x < 0 || y < 0 || x >= width || y >= height)
	{
		return;
	}

	zbuffer[y * framebuffer->w + x] = z;
}

void Canvas::resetDepthBuffer()
{
	memset(zbuffer, MAX_DEPTH_VALUE, sizeof(UintZ) * width * height);
}

void Canvas::randomizePixels()
{
	for (int i = 0; i < framebuffer->w; ++i)
		for(int j = 0; j < framebuffer->h; ++j)
			setPixel(i, j, randColor());
}