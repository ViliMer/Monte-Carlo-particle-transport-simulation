#include "pch.h"
#include "ENDF_obj.h"
#include "input_reader.h"


ENDF_obj::ENDF_obj()
{
}


ENDF_obj::~ENDF_obj()
{
}

ENDF_obj::ENDF_obj(std::string fn)
{
	load_cross_section_data(fn, *this);
}

int getIndex(vector<int> v, int K)
{
	auto it = find(v.begin(), v.end(), K);

	// If element was found
	if (it != v.end())
	{
		// calculating the index of K
		return it - v.begin();
	}
	else {
		// If the element is not present in the vector
		return -1;
	}
}

float ENDF_obj::cross_section(float E, std::vector<int> MT_numbers)
{
	float sigma = 0;

	int len = MT_numbers.size();
	for (int i = 0; i < len; i++) {
		int MT = MT_numbers[i];
		int index = getIndex(MTs, MT);
		if (index != -1) {
			std::vector<float> E_vec = cs_energies[index];
			std::vector<float> sigma_vec = cross_sections[index];

			int left = 0;
			int right = E_vec.size() - 1;

			int j = roundf((left+right)/2);

			if (E <= E_vec[right] && E >= E_vec[0]) {

				while (!(E_vec[j] == E || (E_vec[j] <= E && E < E_vec[j + 1]))) {
					if (E_vec[j] < E) {
						left = j + 1;
					}
					else if (E < E_vec[j]) {
						right = j - 1;
					}
					j = roundf((left + right) / 2);
				}

				if (E_vec[j] == E) {
					sigma = sigma + sigma_vec[j];
				}
				else {
					sigma = sigma + ((sigma_vec[j + 1] - sigma_vec[j]) / (E_vec[j + 1] - E_vec[j])) * (E - E_vec[j]) + sigma_vec[j];
				}
			}
			else if (E > E_vec[right]) {
				sigma = sigma + sigma_vec[right];
			}
			else if (E < E_vec[0]) {
				sigma = sigma + sigma_vec[0];
			}
		}
		
	}
	return sigma;
}