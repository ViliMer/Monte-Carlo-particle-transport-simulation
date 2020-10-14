#pragma once
#include "quadratic_surface.h"
#include "derived_surface.h"
#include <vector>
class cell
{
public:
	cell();
	cell(std::vector<quadratic_surface> surfs, std::vector<derived_surface> derived_surfs);
	std::vector<quadratic_surface> surfaces;
	std::vector<derived_surface> derived_surfaces;

	// Number of surfaces and derived surfaces
	int Ns, Nd;

	bool inside(float x, float y, float z);
};

