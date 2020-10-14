#pragma once
#include <string>
#include <iostream>
#include <math.h>

class quadratic_surface
{
public:
	float A, B, C, D, E, F, G, H, I, J;
	bool complemented;
	std::string surf_name;

	// Default
	quadratic_surface();

	// General
	quadratic_surface(float a, float b, float c, float d, float e, float f, float g, float h, float i, float j, bool complement);

	float surface_equation(float x, float y, float z);
	float distance_equation(float x, float y, float z, float u, float v, float w);
	std::string surface_name();
	void toString();
};


class sphere : public quadratic_surface
{
public:
	float X0, Y0, Z0, R;
	sphere(float x0, float y0, float z0, float r, bool complement);
};

class plane : public quadratic_surface
{
public:
	float X0;
	char AXIS;
	plane(float x0, char axis, bool complement);
};

class cylinder : public quadratic_surface
{
public:
	float X0, Y0, R;
	char AXIS;
	cylinder(float x0, float y0, float r, char axis, bool complement);
};
