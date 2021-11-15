#include "Matrix.h"
#include "MathUtils.h"

mat3::mat3()
{
	clear();
}

mat3::~mat3()
{

}

float mat3::get(int i, int j)
{
	return data[i + j * 3];
}

void mat3::set(int i, int j, float v)
{
	data[i + j * 3] = v;
}

void mat3::clear()
{
	for (int i = 0; i < 9; ++i)
	{
		data[i] = 0;
	}
}

void mat3::identity()
{
	clear();
	set(0, 0, 1);
	set(1, 1, 1);
	set(2, 2, 1);
}

void mat3::transpose()
{
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			set(i, j, get(j, i));
		}
	}
}

vec3 mat3::getColumnVector(int j)
{
	return vec3(get(0, j), get(1, j), get(2, j));
}

vec3 mat3::getRowVector(int i)
{
	return vec3(get(i, 0),get(i, 1),get(i, 2));
}

void mat3::setColumnVector(int j, vec3& c)
{
	set(0, j, c.x);
	set(1, j, c.y);
	set(2, j, c.z);
}

void mat3::setColumnVectors(const vec3 &c1, const vec3 &c2, const vec3 &c3)
{
	set(0, 0, c1.x);
	set(1, 0, c1.y);
	set(2, 0, c1.z);
	set(0, 1, c2.x);
	set(1, 1, c2.y);
	set(2, 1, c2.z);
	set(0, 2, c3.x);
	set(1, 2, c3.y);
	set(2, 2, c3.z);
}

void mat3::setXRotation(const float angle)
{
	vec3 p(1, 0, 0);
	vec3 q(0, cos(angle), sin(angle));
	vec3 r(0, -sin(angle), cos(angle));
	setColumnVectors(p, q, r);
}

void mat3::setYRotation(const float angle)
{
	vec3 p(cos(angle), 0, -sin(angle));
	vec3 q(0, 1, 0);
	vec3 r(sin(angle), 0, cos(angle));
	setColumnVectors(p, q, r);
}

void mat3::setZRotation(const float angle)
{
	vec3 p(cos(angle), sin(angle), 0);
	vec3 q(-sin(angle), cos(angle), 0);
	vec3 r(0, 0, 1);
	setColumnVectors(p, q, r);
}

void mat3::setRotation(vec3& axis,float angle_deg)
{
	*this = rotate(*this, angle_deg, axis);
}

float mat3::determinant()
{
	return 1;
}
mat3 mat3::inverse()
{
	return *this;
}

void mat3::print()
{
	for(int i = 0; i < 3; ++i)
	{
		printf("[");
		for(int j = 0; j < 3; ++j)
		{
			printf(" %.3f ", get(i, j));
		}
		printf("]\n");
	}
}

mat3 rotate_x(float angle_deg)
{
	mat3 matrix;
	float angle = deg_to_rad(angle_deg);
	
	matrix.setColumnVectors(
			vec3(1, 0, 0),
			vec3(0, cos(angle), sin(angle)),
			vec3(0, -sin(angle), cos(angle))
		);

	return matrix;
}
mat3 rotate_y(float angle_deg)
{
	mat3 matrix;
	float angle = deg_to_rad(angle_deg);
	
	matrix.setColumnVectors(
			vec3(cos(angle), 0, -sin(angle)),
			vec3(0, 1, 0),
			vec3(sin(angle), 0, cos(angle))
		);

	return matrix;
}
mat3 rotate_z(float angle_deg)
{
	mat3 matrix;
	float angle = deg_to_rad(angle_deg);
	
	matrix.setColumnVectors(
			vec3(cos(angle), sin(angle), 0),
			vec3(-sin(angle), cos(angle), 0),
			vec3(0, 0, 1)
		);

	return matrix;
}

mat3 rotate(mat3& matrix, float angle_deg, vec3& axis)
{
	vec3 p = matrix.getColumnVector(0);
	vec3 q = matrix.getColumnVector(1);
	vec3 r = matrix.getColumnVector(2);

	p = rotate(p, axis, angle_deg);
	q = rotate(q, axis, angle_deg);
	r = rotate(r, axis, angle_deg);

	matrix.setColumnVectors(p, q, r);

	return matrix;
}
mat3 scale_matrix(vec3& scale)
{
	mat3 matrix;
	matrix.identity();

	vec3 p = matrix.getColumnVector(0);
	vec3 q = matrix.getColumnVector(1);
	vec3 r = matrix.getColumnVector(2);

	p *= scale.x;
	q *= scale.y;
	r *= scale.z;

	matrix.setColumnVectors(p, q, r);

	return matrix;
}
mat3 scale_matrix(mat3& matrix, vec3& scale)
{
	vec3 p = matrix.getColumnVector(0);
	vec3 q = matrix.getColumnVector(1);
	vec3 r = matrix.getColumnVector(2);

	p *= scale.x;
	q *= scale.y;
	r *= scale.z;

	matrix.setColumnVectors(p, q, r);

	return matrix;
}
mat3 scale_matrix(mat3& matrix, vec3& axis, float amount)
{
	vec3 p = matrix.getColumnVector(0);
	vec3 q = matrix.getColumnVector(1);
	vec3 r = matrix.getColumnVector(2);

	p = scale(p, axis, amount);
	q = scale(q, axis, amount);
	r = scale(r, axis, amount);

	matrix.setColumnVectors(p, q, r);

	return matrix;
}

vec3 operator*(mat3& m,vec3& v)
{
	vec3 result;
	result.x = dot(m.getRowVector(0), v);
	result.y = dot(m.getRowVector(1), v);
	result.z = dot(m.getRowVector(2), v);
	return result;
}

mat3 operator*(mat3& a, mat3& b)
{
	vec3 ar1 = a.getRowVector(0);
	vec3 ar2 = a.getRowVector(1);
	vec3 ar3 = a.getRowVector(2);
	
	vec3 bc1 = b.getColumnVector(0);
	vec3 bc2 = b.getColumnVector(1);
	vec3 bc3 = b.getColumnVector(2);
	
	mat3 result;
	
	result.set(0, 0, dot(ar1, bc1));
	result.set(0, 1, dot(ar1, bc2));
	result.set(0, 2, dot(ar1, bc3));
	result.set(1, 0, dot(ar2, bc1));
	result.set(1, 1, dot(ar2, bc2));
	result.set(1, 2, dot(ar2, bc3));
	result.set(2, 0, dot(ar3, bc1));
	result.set(2, 1, dot(ar3, bc2));
	result.set(2, 2, dot(ar3, bc3));

	return result;
}

mat4::mat4()
{
	clear();
}

mat4::~mat4()
{
}

float mat4::get(int i, int j)
{
	return array[i + j * 4];
}

void mat4::set(int i, int j, float v)
{
	array[i + j * 4] = v;
}

void mat4::clear()
{
	memset(array, 0, sizeof(array));
}

void mat4::transpose()
{
	mat4 old = *this;
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			set(i, j, old.get(j, i));
		}
	}
}

vec4 mat4::getColumnVector(int j)
{
	return vec4(get(0, j), get(1, j), get(2, j), get(3, j));
}
vec4 mat4::getRowVector(int i)
{
	return vec4(get(i, 0), get(i, 1), get(i, 2), get(i, 3));
}

void mat4::setColumnVector(int j, vec4& c)
{
	set(0, j, c.x);
	set(1, j, c.y);
	set(2, j, c.z);
	set(3, j, c.w);
}
void mat4::setColumnVectors(vec4& c1, vec4& c2, vec4& c3, vec4& c4)
{
	setColumnVector(0, c1);
	setColumnVector(1, c2);
	setColumnVector(2, c3);
	setColumnVector(3, c4);
}

void mat4::setRowVector(int i,vec4 & r)
{
	set(i, 0, r.x);
	set(i, 1, r.y);
	set(i, 2, r.z);
	set(i, 3, r.w);
}

void mat4::setXRotation(const float angle)
{
}

void mat4::setYRotation(const float angle)
{
}

void mat4::setZRotation(const float angle)
{
}

mat4 mat4::identity()
{
	static mat4 identityMatrix;
	identityMatrix.setColumnVectors
	(
		vec4(1, 0, 0, 0),
		vec4(0, 1, 0, 0),
		vec4(0, 0, 1, 0),
		vec4(0, 0, 0, 1)
	);

	return identityMatrix;
}

mat4 mat4::translation(float x, float y, float z)
{
	mat4 translationMatrix = mat4::identity();
	translationMatrix.setColumnVector(4, vec4(x, y, z, 1.0f));
	return translationMatrix;
}

void mat4::setLookAt(vec3& origin, vec3& up, vec3& target)
{
	identity();

	vec3 forward = (target - origin).normalized();
	vec3 right = cross(forward, up.normalized());
	vec3 newUp = cross(up.normalized(), right);
	setColumnVectors(vec4(right, 0), vec4(newUp, 0), vec4(forward, 0), vec4(origin, 1));
}

void mat4::print()
{
	for(int i = 0; i < 4; ++i)
	{
		printf("[");
		for(int j = 0; j < 4; ++j)
		{
			printf(" %.3f ", get(i, j));
		}
		printf("]\n");
	}
}

vec4 operator*(mat4& m, vec4& v)
{
	vec4 result;

	result.x = dot4(m.getRowVector(0), v);
	result.y = dot4(m.getRowVector(1), v);
	result.z = dot4(m.getRowVector(2), v);
	result.w = dot4(m.getRowVector(3), v);

	return result;
}

mat4 operator*(mat4& a, mat4& b)
{
	vec4 ar1 = a.getRowVector(0);
	vec4 ar2 = a.getRowVector(1);
	vec4 ar3 = a.getRowVector(2);
	vec4 ar4 = a.getRowVector(3);
	
	vec4 bc1 = b.getColumnVector(0);
	vec4 bc2 = b.getColumnVector(1);
	vec4 bc3 = b.getColumnVector(2);
	vec4 bc4 = b.getColumnVector(3);
	
	mat4 result;
	
	result.set(0, 0, dot4(ar1, bc1));
	result.set(0, 1, dot4(ar1, bc2));
	result.set(0, 2, dot4(ar1, bc3));
	result.set(0, 3, dot4(ar1, bc4));
	result.set(1, 0, dot4(ar2, bc1));
	result.set(1, 1, dot4(ar2, bc2));
	result.set(1, 2, dot4(ar2, bc3));
	result.set(1, 3, dot4(ar2, bc4));
	result.set(2, 0, dot4(ar3, bc1));
	result.set(2, 1, dot4(ar3, bc2));
	result.set(2, 2, dot4(ar3, bc3));
	result.set(2, 3, dot4(ar3, bc4));
	result.set(3, 0, dot4(ar4, bc1));
	result.set(3, 1, dot4(ar4, bc2));
	result.set(3, 2, dot4(ar4, bc3));
	result.set(3, 3, dot4(ar4, bc4));

	return result;
}

mat4 translate(mat4& matrix, vec3& translation)
{
	mat4 tm = mat4::identity();
	tm.setColumnVector(3, vec4(translation, 1));
	return tm * matrix;
}

mat4 rotate(mat4& matrix, float angle_deg, vec3& axis)
{
	mat4 rm = mat4::identity();
	float a = deg_to_rad(angle_deg);
	vec3 u = axis.normalized();
	rm.setRowVector(0, vec4(cos(a)+u.x*u.x*(1-cos(a)), u.x*u.y*(1-cos(a))-u.z*sin(a), u.x*u.z*(1-cos(a))+u.y*sin(a) ,0));
	rm.setRowVector(1, vec4(u.y*u.x*(1-cos(a))+u.z*sin(a), cos(a)+u.y*u.y*(1-cos(a)), (u.y*u.z*(1-cos(a)))-(u.x*sin(a)), 0));
	rm.setRowVector(2, vec4((u.z*u.x*(1-cos(a)))-(u.y*sin(a)), u.z*u.y*(1-cos(a))+u.x*sin(a), cos(a)+u.z*u.z*(1-cos(a)), 0));

	return rm * matrix;
}

mat4 scale(mat4& matrix, vec3& scale)
{
	mat4 sm = mat4::identity();

	sm.set(0, 0, scale.x);
	sm.set(1, 1, scale.y);
	sm.set(2, 2, scale.z);

	return sm * matrix;
}

mat4 orthographic(float left, float right, float bottom, float top, float near, float far)
{
	mat4 matrix = mat4::identity();

	int zNeg = 1;
	if (RIGHT_HANDED_COORDINATE_SYSTEM)
		zNeg = -1;

	matrix.setColumnVectors(
		vec4(2 / (right - left), 0, 0, 0),
		vec4(0, 2 / (top - bottom), 0, 0),
		vec4(0, 0, zNeg * 2 / (far - near), 0),
		vec4(-(right + left) / (right - left),  -(top + bottom) / (top - bottom), -(far + near) / (far - near), 1)
		);

	return matrix;
}

mat4 frustum(float left,float right,float bottom,float top,float near,float far)
{
	mat4 matrix;

	matrix.setColumnVectors(
		vec4((2 * near) / (right - left), 0, 0, 0),
		vec4(0, (2 * near) / (top - bottom), 0, 0),
		vec4((right + left) / (right - left), (top + bottom) / (top - bottom), -(far + near) / (far - near), -1),
		vec4(0, 0, (-2 * far * near) / (far - near), 0)
		);
	
	return matrix;
}

mat4 perspective(float fovy,float aspect,float near,float far)
{
	float top, bottom, left, right;

	top = near * tan(deg_to_rad(fovy)/2);
	bottom = -top;
	right = top * aspect;
	left = -right;

	return frustum(left, right, bottom, top, near, far);
}

mat4 inverse(mat4& matrix)
{
	return matrix;
}