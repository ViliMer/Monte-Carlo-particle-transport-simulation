#include "pch.h"
#include "neutron.h"
#include "vec3f.h"
#include <math.h>


neutron::neutron()
{
}


neutron::~neutron()
{
}

float vel(float energy)
{
	// https://www.psi.ch/en/niag/neutron-physics
	// meV's converted to MeV's
	return 437 * sqrt(1e9 * energy);
}

neutron::neutron(float e)
{
	energy = e;
	velocity = vec3f::randUnitVector().scaledBy(vel(e));
}