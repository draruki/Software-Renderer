#include <iostream>
#include "Texture.h"
#include "MathUtils.h"
#include "stb_image.h"
#define STB_IMAGE_IMPLEMENTATION

Texture::Texture()
{
	width = 0;
	height = 0;
	texels = NULL;
}

Texture::~Texture()
{
	if (texels != NULL)
	{
		delete[] texels;
	}
}

void Texture::create(int width, int height)
{
	if (texels != NULL)
	{
		delete[] texels;
	}

	texels = new Color[width * height];
	this->width = width;
	this->height = height;
}

void Texture::load(const char* filepath)
{
	int w, h, comp;
	unsigned char* pixels = stbi_load(filepath, &w, &h, &comp, 4);
	if (pixels == NULL)
	{
		return;
	}

	create(w, h);
	memcpy(texels, pixels, width * height * 4);

	delete pixels;
}

void Texture::generateRandom(int width, int height)
{
	create(width, height);

	for (int i = 0; i < width; ++i)
	{
		for (int j = 0; j < height; ++j)
		{
			setTexel(i, j, Color(randColor()));
		}
	}
}

Color Texture::sample(float u, float v)
{
	if (texels == NULL)
	{
		return Color(0, 0, 0, 255);
	}

	u = clamp(u, 0, 1);
	v = clamp(v, 0, 1);

	v = 1 - v;

	int x = floorf(u * (width-1));
	int y = floorf(v * (height-1));

	return getTexel(x, y);
}

Color Texture::getTexel(int x,int y)
{
	if (texels == NULL) return Color(0, 0, 0, 255);

	if (x < 0 || y < 0 || x >= width || y >= height) { return Color(0, 0, 0, 255); }
	
	return texels[y * width + x];
}

void Texture::setTexel(int x, int y, Color color)
{
	if (texels == NULL) return;

	if (x < 0 || y < 0 || x >= width || y >= height) return;
	
	texels[y * width + x] = color;
}