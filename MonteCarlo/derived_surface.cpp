#include "pch.h"
#include "derived_surface.h"

using namespace std;

derived_surface::derived_surface()
{
}

float derived_surface::surface_equation(float x, float y, float z)
{
	// Offset by small epsilon because of floating point accuracy
	float epsilon = 0.0001;
	float S = -1;
	int lenght = surfaces.size();
	for (int i = 0; i < lenght; i++) {
		if (surfaces[i].surface_equation(x, y, z) > epsilon) {
			S = 1;
		}
	}
	if (complemented) S = -S;
	return S;
}

float derived_surface::distance_equation(float x, float y, float z, float u, float v, float w)
{
	vector<float> distances;
	float length = surfaces.size();
	for (int i = 0; i < length; i++) {
		distances.push_back(surfaces[i].distance_equation(x, y, z, u, v, w));
	}

	float distance = 1000000000;

	for (int i = 0; i < length; i++) {
		if (distances[i] > 0) {
			float new_x = x + distances[i] * u;
			float new_y = y + distances[i] * v;
			float new_z = z + distances[i] * w;
			if (surface_equation(new_x, new_y, new_z) < 0) {
				if (distances[i] < distance) {
					distance = distances[i];
				}
			}
		}
	}

	if (distance = 1000000000) return -1;
	else return distance;
}

truncated_cylinder::truncated_cylinder(cylinder cylinder, plane plane1, plane plane2, bool complement)
{
	if (plane1.X0 < plane2.X0) plane1.complemented = true;
	else plane2.complemented = true;

	surfaces.push_back(cylinder);
	surfaces.push_back(plane1);
	surfaces.push_back(plane2);

	complemented = complement;
}

cuboid::cuboid(plane xplane1, plane xplane2, plane yplane1, plane yplane2, plane zplane1, plane zplane2, bool complement)
{
	if (xplane1.X0 < xplane2.X0) xplane1.complemented = true;
	else xplane2.complemented = true;

	if (yplane1.X0 < yplane2.X0) yplane1.complemented = true;
	else yplane2.complemented = true;

	if (zplane1.X0 < zplane2.X0) zplane1.complemented = true;
	else zplane2.complemented = true;

	surfaces.push_back(xplane1);
	surfaces.push_back(xplane2);
	surfaces.push_back(yplane1);
	surfaces.push_back(yplane2);
	surfaces.push_back(zplane1);
	surfaces.push_back(zplane2);

	complemented = complement;
}

square_prism::square_prism(plane xplane1, plane xplane2, plane yplane1, plane yplane2, bool complement)
{
	if (xplane1.X0 < xplane2.X0) xplane1.complemented = true;
	else xplane2.complemented = true;

	if (yplane1.X0 < yplane2.X0) yplane1.complemented = true;
	else yplane2.complemented = true;

	surfaces.push_back(xplane1);
	surfaces.push_back(xplane2);
	surfaces.push_back(yplane1);
	surfaces.push_back(yplane2);

	complemented = complement;
}