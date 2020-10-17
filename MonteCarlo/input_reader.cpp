#include "pch.h"
#include "input_reader.h"

vector<string> words(string str) {
	stringstream s(str);
	vector<std::string> segments;
	string word;

	while (s >> word)
	{
		segments.push_back(word);
	}

	return segments;
}

void loadMCVariables(int &N, int &M)
{
	string line;
	ifstream configFile;
	configFile.open("MC_config.txt");
	if (configFile.is_open()) {
		while (getline(configFile, line)) {
			vector<string> w = words(line);
			stringstream num(w[1]);
			if (w[0] == "N") {
				num >> N;
			}
			else if (w[0] == "M")
			{
				num >> M;
			}
		}
		configFile.close();
	}
	else std::cout << "Unable to open file" << std::endl;
}

void loadBuffonVariables(int &N, int &M, float &needle_length, float &spacing)
{
	string line;
	ifstream configFile;
	configFile.open("buffon_config.txt");
	if (configFile.is_open()) {
		while (getline(configFile, line)) {
			vector<string> w = words(line);
			stringstream num(w[1]);
			if (w[0] == "N") {
				num >> N;
			}
			else if (w[0] == "M")
			{
				num >> M;
			}
			else if (w[0] == "needle_length")
			{
				num >> needle_length;
			}
			else if (w[0] == "spacing")
			{
				num >> spacing;
			}
		}
		configFile.close();
	}
	else std::cout << "Unable to open file" << std::endl;
}

void load_elementary_surfaces(vector<quadratic_surface> &arr)
{
	string line;
	ifstream CSG_file;
	CSG_file.open("Elementary surfaces.txt");
	if (CSG_file.is_open()) {
		while (getline(CSG_file, line)) {
			vector<string> w = words(line);

			if (w[0] == "sphere") {
				float x0 = std::stof(w[1]);
				float y0 = std::stof(w[2]);
				float z0 = std::stof(w[3]);
				float r = std::stof(w[4]);
				bool complement;
				istringstream(w[5]) >> complement;

				sphere new_sphere(x0, y0, z0, r, complement);
				arr.push_back(new_sphere);
			}
			else if (w[0] == "plane") {
				float x0 = std::stof(w[1]);
				char axis = w[2][0];
				bool complement;
				istringstream(w[3]) >> complement;

				plane new_plane(x0, axis, complement);
				arr.push_back(new_plane);
			}
			else if (w[0] == "cylinder") {
				float x0 = std::stof(w[1]);
				float y0 = std::stof(w[2]);
				float r = std::stof(w[3]);
				char axis = w[4][0];
				bool complement;
				istringstream(w[5]) >> complement;

				cylinder new_cylinder(x0, y0, r, axis, complement);
				arr.push_back(new_cylinder);
			}
			else if (w[0] == "general") {
				//do something
				std::cout << "Error in load_elementary_surfaces while loading general quadratic surface" << std::endl;
			}
		}
	}
}

void load_derived_surfaces(vector<derived_surface> &arr)
{
	string line;
	ifstream CSG_file;
	CSG_file.open("Derived surfaces.txt");
	if (CSG_file.is_open()) {
		while (getline(CSG_file, line)) {
			vector<string> w = words(line);

			if (w[0] == "truncated_cylinder") {

				float cylinder_x0, cylinder_y0, cylinder_r, plane1_offset, plane2_offset;
				char axis;
				bool complement;

				for (int i = 1; i < 10; i++) {
					if (w[i] == "cylinder") {
						cylinder_x0 = std::stof(w[i+1]);
						cylinder_y0 = std::stof(w[i+2]);
						cylinder_r = std::stof(w[i+3]);
						i = i + 3;
					}
					else if (w[i] == "plane1") {
						plane1_offset = std::stof(w[i + 1]);
						i = i + 1;
					}
					else if (w[i] == "plane2") {
						plane2_offset = std::stof(w[i + 1]);
						i = i + 1;
					}
					else if (w[i] == "axis") {
						axis = w[i + 1][0];
						i = i + 1;
					}
					else if (w[i] == "complemented") {
						istringstream(w[i + 1]) >> complement;
						i = i + 1;
					}
				}

				cylinder new_cylinder(cylinder_x0, cylinder_y0, cylinder_r, axis, false);
				plane new_plane1(plane1_offset, axis, false);
				plane new_plane2(plane2_offset, axis, false);

				truncated_cylinder new_truncated_cylinder(new_cylinder, new_plane1, new_plane2, complement);
				arr.push_back(new_truncated_cylinder);
			}
			else if (w[0] == "cuboid") {

				float xplane1_offset, xplane2_offset, yplane1_offset, yplane2_offset, zplane1_offset, zplane2_offset;
				bool complement;

				for (int i = 1; i < 12; i++)
				{
					if (w[i] == "xplane1") {
						xplane1_offset = std::stof(w[i + 1]);
						i = i + 1;
					}
					else if (w[i] == "xplane2") {
						xplane2_offset = std::stof(w[i + 1]);
						i = i + 1;
					}
					else if (w[i] == "yplane1") {
						yplane1_offset = std::stof(w[i + 1]);
						i = i + 1;
					}
					else if (w[i] == "yplane2") {
						yplane2_offset = std::stof(w[i + 1]);
						i = i + 1;
					}
					else if (w[i] == "zplane1") {
						zplane1_offset = std::stof(w[i + 1]);
						i = i + 1;
					}
					else if (w[i] == "zplane2") {
						zplane2_offset = std::stof(w[i + 1]);
						i = i + 1;
					}
					else if (w[i] == "complemented") {
						istringstream(w[i + 1]) >> complement;
						i = i + 1;
					}
				}

				plane new_xplane1(xplane1_offset, 'x', false);
				plane new_xplane2(xplane2_offset, 'x', false);
				plane new_yplane1(yplane1_offset, 'y', false);
				plane new_yplane2(yplane2_offset, 'y', false);
				plane new_zplane1(zplane1_offset, 'z', false);
				plane new_zplane2(zplane2_offset, 'z', false);

				cuboid new_cuboid(new_xplane1, new_xplane2, new_yplane1, new_yplane2, new_zplane1, new_zplane2, complement);
				arr.push_back(new_cuboid);
			}
			else if (w[0] == "square_prism") {

				float xplane1_offset, xplane2_offset, yplane1_offset, yplane2_offset;
				char axis;
				bool complement;

				for (int i = 1; i < 10; i++)
				{
					if (w[i] == "xplane1") {
						xplane1_offset = std::stof(w[i + 1]);
						i = i + 1;
					}
					else if (w[i] == "xplane2") {
						xplane2_offset = std::stof(w[i + 1]);
						i = i + 1;
					}
					else if (w[i] == "yplane1") {
						yplane1_offset = std::stof(w[i + 1]);
						i = i + 1;
					}
					else if (w[i] == "yplane2") {
						yplane2_offset = std::stof(w[i + 1]);
						i = i + 1;
					}
					else if (w[i] == "axis") {
						axis = w[i + 1][0];
						i = i + 1;
					}
					else if (w[i] == "complemented") {
						istringstream(w[i + 1]) >> complement;
						i = i + 1;
					}
				}

				plane new_xplane1(xplane1_offset, axis, false);
				plane new_xplane2(xplane2_offset, axis, false);
				plane new_yplane1(yplane1_offset, axis, false);
				plane new_yplane2(yplane2_offset, axis, false);

				square_prism new_square_prism(new_xplane1, new_xplane2, new_yplane1, new_yplane2, complement);
				arr.push_back(new_square_prism);
			}
			else if (w[0] == "general") {
				//do something
				std::cout << "Error in load_derived_surfaces while loading general quadratic surface" << std::endl;
			}
		}
	}
}


// Handles a single line of input in Cells.txt
void parse_line(vector<string> &w, vector<quadratic_surface> &elementary_surfaces, vector<derived_surface> &derived_surfaces)
{
	if (w[0] == "sphere") {
		float x0 = std::stof(w[1]);
		float y0 = std::stof(w[2]);
		float z0 = std::stof(w[3]);
		float r = std::stof(w[4]);
		bool complement;
		istringstream(w[5]) >> complement;

		sphere new_sphere(x0, y0, z0, r, complement);
		elementary_surfaces.push_back(new_sphere);
	}
	else if (w[0] == "plane") {
		float x0 = std::stof(w[1]);
		char axis = w[2][0];
		bool complement;
		istringstream(w[3]) >> complement;

		plane new_plane(x0, axis, complement);
		elementary_surfaces.push_back(new_plane);
	}
	else if (w[0] == "cylinder") {
		float x0 = std::stof(w[1]);
		float y0 = std::stof(w[2]);
		float r = std::stof(w[3]);
		char axis = w[4][0];
		bool complement;
		istringstream(w[5]) >> complement;

		cylinder new_cylinder(x0, y0, r, axis, complement);
		elementary_surfaces.push_back(new_cylinder);
	}
	else if (w[0] == "truncated_cylinder") {

		float cylinder_x0, cylinder_y0, cylinder_r, plane1_offset, plane2_offset;
		char axis;
		bool complement;

		for (int i = 1; i < 12; i++) {
			if (w[i] == "cylinder") {
				cylinder_x0 = std::stof(w[i + 1]);
				cylinder_y0 = std::stof(w[i + 2]);
				cylinder_r = std::stof(w[i + 3]);
				i = i + 3;
			}
			else if (w[i] == "plane1") {
				plane1_offset = std::stof(w[i + 1]);
				i = i + 1;
			}
			else if (w[i] == "plane2") {
				plane2_offset = std::stof(w[i + 1]);
				i = i + 1;
			}
			else if (w[i] == "axis") {
				axis = w[i + 1][0];
				i = i + 1;
			}
			else if (w[i] == "complemented") {
				istringstream(w[i + 1]) >> complement;
				i = i + 1;
			}
		}

		cylinder new_cylinder(cylinder_x0, cylinder_y0, cylinder_r, axis, false);
		plane new_plane1(plane1_offset, axis, false);
		plane new_plane2(plane2_offset, axis, false);

		truncated_cylinder new_truncated_cylinder(new_cylinder, new_plane1, new_plane2, complement);
		derived_surfaces.push_back(new_truncated_cylinder);
	}
	else if (w[0] == "cuboid") {

		float xplane1_offset, xplane2_offset, yplane1_offset, yplane2_offset, zplane1_offset, zplane2_offset;
		bool complement;

		for (int i = 1; i < 12; i++)
		{
			if (w[i] == "xplane1") {
				xplane1_offset = std::stof(w[i + 1]);
				i = i + 1;
			}
			else if (w[i] == "xplane2") {
				xplane2_offset = std::stof(w[i + 1]);
				i = i + 1;
			}
			else if (w[i] == "yplane1") {
				yplane1_offset = std::stof(w[i + 1]);
				i = i + 1;
			}
			else if (w[i] == "yplane2") {
				yplane2_offset = std::stof(w[i + 1]);
				i = i + 1;
			}
			else if (w[i] == "zplane1") {
				zplane1_offset = std::stof(w[i + 1]);
				i = i + 1;
			}
			else if (w[i] == "zplane2") {
				zplane2_offset = std::stof(w[i + 1]);
				i = i + 1;
			}
			else if (w[i] == "complemented") {
				istringstream(w[i + 1]) >> complement;
				i = i + 1;
			}
		}

		plane new_xplane1(xplane1_offset, 'x', false);
		plane new_xplane2(xplane2_offset, 'x', false);
		plane new_yplane1(yplane1_offset, 'y', false);
		plane new_yplane2(yplane2_offset, 'y', false);
		plane new_zplane1(zplane1_offset, 'z', false);
		plane new_zplane2(zplane2_offset, 'z', false);

		cuboid new_cuboid(new_xplane1, new_xplane2, new_yplane1, new_yplane2, new_zplane1, new_zplane2, complement);
		derived_surfaces.push_back(new_cuboid);
	}
	else if (w[0] == "square_prism") {

		float xplane1_offset, xplane2_offset, yplane1_offset, yplane2_offset;
		char axis;
		bool complement;

		for (int i = 1; i < 12; i++)
		{
			if (w[i] == "xplane1") {
				xplane1_offset = std::stof(w[i + 1]);
				i = i + 1;
			}
			else if (w[i] == "xplane2") {
				xplane2_offset = std::stof(w[i + 1]);
				i = i + 1;
			}
			else if (w[i] == "yplane1") {
				yplane1_offset = std::stof(w[i + 1]);
				i = i + 1;
			}
			else if (w[i] == "yplane2") {
				yplane2_offset = std::stof(w[i + 1]);
				i = i + 1;
			}
			else if (w[i] == "axis") {
				axis = w[i + 1][0];
				i = i + 1;
			}
			else if (w[i] == "complemented") {
				istringstream(w[i+1]) >> complement;
				i = i + 1;
			}
		}

		// There is a problem with the axis
		plane new_xplane1(xplane1_offset, axis, false);
		plane new_xplane2(xplane2_offset, axis, false);
		plane new_yplane1(yplane1_offset, axis, false);
		plane new_yplane2(yplane2_offset, axis, false);

		square_prism new_square_prism(new_xplane1, new_xplane2, new_yplane1, new_yplane2, complement);
		derived_surfaces.push_back(new_square_prism);
	}
}

void load_cells(string fn, vector<cell> &arr) {
	vector<quadratic_surface> elementary_surfaces;
	vector<derived_surface> derived_surfaces;
	string line;
	ifstream cells_file;
	cells_file.open(fn);
	if (cells_file.is_open()) {
		while (getline(cells_file, line)) {
			vector<string> w = words(line);
			if (w[0] == "cell_start") {
				// A new cell starts here, nothing needs to be done
			}
			else if (w[0] == "cell_end") {
				cell new_cell(elementary_surfaces, derived_surfaces);
				arr.push_back(new_cell);
				elementary_surfaces.clear();
				derived_surfaces.clear();
			}
			else {
				parse_line(w, elementary_surfaces, derived_surfaces);
			}
		}
	}

}