#pragma once
#include "quadratic_surface.h"
#include <vector>

class derived_surface
{
public:
	derived_surface();
	bool complemented;

	std::vector<quadratic_surface> surfaces;
	float surface_equation(float x, float y, float z);
	float distance_equation(float x, float y, float z, float u, float v, float w);
};

class truncated_cylinder : public derived_surface
{
public:
	truncated_cylinder(cylinder cylinder, plane plane1, plane plane2, bool complement);
};

class cuboid : public derived_surface
{
public:
	cuboid(plane xplane1, plane xplane2, plane yplane1, plane yplane2, plane zplane1, plane zplane2, bool complement);
};

class square_prism : public derived_surface
{
public:
	square_prism(plane xplane1, plane xplane2, plane yplane1, plane yplane2, bool complement);
};