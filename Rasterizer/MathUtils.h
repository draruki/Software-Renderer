#pragma once

#include <SDL.h>
#include <math.h>

#define PI 3.14159265359

#define max(a, b) (a > b? a : b)
#define min(a, b) (a < b? a : b)
#define clamp(v, a, b) (max(v, a) ==  v ? min(v, b) : a)

inline float rad_to_deg(float angleRad)
{
	return angleRad * 180 / PI;
}
inline float deg_to_rad(float angleDeg)
{
	return angleDeg * PI / 180;
}