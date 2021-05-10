#include <iostream>

#include <SFML/Graphics.hpp>

#include <random>
#include <ctime>

#include "Game.h"

const int WIN_W = 640;
const int WIN_H = 480;

const int C_W = 40;
const int C_H = 40;

int main()
{
	sf::RenderWindow window(sf::VideoMode(WIN_W, WIN_H), "Minesweeper");

	window.setFramerateLimit(30);
	window.setVerticalSyncEnabled(true);

	// Seed random
	srand((unsigned)time(0));

	CGame game(WIN_W, WIN_H, C_W, C_H);

	bool won = false;
	bool playing = true;
	bool lost = false;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}

			if (playing && !lost)
			{
				if (event.type == sf::Event::MouseButtonPressed)
				{
					int x = event.mouseButton.x / C_W;
					int y = event.mouseButton.y / C_H;
					game.PickTile(y, x);
				}
			}

			if (!playing && (lost || won))
			{
				if (event.type == sf::Event::KeyPressed)
				{
					if (event.key.code == sf::Keyboard::R)
					{
						std::cout << "RESET";
						game.Reset();
						won = false;
						playing = true;
						lost = false;
					}
				}
			}
		}

		if (game.HasLost())
		{
			lost = true;
			playing = false;
		}

		window.clear(sf::Color::White);

		game.Render(window);

		if (game.HasWon())
		{
			if (game.GetLevel() == 3)
			{
				won = true;
				playing = false;
				game.Win();
			}
			else
			{
				game.PlayNextLevel();
			}
		}

		window.display();
	}

	return 0;
}