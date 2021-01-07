#include "pch.h"
#include "vec3f.h"
#include <math.h>
#include <stdlib.h>

#define M_PI 3.14159265358979323846

vec3f::vec3f()
{
}


vec3f::~vec3f()
{
}


vec3f::vec3f(float component1, float component2, float component3) {
	x = component1;
	y = component2;
	z = component3;
}


vec3f vec3f::sum(vec3f other) {
	return vec3f(x + other.x, y + other.y, z + other.y);
}

vec3f vec3f::scaledBy(float number) {
	return vec3f(x*number, y*number, z*number);
}

void vec3f::scaleBy(float number) {
	x = x * number;
	y = y * number;
	z = z * number;
}

float vec3f::magnitude() {
	return sqrt(x*x + y*y + z*z);
}

void vec3f::normalize() {
	this->scaleBy(1 / this->magnitude());
}
vec3f vec3f::normalized() {
	return this->scaledBy(1 / this->magnitude());
}

vec3f vec3f::randUnitVector() {

	// https://gist.github.com/andrewbolster/10274979
	float phi = ((float)rand() / (RAND_MAX)) * 2 * M_PI;
	float costheta = ((float)rand() / (RAND_MAX)) * 2 - 1;
	float theta = acosf(costheta);

	float comp1 = sin(theta)*cos(phi);
	float comp2 = sin(theta)*sin(phi);
	float comp3 = cos(theta);

	return vec3f(comp1, comp2, comp3);
}