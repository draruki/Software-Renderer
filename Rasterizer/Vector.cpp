#include "Vector.h"
#include "MathUtils.h"

vec3::vec3(float x, float y, float z) : x(x), y(y), z(z) {}

float dot(vec3& v, vec3& u)
{
	return v.x*u.x + v.y*u.y + v.z*u.z;
}

float dot4(vec4& v,vec4& u)
{
	return v.x*u.x + v.y*u.y + v.z*u.z + v.w*u.w;
}

vec3 cross(vec3& v, vec3& u)
{
	return vec3
	(
		(v.y * u.z) - (u.y * v.z),
		(v.x * u.z) - (u.x * v.z),
		(v.x * u.y) - (u.x * v.y)
	);
}

vec3 vec3_right(1, 0, 0);
vec3 vec3_up(0, 1, 0);
vec3 vec3_forward(0, 0, 1);

vec3 rotate(vec3& v, vec3& axis, float angle_deg)
{
	float angle_rad = deg_to_rad(angle_deg);
	vec3 n = axis.normalized();
	vec3 vOr = dot(v, n) * n;
	vec3 vBottom = v - vOr;
	vec3 w = cross(n, vBottom);
	vec3 vPrimeBottom = cos(angle_rad) * vBottom + sin(angle_rad) * w;
	vec3 vPrime = vOr + vPrimeBottom;

	return vPrime;
}

vec3 scale(vec3& v, vec3& scale)
{
	return vec3(v.x * scale.x, v.y * scale.y, v.x * scale.z);
}

vec3 scale(vec3& v, vec3& axis, float amount)
{
	vec3 n;
	float k;
	vec3 vPrime;
	vec3 vOr;
	vec3 vBottom;
	vec3 vPrimeBottom;
	vec3 vPrimeOr;

	n = axis;
	k = amount;
	vOr = dot(v, n) * n;
	vBottom = v - vOr;
	vPrimeBottom = vBottom;
	vPrimeOr = k * vOr;
	vPrime = vPrimeBottom + vPrimeOr;

	return vPrime;
}

void vec3::set(const float x, const float y, const float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

void vec3::setAll(const float v)
{
	this->x = v;
	this->y = v;
	this->z = v;
}

void vec3::zero()
{
	set(0, 0, 0);
}

float vec3::magnitude()
{
	return sqrt(x*x + y*y + z*z);
}

void vec3::normalize()
{
	*this /= magnitude();
}

vec3 vec3::normalized()
{
	return (*this) / magnitude();
}

void vec3::print()
{
	printf("vec3(%.3f, %.3f, %.3f)\n", x, y, z);
}

vec3 operator+(vec3& a, vec3& b)
{
	return vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}

vec3 operator-(vec3 & a, vec3 & b)
{
	return vec3(a.x - b.x, a.y - b.y, a.z - b.z);
}

vec3 operator*(vec3& v, float f)
{
	return vec3(v.x * f, v.y * f, v.z * f);
}

vec3 operator*(float f, vec3& v)
{
	return v * f;
}

vec3 operator/(vec3& v, float f)
{
	if (f == 0) f = 1;
	return vec3(v.x / f, v.y / f, v.z / f);
}

void operator+=(vec3& a, vec3& b)
{
	a = a + b;
}

void operator-=(vec3& a, vec3& b)
{
	a = a - b;
}

void operator*=(vec3& v, float f)
{
	v = v * f;
}

void operator/=(vec3& v, float f)
{
	v = v / f;
}

vec3 operator-(vec3& v)
{
	vec3 nv = vec3(-v.x,-v.y,-v.z);

	if (nv.x == -0.f) nv.x = 0.f;
	if (nv.y == -0.f) nv.y = 0.f;
	if (nv.z == -0.f) nv.z = 0.f;

	return nv;
}

vec2::vec2(float x, float y) : x(x), y(y) {}

void vec2::add(vec2& b)
{
	x += b.x;
	y += b.y;
}

void vec2::sub(vec2& b)
{
	x -= b.x;
	y -= b.y;
}

void vec2::mul(float v)
{
	x *= v;
	y *= v;
}

void vec2::div(float v)
{
	x /= v;
	y /= v;
}

float vec2::magnitude()
{
	return sqrt(x*x + y*y);
}

void vec2::normalize()
{
	div(magnitude());
}

void vec2::print()
{
	std::cout << "vec2(" << x << ", " << y << ")" << std::endl;
}

vec2 operator*(vec2& v, float f)
{
	return vec2(v.x * f, v.y * f);
}

vec2 operator*(float f, vec2& v)
{
	return v * f;
}

vec2 operator+(vec2& a, vec2& b)
{
	return vec2(a.x + b.x, a.y + b.y);
}

vec4::vec4(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

vec4::vec4(vec3& v, float w)
{
	x = v.x;
	y = v.y;
	z = v.z;
	this->w = w;
}