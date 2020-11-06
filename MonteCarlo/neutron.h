#pragma once

class neutron
{
public:
	neutron();
	~neutron();

	float energy;
	int pos[3] = { 0,0,0 };
	int dir[3] = { 0,0,0 };
	float time = 0;
	int generation = 0;

	neutron(float e);

};

