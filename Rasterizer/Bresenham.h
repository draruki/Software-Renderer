#pragma once

#include <iostream>
#include "Color.h"

void setPixel(int x, int y, Uint32 color);

inline void swap(int* a,int* b)
{
	int A = *a;
	*a = *b;
	*b = A;
}

/*
	plots a straight line between two pixels.
	fast because it doesn't use float based m slope rounding for calculation like the DDA algorithm does.

	it can draw 4 types of lines on the right two quadrants (or right four octants).
	it then does an endpoint swap to draw the left four octants. so it can draw lines in all 8 octants (or four quadrants) of the cartesian coordinate system.

	only problem with this function is that the four plotting routines could be generalized a bit more. perhaps into two loops.
*/
inline void bresenham(int x0, int y0, int x1, int y1, Uint32 color)
{
	// swap endpoints for left octants
	if (x1 < x0)
	{
		swap(&x0, &x1);
		swap(&y0, &y1);
	}

	int dy = y1 - y0;
	int dx = x1 - x0;
	float m = dy * 1.f / dx;
	float eps = 0;

	// plot low slope
	if (m >= 0 && m <= 1)
	{
		for (int x = x0, y = y0; x <= x1; ++x)
		{
			setPixel(x, y, color);

			if (2 * (eps + dy) < dx)
			{
				eps = dy + eps;
			}
			else
			{
				eps = dy + eps - dx;
				++y;
			}
		}
	}
	// plot negative low slope
	if (m < 0 && m >= -1)
	{
		for (int x = x0, y = y0; x <= x1; ++x)
		{
			setPixel(x, y, color);
			
			if (2 * (eps + dy) > -dx)
			{
				eps = dy + eps;
			}
			else
			{
				eps = dy + eps + dx;
				--y;
			}
		}
	}
	// plot high slope
	else if (m > 1)
	{
		for (int y = y0, x = x0; y <= y1; ++y)
		{
			setPixel(x, y, color);
			
			if (2 * (eps + dx) < dy)
			{
				eps = dx + eps;
			}
			else
			{
				eps = dx + eps - dy;
				++x;
			}
		}
	}
	// plot negative high slope
	else if (m < -1)
	{
		for (int y = y0, x = x0; y >= y1; --y)
		{
			setPixel(x, y, color);
			
			if (2 * (eps + dx) < -dy)
			{
				eps = dx + eps;
			}
			else
			{
				eps = dx + eps + dy;
				++x;
			}
		}
	}
}