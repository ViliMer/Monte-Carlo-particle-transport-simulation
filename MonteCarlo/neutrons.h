#pragma once
#include "ENDF_obj.h"
#include "neutron.h"
#include<tuple>

class neutrons
{
public:
	neutrons();
	~neutrons();

	neutrons(float e, int amount);

	// The medium consists of different materials that have some concentrations
	std::vector<ENDF_obj> medium;
	std::vector<float> consentrations;

	// Individual neutrons in vector
	std::vector<neutron> primary_neutrons;
	//std::vector<neutron> secondary_neutrons;

	// Calculate neutron velocity from energy
	float velocity(float energy);

	// Sample reaction type (by first sampling nuclide) and then proceed with that reaction
	void react(std::vector<std::tuple<int, float>> *gen_energy_pairs);
	int sample_nuclide(float energy);

	// Different reaction types
	void capture();
	void fission(float T, ENDF_obj nuclide);
	void scattering(float T, ENDF_obj nuclide, std::vector<std::tuple<int, float>> *gen_energy_pairs);
};

int binary_search_index(float target, std::vector<float> vec);