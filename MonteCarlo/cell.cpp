#include "pch.h"
#include "cell.h"


cell::cell()
{
}

cell::cell(std::vector<quadratic_surface> surfs, std::vector<derived_surface> derived_surfs)
{
	surfaces = surfs;
	derived_surfaces = derived_surfs;
	Ns = surfs.size();
	Nd = derived_surfs.size();
}

bool cell::inside(float x, float y, float z) {
	// Offset by small epsilon because of floating point accuracy
	float epsilon = 0.0001;
	bool point_inside = true;
	for (int i = 0; i < Ns; i++) {
		if (surfaces[i].surface_equation(x,y,z) > epsilon) {
			point_inside = false;
			break;
		}
	}

	if (point_inside) {
		for (int i = 0; i < Nd; i++) {
			if (derived_surfaces[i].surface_equation(x, y, z) > 0) {
				point_inside = false;
				break;
			}
		}
	}

	return point_inside;
}

