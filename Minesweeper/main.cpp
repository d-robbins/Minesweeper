#include <iostream>

#include <SFML/Graphics.hpp>

#include <random>
#include <ctime>

#include "Game.h"

const int WIN_W = 500;
const int WIN_H = 500;

const int C_W = 50;
const int C_H = 50;

int main()
{
	sf::RenderWindow window(sf::VideoMode(WIN_W, WIN_H), "Minesweeper");
	
	// Seed random
	srand((unsigned)time(0));

	CGame game(WIN_W, WIN_H, C_W, C_H);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}

			if (event.type == sf::Event::MouseButtonPressed)
			{
				int x = event.mouseButton.x / 50;
				int y = event.mouseButton.y / 50;
				game.PickTile(y, x);
			}
		}


		window.clear(sf::Color::White);

		game.Render(window);

		window.display();
	}
	return 0;
}