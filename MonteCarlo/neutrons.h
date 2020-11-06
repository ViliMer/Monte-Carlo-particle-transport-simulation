#pragma once
#include "ENDF_obj.h"

class neutrons
{
public:
	neutrons();
	~neutrons();

	// The medium consists of different materials that have some concentrations
	std::vector<ENDF_obj> medium;
	std::vector<float> consentrations;

	// Energies of neutrons in a vector
	std::vector<float> neutron_energies;

	// Calculate neutron velocity from energy
	float velocity(float energy);

	// Sample reaction type (by first sampling nuclide) and then proceed with that reaction
	void react(float energy);
	int sample_nuclide(float energy);

	// Different reaction types
	void capture();
	void fission(float T);
	void scattering(float T);
};

int binary_search_index(float target, std::vector<float> vec);