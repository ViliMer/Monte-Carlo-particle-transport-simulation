#include "pch.h"
#include "neutrons.h"


neutrons::neutrons()
{
}


neutrons::~neutrons()
{
}

neutrons::neutrons(float e, int amount)
{
	for (int i = 0; i < amount; i++) {
		primary_neutrons.push_back(neutron(e));
	}
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

	// Sample random variable in unit interval
	float r = ((float)rand() / (RAND_MAX));
	std::vector<float> cross_sections;

	std::vector<int> capture_indices = {102,103,104,105,106,107};
	cross_sections.push_back(medium[nuclide_index].cross_section(energy, capture_indices));

	std::vector<int> fission_indices = {18};
	cross_sections.push_back(cross_sections[0] + medium[nuclide_index].cross_section(energy, fission_indices));

	std::vector<int> scattering_indices;
	for (int i = 51; i < 92; i++) scattering_indices.push_back(i);
	cross_sections.push_back(cross_sections[1] + medium[nuclide_index].cross_section(energy, scattering_indices));

	float target = cross_sections[2] * r;

	int reaction_index = binary_search_index(target, cross_sections);

	// Here we should check if there are secondary neutrons
	std::vector<neutron> &n = primary_neutrons;

	if (reaction_index == 0) {
		capture(n);
	}
	else if (reaction_index == 1) {
		fission(medium[nuclide_index].T, medium[nuclide_index],n);
	}
	else if (reaction_index == 2) {
		scattering(medium[nuclide_index].T,n);
	}

	// T‰‰ll‰ varmaan kusee joku... sen verran huurussa
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

	float target = macroscopic_cross_sections[consentrations.size()] * r;

	int idx = binary_search_index(target, macroscopic_cross_sections);

	// -1 because pushed 0 into vector first
	return idx-1;
}

void neutrons::capture(std::vector<neutron> &n)
{
	n.pop_back();
}

float sample_Maxwell(float T)
{
	float R = 0;
	float r1 = 0;
	float r2 = 0;
	do
	{
		r1 = ((float)rand() / (RAND_MAX));
		r2 = ((float)rand() / (RAND_MAX));
		R = r1 * r1 + r2 * r2;
	} while (R > 1);

	float r3 = ((float)rand() / (RAND_MAX));
	float r4 = ((float)rand() / (RAND_MAX));

	float E = -T * (r1*r1*log(r3) / R + log(r4));

	return E;
}

float maxwell_fission_spectrum(float C0, float E, float T)
{
	return C0 * sqrt(E)*exp(-E / T);
}

void neutrons::fission(float T, ENDF_obj nuclide, std::vector<neutron> &n)
{
	n.pop_back();

	float E = sample_Maxwell(T);

	int index = binary_search_index(E, nuclide.energies);
	// Linear interpolation
	float nubar = ((nuclide.nubars[index] - nuclide.nubars[index-1]) / (nuclide.energies[index] - nuclide.energies[index - 1])) * (E - nuclide.energies[index - 1]) + nuclide.nubars[index - 1];

	int nu = floor(nubar);

	float r = ((float)rand() / (RAND_MAX));
	if (r < nubar - nu) nu += 1;

	for (int i = 0; i < nu; i++) {
		// 1 is a normalization constant, not sure what should be here
		secondary_neutrons.push_back(neutron(maxwell_fission_spectrum(1,E,T)));
	}

}

void neutrons::scattering(float T, std::vector<neutron> &n)
{
	// WIP
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