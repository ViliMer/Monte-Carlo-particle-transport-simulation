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

//float neutrons::velocity(float energy)
//{
//	// https://www.psi.ch/en/niag/neutron-physics
//	return 437 * sqrt(energy);
//}

float energy(float vel, float mass)
{
	float E = .5f * mass * vel * vel;

	// Convert joules to eV's
	// 1 Joule = 6.24150913e18 eV's
	E *= 6.24150913e12;

	return E;
}

float vel(float energy, float mass)
{
	// eV:s to joules
	float energy_conversion_constant = 1.602176634e-13;
	// Daltons to kg:s
	float mass_conversion_constant = 1.66053906660e-27;
	return sqrt(2 * energy * energy_conversion_constant / (mass * mass_conversion_constant));
}



void neutrons::react(std::vector<std::tuple<int, float>> *gen_energy_pairs)
{
	float energy = primary_neutrons.back().energy;
	int nuclide_index = sample_nuclide(energy);


	// Sample reaction index

	// Sample random variable in unit interval
	float r = ((float)rand() / (RAND_MAX));
	std::vector<float> cross_sections;

	std::vector<int> capture_indices = { 102,103,104,105,106,107 };
	cross_sections.push_back(medium[nuclide_index].cross_section(energy, capture_indices));

	std::vector<int> fission_indices = { 18 };
	cross_sections.push_back(cross_sections[0] + medium[nuclide_index].cross_section(energy, fission_indices));

	std::vector<int> scattering_indices = { 2 };
	//for (int i = 51; i < 92; i++) scattering_indices.push_back(i);
	cross_sections.push_back(cross_sections[1] + medium[nuclide_index].cross_section(energy, scattering_indices));

	float target = cross_sections[2] * r;

	int reaction_index = binary_search_index(target, cross_sections);

	//std::cout << "Cross sections: " << cross_sections[0] << ", " << cross_sections[1] << ", " << cross_sections[2] << std::endl;
	//std::cout << "Reaction index " << reaction_index << std::endl;

	if (reaction_index == 0) {
		capture();
	}
	else if (reaction_index == 1) {
		fission(medium[nuclide_index].T, medium[nuclide_index]);
	}
	else if (reaction_index == 2) {
		scattering(medium[nuclide_index].T, medium[nuclide_index], gen_energy_pairs);
	}

	// Täällä varmaan kusee joku... sen verran huurussa
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

void neutrons::capture()
{
	primary_neutrons.pop_back();
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

void neutrons::fission(float T, ENDF_obj nuclide)
{
	primary_neutrons.pop_back();

	float E = sample_Maxwell(T);

	int index = binary_search_index(E, nuclide.energies);
	// Linear interpolation
	float nubar = ((nuclide.nubars[index] - nuclide.nubars[index-1]) / (nuclide.energies[index] - nuclide.energies[index - 1])) * (E - nuclide.energies[index - 1]) + nuclide.nubars[index - 1];

	int nu = floor(nubar);

	float r = ((float)rand() / (RAND_MAX));
	if (r < nubar - nu) nu += 1;

	for (int i = 0; i < nu; i++) {
		// 1 is a normalization constant, not sure what should be here
		primary_neutrons.push_back(neutron(maxwell_fission_spectrum(1,E,T)));
	}

}

void neutrons::scattering(float T, ENDF_obj nuclide, std::vector<std::tuple<int, float>> *gen_energy_pairs)
{
	// Velocities should be vectors...
	// WIP

	float E_fg = 0.0002;

	neutron neut = primary_neutrons.back();
	neut.generation += 1;
	float E_neutron = neut.energy;
	float E_target, target_speed;

	// Should also sample direction, but doesnt matter at the moment...


	if (E_neutron < E_fg) {
		// Kalvoista... Temperature in MeV:s, multiply temperature in Kelvins by Boltzmann constant
		E_target = sample_Maxwell(T * 8.6167e-11);
		target_speed = vel(E_target, nuclide.AW);
	}
	else {
		target_speed = 0;
	}

	float A = nuclide.AW/1.00866491588;

	vec3f randUnit = vec3f::randUnitVector();
	vec3f randScaled = randUnit.scaledBy(A*target_speed);
	vec3f summa = neut.velocity.sum(randScaled);

	vec3f V_CM = summa.scaledBy(1/(1+A));
	vec3f v_C = neut.velocity.sum(V_CM.scaledBy(-1));
	
	// sample direction isotropically
	vec3f newDir = vec3f::randUnitVector();
	v_C = newDir.scaledBy(v_C.magnitude());

	neut.velocity = v_C.sum(V_CM);
	neut.energy = energy(neut.velocity.magnitude(), 1.674927471e-27);
	primary_neutrons.pop_back();
	primary_neutrons.push_back(neut);
	std::tuple<int, float> gen_energy_pair = std::make_tuple(neut.generation, neut.energy);
	gen_energy_pairs->push_back(gen_energy_pair);
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
	else if (target <= vec[left]) {
		return 0;
	}
	else if (target == vec[right]) {
		return right;
	}
	else if (target > vec[right]) {
		std::cout << "Error in binary_search_index" << std::endl;
		std::cout << "Target, vec[left], vec[right]" << target << ", " << vec[left] << ", " << vec[right] << std::endl;
		return -1;
	}

	return n;
}