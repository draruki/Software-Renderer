#pragma once

#include <iostream>
#include "Math.h"

class vec3
{
public:
	float x;
	float y;
	float z;

	vec3(float x = 0, float y = 0, float z = 0);
	
	void zero();
	void set(const float x, const float y, const float z);
	void setAll(const float v);
	float magnitude();
	void normalize();
	vec3 normalized();

	void print();
};
vec3 operator+(vec3& a, vec3& b);
vec3 operator-(vec3& a, vec3& b);
vec3 operator*(vec3& v, float f);
vec3 operator*(float f, vec3& v);
vec3 operator/(vec3& v, float f);
void operator+=(vec3& a, vec3& b);
void operator-=(vec3& a, vec3& b);
void operator*=(vec3& v, float f);
void operator/=(vec3& v, float f);
vec3 operator-(vec3& v);

float dot(vec3& v, vec3& u);
vec3 cross(vec3& v, vec3& u);
vec3 rotate(vec3& v, vec3& axis, float angle_deg);
vec3 scale(vec3& v, vec3& axis, float amount);
vec3 scale(vec3& v, vec3& scale);

extern vec3 vec3_right;
extern vec3 vec3_up;
extern vec3 vec3_forward;

class vec2
{
public:
	float x;
	float y;

	vec2(float x = 0, float y = 0);
	
	void add(vec2& b);
	void sub(vec2& b);
	void mul(float v);
	void div(float v);
	float magnitude();
	void normalize();

	void print();
};
vec2 operator*(vec2& v, float f);
vec2 operator*(float f, vec2& v);
vec2 operator+(vec2& a, vec2& b);

class vec4
{
public:
	float x;
	float y;
	float z;
	float w;

	vec4(float x = 0, float y = 0, float z = 0, float w = 0);
	vec4(vec3& v, float w);
};
float dot4(vec4& v, vec4& u);