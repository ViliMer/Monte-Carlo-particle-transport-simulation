#pragma once

#include "quadratic_surface.h"
#include "derived_surface.h"
#include "cell.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>

using namespace std;

vector<string> words(string str);
void loadMCVariables(int &N, int &M);
void loadBuffonVariables(int &N, int &M, float &needle_length, float &spacing);

void load_elementary_surfaces(vector<quadratic_surface> &arr);
void load_derived_surfaces(vector<derived_surface> &arr);

void parse_line(vector<string> &w, vector<quadratic_surface> &elementary_surfaces, vector<derived_surface> &derived_surfaces);
void load_cells(string fn, vector<cell> &arr);
