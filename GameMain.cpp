// ©2023, XYZ School. All rights reserved.
// Authored by Aleksandr Rybalka (polterageist@gmail.com)

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <ctime>
#include "Constants.h"
#include "Game.h"

int main()
{
	using namespace ApplesGame;

	int seed = (int)time(nullptr);
	srand(seed);
	//Init window
	sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Apples game!");

	//Game initilization
	Game game;
	InitGame(game);
	GameMenu(game);

	sf::Clock gameClock;
	float lastTime = gameClock.getElapsedTime().asSeconds();

	//Main loop
	while (window.isOpen())
	{
		//Reduce framerate to not spam CPU and GPU
		sf::sleep(sf::milliseconds(16));

		//Calculate time delta
		float currentTime = gameClock.getElapsedTime().asSeconds();
		float deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		//Read events
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
				break;
			}
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
			{
				window.close();
				break;
			}

			// Обработка ввода в меню
			if (game.state == GameState::Menu)
			{
				ProcessMenuInput(game, event);
			}
			else if (event.type == sf::Event::KeyPressed &&
				event.key.code == sf::Keyboard::Escape)
			{
				if (game.state == GameState::Playing)
				{
					game.state = GameState::Menu;
					GameMenu(game);
				}
				else
				{
					window.close();
				}
			}
		}

		//Update game state
		UpdateGame(game, deltaTime);

		//Draw game
		window.clear();
		DrawGame(game, window);
		
		window.display();
	}

	//Deinitilization
	DeinitializeGame(game);

	return 0;

}