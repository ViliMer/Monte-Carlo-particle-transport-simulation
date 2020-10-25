#pragma once
#include "cell.h"
#include <SFML/Graphics.hpp>

using namespace std;

void render(vector<cell> &cells);

class camera
{
public:
	sf::Vector3f camera_pos = sf::Vector3f(-5,0,0);
	sf::Vector3f camera_dir = sf::Vector3f(1,0,0);

	int xres = 1200;
	int yres = 800;

	camera();
	camera(sf::Vector3f pos, sf::Vector3f dir, int xresolution, int yresolution);

	sf::Vector3f pos_screen(int x, int y);
};