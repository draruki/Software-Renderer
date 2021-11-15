#pragma once

#include "Vector.h"

#define RIGHT_HANDED_COORDINATE_SYSTEM 0

class mat3
{
public:
	float data[9];

	mat3();
	~mat3();

	float get(int i, int j);
	void set(int i, int j, float v);
	void identity();
	void clear();
	void transpose();
	float determinant();
	mat3 inverse();
	vec3 getColumnVector(int j);
	vec3 getRowVector(int i);
	void setColumnVector(int j, vec3& c);
	void setColumnVectors(const vec3& c1, const vec3& c2, const vec3& c3);
	void setXRotation(const float angle);
	void setYRotation(const float angle);
	void setZRotation(const float angle);
	void setRotation(vec3& axis, float angle_deg);
	void print();

};
vec3 operator*(mat3& m, vec3& v);
mat3 operator*(mat3& a, mat3& b);
mat3 rotate_x(float angle_deg);
mat3 rotate_y(float angle_deg);
mat3 rotate_z(float angle_deg);
mat3 rotate(mat3& matrix, float angle_deg, vec3& axis);
mat3 scale_matrix(vec3& scale);
mat3 scale_matrix(mat3& matrix, vec3& scale);
mat3 scale_matrix(mat3& matrix, vec3& axis, float amount);

class mat4
{
public:
	float array[16];

	mat4();
	~mat4();

	float get(int i, int j);
	void set(int i, int j, float v);
	void clear();
	void transpose();
	vec4 getColumnVector(int j);
	vec4 getRowVector(int i);
	void setColumnVector(int j, vec4& c);
	void setColumnVectors(vec4& c1, vec4& c2, vec4& c3, vec4& c4);
	void setRowVector(int i, vec4& r);
	void setXRotation(const float angle);
	void setYRotation(const float angle);
	void setZRotation(const float angle);
	void setLookAt(vec3& origin, vec3& up, vec3& target);
	void print();
	static mat4 identity();
	static mat4 translation(float x, float y, float z);
};
vec4 operator*(mat4& m, vec4& v);
mat4 operator*(mat4& a, mat4& b);
mat4 rotate(mat4& matrix, float angle_deg, vec3& axis);
mat4 scale(mat4& matrix, vec3& scale);
mat4 translate(mat4& matrix, vec3& translation);
mat4 orthographic(float left, float right, float bottom, float top, float near, float far);
mat4 frustum(float left, float right, float bottom, float top, float near, float far);
mat4 perspective(float fovy, float aspect, float near, float far);
mat4 inverse(mat4& matrix);