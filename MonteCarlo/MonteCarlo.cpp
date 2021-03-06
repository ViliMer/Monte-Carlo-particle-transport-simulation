// MonteCarlo.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include "pch.h"
#include "input_reader.h"
#include "visualizer.h"
#include "neutrons.h"
#include <math.h>
#include <chrono>
#include <algorithm>


using namespace std;
using namespace std::chrono;

#define M_PI 3.14159265358979323846

float MCSim(int M)
{
	int m = 0;

	int j = 0;
	while (j < M) {
		j++;

		// Randomly sample M points between (0 < x < 1, 0 < y < 1)
		float x = ((float)rand() / (RAND_MAX));
		float y = ((float)rand() / (RAND_MAX));
		float r = sqrt((x*x) + (y*y));

		// Count the number of points falling inside the circle: m
		if (r <= 1.0f) {
			m++;
		}
	}
	return 4 * float(m) / M;
}

float buffonSim(int M, float needle_length, float spacing)
{
	int needle_over_line = 0;

	int j = 0;
	while (j < M) {
		j++;

		// Randomly sample M distances from the center of the needle to the nearest line between (0 < dist < spacing/2)
		// and sample M angles of the needle compared to the normal of the lines between (-pi/2 < theta < pi/2)
		float dist = ((float)rand() / (RAND_MAX)) * spacing/2;
		float theta = (((float)rand() / (RAND_MAX)) * M_PI) - M_PI/2;	// I assume that I can use pi here, since I'm calculating the probability, not pi

		// The length of the side of the triangle, that is perpendicular to the lines
		float temp_dist = cos(theta) * needle_length / 2;


		// Count the number of times the needle goes over a line
		// If the side of the triangle thats perpendicular to the lines is larger than the distance from the center of the needle to the line, then needle crosses line
		if (temp_dist > dist) {
			needle_over_line++;
		}
	}
	return float(needle_over_line) / M;
}

void statistics(int N, vector<float> arr, float &mean, float &std, float &variance, float &FOM, float time)
{
	float sum = 0;
	for (int i = 0; i < N; i++) {
		sum = sum + arr[i];
	}
	mean = sum / (float)N;

	float temp_sum = 0;
	for (int i = 0; i < N; i++) {
		temp_sum = temp_sum + pow(arr[i] - mean, 2.0f);
	}
	variance = (float)1.0f / (((float)N - 1))*temp_sum;
	std = pow(variance, 0.5f);
	FOM = (float)1.0f / (variance * time);

	//float sum_of_squares = 0;
	//for (int i = 0; i < N; i++) {
	//	sum = sum + arr[i];
	//	sum_of_squares = sum_of_squares + pow(arr[i], 2.0f);
	//}
	//variance = (float)1 / (N*(N - 1))*(sum_of_squares - pow(sum, 2.0f) / N);
	//mean = sum / N;
	//std = pow(variance, 0.5f);
	//FOM = (float)1 / (variance * time);
}

// Simple W/S test for normality
float WSTest(vector<float> array, float std)
{
	float Q, range, min, max;
	min = *min_element(array.begin(), array.end());
	max = *max_element(array.begin(), array.end());
	range = max - min;

	Q = (float)range / std;

	return Q;
}

int ex1()
{
	// Initialize some variables
	int N, M;
	float spacing, needle_length;

	loadMCVariables(N, M);
	vector<float> pi_approximations;

	auto startMC = high_resolution_clock::now();
	for (int i = 0; i < N; i++) {
		float pi_approx = MCSim(M);
		pi_approximations.push_back(pi_approx);
	}
	auto stopMC = high_resolution_clock::now();
	auto running_time_MC = duration_cast<microseconds>(stopMC - startMC);
	float MC_time = (float)running_time_MC.count() / 1000000;

	cout << "##########\nResults for calculating pi with Monte Carlo method by randomly selecting points from square:" << endl;
	cout << "Running time of Monte Carlo Pi calculation was: " << MC_time << " seconds." << endl;

	// Calculate mean, variance, std and FOM
	float pi_mean, variance_MC, std_MC, FOM_MC;
	statistics(N, pi_approximations, pi_mean, std_MC, variance_MC, FOM_MC, MC_time);
	cout << "Variance is: " << variance_MC << ", standard deviation is: " << std_MC << ", mean value is: " << pi_mean << ", FOM is: " << FOM_MC << endl;

	float Q_MC = WSTest(pi_approximations, std_MC);
	cout << "W/S test test statistic Q is: " << Q_MC << endl;

	// #######################################
	// Buffon starts here
	cout << "\n\n##########\nResults for Buffon's needle problem:" << endl;
	loadBuffonVariables(N, M, needle_length, spacing);
	vector<float> probabilities;

	auto start_buffon = high_resolution_clock::now();
	for (int i = 0; i < N; i++) {
		float prob = buffonSim(M, needle_length, spacing);
		probabilities.push_back(prob);
	}
	auto stop_buffon = high_resolution_clock::now();
	auto running_time_buffon = duration_cast<microseconds>(stop_buffon - start_buffon);
	float buffon_time = (float)running_time_buffon.count() / 1000000;
	cout << "Running time of Monte Carlo Buffon calculation was: " << buffon_time << " seconds." << endl;

	// Calculate mean, variance, std and FOM
	float probs_mean, variance_buffon, std_buffon, FOM_buffon;
	statistics(N, probabilities, probs_mean, std_buffon, variance_buffon, FOM_buffon, buffon_time);
	cout << "Variance is: " << variance_buffon << ", standard deviation is: " << std_buffon << ", mean value is: " << probs_mean << ", FOM is: " << FOM_buffon << endl;

	float Q_buffon = WSTest(probabilities, std_buffon);
	cout << "W/S test test statistic Q is: " << Q_buffon << endl;
	return 0;
}

// Returns vector that contains indices of the cells that contain the point
int cell_search(std::vector<cell> cells, float x, float y, float z)
{
	int length = cells.size();

	for (int i = 0; i < length; i++) {
		if (cells[i].inside(x, y, z)) {
			return i;
		}
	}
	return -1;
}

vector<float> MC_cell_volumes(int M, vector<cell> &cells, float xdim, float ydim, float zdim)
{
	int length = cells.size();
	vector<int> cell_counts;
	for (int i = 0; i < length; i++) {
		cell_counts.push_back(0);
	}

	int j = 0;
	while (j < M) {
		j++;

		// Randomly sample M points between (-1 < x < 1, -1 < y < 1, 0 < z < 1)
		float x = ((float)rand() / (RAND_MAX)) * 2 * xdim - xdim;
		float y = ((float)rand() / (RAND_MAX)) * 2 * ydim - ydim;
		float z = ((float)rand() / (RAND_MAX)) * zdim;

		// Count the number of points falling inside each cell
		int index = cell_search(cells, x, y, z);
		if (index != -1) {
			cell_counts[index] = cell_counts[index] + 1;
		}
	}
	vector<float> volumes;
	
	for (int i = 0; i < length; i++) {
		volumes.push_back(4 * xdim * ydim * zdim * float(cell_counts[i]) / M);
	}
	return volumes;
}

void ex2()
{
	//vector<quadratic_surface> surfaces;
	//load_elementary_surfaces(surfaces);

	//for (int i = 0; i < 3; i++) {
	//	std::cout << surfaces[i].surface_name() << std::endl;
	//	surfaces[i].toString();
	//}

	//vector<derived_surface> derived_surfaces;
	//load_derived_surfaces(derived_surfaces);
	//for (int i = 0; i < 3; i++) {
	//	int length = derived_surfaces[i].surfaces.size();
	//	for (int j = 0; j < length; j++) {
	//		std::cout << derived_surfaces[i].surfaces[j].surface_name() << std::endl;
	//		derived_surfaces[i].surfaces[j].toString();
	//	}
	//}

	int N, M;
	loadMCVariables(N, M);

	int num_cells = 4;

	vector<vector<float>> volume_approximations;

	for (int i = 0; i < num_cells; i++) {
		vector<float> temp;
		temp.reserve(N);
		volume_approximations.push_back(temp);
	}

	vector<cell> cells;
	load_cells("Fuelpin cells.txt", cells);
	//render(cells);

	auto start = high_resolution_clock::now();
	for (int i = 0; i < N; i++) {
		vector<float> vol_approx = MC_cell_volumes(M, cells, 1, 1, 1);
		for (int j = 0; j < num_cells; j++) {
			volume_approximations[j].push_back(vol_approx[j]);
		}
	}
	auto stop = high_resolution_clock::now();
	auto running_time = duration_cast<microseconds>(stop - start);
	float time = (float)running_time.count() / 1000000;

	cout << "##########\n1) Results for calculating volumes with Monte Carlo method by randomly selecting points from cube:" << endl;
	cout << "Running time of calculation was: " << time << " seconds." << endl;

	for (int i = 0; i < num_cells; i++) {
		// Calculate mean, variance, std and FOM
		float mean, variance, std, FOM;
		statistics(N, volume_approximations[i], mean, std, variance, FOM, time);
		cout << "##### " << i << "th cell:" << endl;
		cout << "Variance is: " << variance << ", standard deviation is: " << std << ", mean value is: " << mean << ", FOM is: " << FOM << endl;
	}

	//############### 2) starts here
	volume_approximations.clear();

	for (int i = 0; i < num_cells; i++) {
		vector<float> temp;
		temp.reserve(N);
		volume_approximations.push_back(temp);
	}

	cells.clear();
	load_cells("2) cells.txt", cells);

	start = high_resolution_clock::now();
	for (int i = 0; i < N; i++) {
		vector<float> vol_approx = MC_cell_volumes(M, cells, 2.5, 2.5, 8);
		for (int j = 0; j < num_cells; j++) {
			volume_approximations[j].push_back(vol_approx[j]);
		}
	}
	stop = high_resolution_clock::now();
	running_time = duration_cast<microseconds>(stop - start);
	time = (float)running_time.count() / 1000000;

	cout << "##########\n2) Results for calculating volumes with Monte Carlo method by randomly selecting points from cube:" << endl;
	cout << "Running time of calculation was: " << time << " seconds." << endl;

	for (int i = 0; i < num_cells; i++) {
		// Calculate mean, variance, std and FOM
		float mean, variance, std, FOM;
		statistics(N, volume_approximations[i], mean, std, variance, FOM, time);
		cout << "##### " << i << "th cell:" << endl;
		cout << "Variance is: " << variance << ", standard deviation is: " << std << ", mean value is: " << mean << ", FOM is: " << FOM << endl;
	}
	

}

void ex3()
{
	// No geometry, so skip step 1 (sampling path length) I guess
	float target = 3.5;
	vector<float> vec;
	cout << "Vec is: " << endl;
	for (float i = 0; i < 10; i++) {
		vec.push_back(i);
		cout << vec[i] << endl;
	}
	int index = binary_search_index(target, vec);
	cout << "Found index: " << index << endl;


	// Ex3 starts here

	// First initialize stuff...
	//#######
	ENDF_obj medium1("H1.dat.txt");
	ENDF_obj medium2("H2.dat.txt");

	neutrons neuts(14,1000);
	neuts.consentrations.push_back(0.5);
	neuts.consentrations.push_back(0.5);
	neuts.medium.push_back(medium1);
	neuts.medium.push_back(medium2);

	vector<tuple<int, float>> gen_energy_pairs;
	//#######

	// Then loop...
	int i = 1;
	while (!neuts.primary_neutrons.empty()) {
		neuts.react(&gen_energy_pairs);
		if (i % 10000 == 0) {
			cout << "Iteration: " << i << endl;
			cout << "Primary neutrons left: " << neuts.primary_neutrons.size() << endl;
		}
		
		i++;
	}

	ofstream file;
	file.open("ex3_results.txt");
	for (int i = 0; i < gen_energy_pairs.size(); i++) {
		file << get<0>(gen_energy_pairs[i]);
		file << " ";
		file << get<1>(gen_energy_pairs[i]);
		file << "\n";
	}
	file.close();
	return;

}

int main()
{

	//ex1();
	//ex2();
	ex3();

}