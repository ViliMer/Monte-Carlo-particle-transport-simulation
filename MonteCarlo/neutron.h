#pragma once
#include "vec3f.h"

class neutron
{
public:
	neutron();
	~neutron();

	float energy;
	// L frame position and velocity
	// dir is not usefull, needs to be replaced with velocity, that is vector
	vec3f pos = vec3f(0, 0, 0);
	vec3f velocity = vec3f(0, 0, 0);
	float time = 0;
	int generation = 0;

	neutron(float e);

};

