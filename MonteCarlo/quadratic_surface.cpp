#include "pch.h"
#include "quadratic_surface.h"

using namespace std;

// Default
quadratic_surface::quadratic_surface()
{
	A = 0;
	B = 0;
	C = 0;
	D = 0;
	E = 0;
	F = 0;
	G = 0;
	H = 0;
	I = 0;
	J = 0;

	complemented = false;

	surf_name = "General surface";
}


// General
quadratic_surface::quadratic_surface(float a, float b, float c, float d, float e, float f, float g, float h, float i, float j, bool complement)
{
	A = a;
	B = b;
	C = c;
	D = d;
	E = e;
	F = f;
	G = g;
	H = h;
	I = i;
	J = j;

	complemented = complement;

	surf_name = "General surface";
}

float quadratic_surface::surface_equation(float x, float y, float z)
{
	float S = A * x*x + B * y*y + C * z*z + D * x*y + E * y*z + F * z*x + G * x + H * y + I * z + J;
	if (complemented) S = -S;
	return S;
}

float quadratic_surface::distance_equation(float x, float y, float z, float u, float v, float w) {

	float K = A*x*x + B*y*y + C*z*z + D*x*y + E*y*z + F*x*z + G*x + H*y + I*z + J;
	float L = 2 * (A*u*x + B*v*y + C*w*z) + D * (v*x + u*y) + E * (w*y + v*z) + F * (w*x + u*z) + G*u + H*v + I*w;
	float M = A*u*u + B*v*v + C*w*w + D*u*v + E*v*w + F*u*w;

	float discriminant = L * L - 4 * M*K;

	if (discriminant > 0) {
		float root = sqrt(discriminant);
		
		float dist_plus = (-L + root) / (2 * M);
		float dist_minus = (-L - root) / (2 * M);

		if (dist_plus > 0 && dist_minus > 0) return fmin(dist_plus, dist_minus);
		else if (dist_plus > 0) return dist_plus;
		else if (dist_minus > 0) return dist_minus;
	}
	// Here either determinant is negative (no solutions exist), or both solutions are negative
	return -1;
}

void quadratic_surface::toString()
{
	std::cout << "A = " << A << "\nB = " << B << "\nC = " << C << "\nD = " << D << "\nE = " << E << "\nF = " << F << "\nG = " << G << "\nH = " << H << "\nI = " << I << "\nJ = " << J << "\nComplemented = " << complemented << std::endl;
}

std::string quadratic_surface::surface_name() {
	return surf_name;
}

// Axis aligned plane
plane::plane(float x0, char axis, bool complement)
{
	X0 = x0;
	AXIS = axis;

	if (axis == 'x') {
		G = 1;
	}
	else if (axis == 'y') {
		H = 1;
	}
	else if (axis == 'z') {
		I = 1;
	}
	A = 0;
	B = 0;
	C = 0;
	D = 0;
	E = 0;
	F = 0;
	J = -x0;

	complemented = complement;

	surf_name = "Axis aligned plane";
}

// Axis aligned cylinder
cylinder::cylinder(float x0, float y0, float r, char axis, bool complement)
{
	X0 = x0;
	Y0 = y0;
	R = r;
	AXIS = axis;

	if (axis == 'x') {
		A = 0;
		B = 1;
		C = 1;

		G = 0;
		H = -2 * x0;
		I = -2 * y0;
	}
	else if (axis == 'y') {
		A = 1;
		B = 0;
		C = 1;

		G = -2 * x0;
		H = 0;
		I = -2 * y0;
	}
	else if (axis == 'z') {
		A = 1;
		B = 1;
		C = 0;
		
		G = -2 * x0;
		H = -2 * y0;
		I = 0;
	}
	D = 0;
	E = 0;
	F = 0;
	J = x0 * x0 + y0 * y0 - r * r;

	complemented = complement;

	surf_name = "Axis aligned cylinder";
}

sphere::sphere(float x0, float y0, float z0, float r, bool complement)
{
	X0 = x0;
	Y0 = y0;
	Z0 = z0;
	R = r;

	A = 1;
	B = 1;
	C = 1;
	D = 0;
	E = 0;
	F = 0;
	G = -2 * x0;
	H = -2 * y0;
	I = -2 * z0;
	J = x0 * x0 + y0 * y0 + z0 * z0 + r * r;

	complemented = complement;

	surf_name = "Sphere";
}