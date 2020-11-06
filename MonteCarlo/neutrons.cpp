#include "pch.h"
#include "neutrons.h"


neutrons::neutrons()
{
}


neutrons::~neutrons()
{
}

float neutrons::velocity(float energy)
{
	// https://www.psi.ch/en/niag/neutron-physics
	return 437 * sqrt(energy);
}

void neutrons::react(float energy)
{
	int nuclide_index = sample_nuclide(energy);

	// Sample reaction index
	int reaction_index = 0;

	if (reaction_index == 1) {
		capture();
	}
	else if (reaction_index == 2) {
		fission(medium[nuclide_index].T);
	}
	else if (reaction_index == 3) {
		scattering(medium[nuclide_index].T);
	}
}

int neutrons::sample_nuclide(float energy)
{
	// Sample random variable in unit interval
	float r = ((float)rand() / (RAND_MAX));

	std::vector<float> macroscopic_cross_sections;
	macroscopic_cross_sections.push_back(0);

	for (int i = 0; i < consentrations.size(); i++) {
		macroscopic_cross_sections.push_back(macroscopic_cross_sections[i] + medium[i].cross_section(energy,medium[i].MTs) * consentrations[i]);
	}

	float target = macroscopic_cross_sections[consentrations.size() + 1];



	return 0;
}

void neutrons::capture()
{

}

void neutrons::fission(float T)
{

}

void neutrons::scattering(float T)
{

}

int binary_search_index(float target, std::vector<float> vec)
{
	int left = 0;
	int right = vec.size() - 1;

	int n = roundf((left + right) / 2);

	if (target < vec[right] && target > vec[left]) {

		while (!(vec[n-1] <= target && target < vec[n])) {
			if (vec[n] < target) {
				left = n+1;
			}
			else if (target < vec[n]) {
				right = n-1;
			}
			n = roundf((left + right) / 2);
		}

	}
	else if (target >= vec[right] || target <= vec[left]) {
		std::cout << "asd" << std::endl;
		return -1;
	}

	return n;
}