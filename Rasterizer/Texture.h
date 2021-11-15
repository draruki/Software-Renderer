#pragma once

#include "Color.h"

class Texture
{
public:
	Texture();
	~Texture();

	int width;
	int height;

	void generateRandom(int width, int height);
	void load(const char* filepath);
	Color sample(float u, float v);
	Color getTexel(int x, int y);
	void setTexel(int x, int y, Color color);

private:
	Color* texels;

	void create(int width, int height);
};