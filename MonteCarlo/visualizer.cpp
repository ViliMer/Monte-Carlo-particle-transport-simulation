#include "pch.h"
#include "visualizer.h"
#include <math.h>

void render(vector<cell> &cells)
{
	
	sf::Vector3f camera_pos(-5, 0, 0);
	sf::Vector3f camera_dir(1, 0, 0);

	sf::RenderWindow window(sf::VideoMode(1200, 1200), "Render");
	sf::Image current_image;
	current_image.create(800, 1200, sf::Color::White);
	sf::Texture texture;

	texture.loadFromImage(current_image);

	sf::Sprite sprite;

	sprite.setTexture(texture);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear(sf::Color::White);
		for (int x = 0; x < 1200; x++) {
			for (int y = 0; y < 1200; y++) {
				if (sqrt((x-200)*(x-200) + (y-300)*(y-300)) < 200) {
					current_image.setPixel(x, y, sf::Color::Blue);
				}
			}
		}
		texture.loadFromImage(current_image);
		window.draw(sprite);
		window.display();
	}
}


camera::camera() {}

camera::camera(sf::Vector3f pos, sf::Vector3f dir, int xresolution, int yresolution)
{
	camera_pos = pos;
	camera_dir = dir;

	xres = xresolution;
	yres = yresolution;
}

sf::Vector3f camera::pos_screen(int x, int y) {
	// WIP
	return (sf::Vector3f(xres - xres / 2 - x, yres - yres / 2 - y, 0));
}