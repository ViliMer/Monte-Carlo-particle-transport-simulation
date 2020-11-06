#pragma once
#include <string>
#include <iostream>
#include <math.h>
#include <string>
#include <vector>

class ENDF_obj
{
public:
	ENDF_obj();
	~ENDF_obj();

	std::string symbol = "";
	int Z = 0, A = 0, NNU = 0;
	float AW = 0, T = 0;
	std::vector<float> energies, nubars, Qs;
	std::vector<int> MTs, NEs;
	std::vector<std::vector<float>> cs_energies, cross_sections;

	// Initialize object from file
	ENDF_obj(std::string fn);

	float cross_section(float E, std::vector<int> MT_numbers);
};

