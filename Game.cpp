#include "Game.h"
#include <cassert>

namespace ApplesGame
{
	void RestartGame(Game& game)
	{
		//Init player
		InitPlayer(game.player, game);

		//Init apples
		game.numApples = MIN_APPLES + std::rand() % (MAX_APPLES - MIN_APPLES + 1);
		game.apples.clear();
		game.apples.reserve(game.numApples);
		for (int i = 0; i < game.numApples; ++i)
		{
			Apple apple;
			InitApple(apple, game);
			game.apples.push_back(apple);
		}

		//Init stones
		if (!(game.gameMode & static_cast<uint32_t>(GameSettingsBits::IsZenMode)) 
			&& !(game.gameMode & static_cast<uint32_t>(GameSettingsBits::IsHardcoreMode)))
		{
			game.numStones = MIN_STONES + std::rand() % (MAX_STONES - MIN_STONES + 1);
		}
		game.stones.clear();
		game.stones.reserve(game.numStones);
		for (int i = 0; i < game.numStones; ++i)
		{
			Stone stone;
			InitStone(stone, game);
			game.stones.push_back(stone);
		}

		if (game.gameMode & static_cast<uint32_t>(GameSettingsBits::IsZenMode))
		{
			game.numStones = 0;
		}

		if (game.gameMode & static_cast<uint32_t>(GameSettingsBits::IsHardcoreMode))
		{
			game.numStones = 20;
		}

		game.numEatenApples = 0;
		game.isGameFinished = false;
		game.isGameWon = false;
		game.timeSinceGameFinished = 0.f;
		game.score = 0;
		game.GameOvertext.setString("");
		game.Victorytext.setString("");
		game.scoreText.setString("Score: 0");
	}

	void GameMenu(Game& game)
	{
		int choice = 0;
		bool menuActive = true;

		assert(game.font.loadFromFile(RESOURCES_PATH + "\\Fonts\\Roboto-LightItalic.ttf"));

		game.background.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
		game.background.setFillColor(sf::Color::Black);
		game.background.setPosition(0.f, 0.f);

		game.menuText.setFont(game.font);
		game.menuText.setString("APPLES GAME");
		game.menuText.setCharacterSize(48);
		game.menuText.setFillColor(sf::Color::Yellow);
		game.menuText.setPosition(SCREEN_WIDTH / 2.f - 100.f, 100.f);

		game.menuText.setFont(game.font);
		game.menuText.setString("Select the game mode! Use the numpad. ESC to quit the game.");
		game.menuText.setCharacterSize(24);
		game.menuText.setFillColor(sf::Color::White);
		game.menuText.setPosition(15.f, 10.f);

		game.menuOptions.clear();
		game.menuOptions.push_back("1. Acceleration Mode");
		game.menuOptions.push_back("2. Infinite Mode");
		game.menuOptions.push_back("3. Hardcore Mode");
		game.menuOptions.push_back("4. Zen Mode");
		for (int i = 0; i < game.menuOptions.size(); ++i)
		{
			game.menuItems[i].setFont(game.font);
			game.menuItems[i].setString(game.menuOptions[i]);
			game.menuItems[i].setCharacterSize(28);
			game.menuItems[i].setFillColor(sf::Color::White);
			game.menuItems[i].setPosition(SCREEN_WIDTH / 2.f - 150.f,
				150.f + i * 45.f);
		}
	}

	void ProcessMenuInput(Game& game, sf::Event& event)
	{
		if (event.type == sf::Event::KeyPressed)
		{
			switch (event.key.code)
			{
				// Acceleration selected
				case sf::Keyboard::Num1:
				case sf::Keyboard::Numpad1:
				{
					uint32_t flags = 0;
					flags |= static_cast<uint32_t>(GameSettingsBits::IsGameWithAcceleration);
					SetGameMode(game, flags);
					game.state = GameState::Playing;
					RestartGame(game);
					break;
				}

				// Infinite selected
				case sf::Keyboard::Num2:
				case sf::Keyboard::Numpad2:
				{
					uint32_t flags = 0;
					flags |= static_cast<uint32_t>(GameSettingsBits::IsGameInfinite);
					SetGameMode(game, flags);
					game.state = GameState::Playing;
					RestartGame(game);
					break;
				}
				
				// Hardcore selected
				case sf::Keyboard::Num3:
				case sf::Keyboard::Numpad3:
				{
					uint32_t flags = 0;
					flags |= static_cast<uint32_t>(GameSettingsBits::IsHardcoreMode);
					SetGameMode(game, flags);
					game.state = GameState::Playing;
					RestartGame(game);
					break;
				}

					// Zen selected
				case sf::Keyboard::Num4:
				case sf::Keyboard::Numpad4:
				{
					uint32_t flags = 0;
					flags |= static_cast<uint32_t>(GameSettingsBits::IsZenMode);
					SetGameMode(game, flags);
					game.state = GameState::Playing;
					RestartGame(game);
					break;
				}
			}
		}
	}

	void SetGameMode(Game& game, uint32_t flags)
	{
		game.gameMode = flags;

		// Acceleration ON
		if (game.gameMode & static_cast<uint32_t>(GameSettingsBits::IsGameWithAcceleration))
		{
			//check apples collision 
			for (int i = 0; i < game.numApples; ++i)
			{
				if (IsCirclesCollide(game.player.position, PLAYER_SIZE / 2.f,
					game.apples[i].position, APPLE_SIZE / 2.f))
				{
					//game.apples[i].position = GetRandomPositionInScreen(SCREEN_WIDTH, SCREEN_HEIGHT);

					game.apples.erase(game.apples.begin() + i);
					game.numApples--;
					i--;

					++game.numEatenApples;
					game.player.speed += ACCELERATION;
					game.score++;
					game.scoreText.setString("Score: " + std::to_string(game.score));
				}
			}
		}
		// Infinite ON
		if (game.gameMode & static_cast<uint32_t>(GameSettingsBits::IsGameInfinite))
		{
			//check apples collision 
			for (int i = 0; i < game.numApples; ++i)
			{
				if (IsCirclesCollide(game.player.position, PLAYER_SIZE / 2.f,
					game.apples[i].position, APPLE_SIZE / 2.f))
				{
					game.apples[i].position = GetRandomPositionInScreen(SCREEN_WIDTH, SCREEN_HEIGHT);

					++game.numEatenApples;
					game.score++;
					game.scoreText.setString("Score: " + std::to_string(game.score));
				}
			}
		}

		// Hardcore ON
		if (game.gameMode & static_cast<uint32_t>(GameSettingsBits::IsHardcoreMode))
		{
			game.numStones = 20;
		}

		// Zen ON
		if (game.gameMode & static_cast<uint32_t>(GameSettingsBits::IsZenMode))
		{
			game.numStones = 0;
		}
		
	}

	void InitGame(Game& game)
	{
		//game.gameMode |= static_cast<uint32_t>(GameSettingsBits::IsGameInfinite);
		//game.gameMode |= static_cast<uint32_t>(GameSettingsBits::IsGameWithAcceleration);

		assert(game.playerTexture.loadFromFile(RESOURCES_PATH + "\\Player.png"));
		assert(game.appleTexture.loadFromFile(RESOURCES_PATH + "\\Apple.png"));
		assert(game.stoneTexture.loadFromFile(RESOURCES_PATH + "\\Rock.png"));
		assert(game.font.loadFromFile(RESOURCES_PATH + "\\Fonts\\Roboto-LightItalic.ttf"));

		game.background.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
		game.background.setFillColor(sf::Color::Black);
		game.background.setPosition(0.f, 0.f);

		game.scoreText.setFont(game.font);
		game.scoreText.setString("Score: " + std::to_string(game.score));
		game.scoreText.setCharacterSize(24);
		game.scoreText.setFillColor(sf::Color::White);
		game.scoreText.setPosition(15.f, 50.f);

		game.hintText.setFont(game.font);
		game.hintText.setString("Use WASD or arrow keys to move! ESC to quit the game.");
		game.hintText.setCharacterSize(24);
		game.hintText.setFillColor(sf::Color::White);
		game.hintText.setPosition(15.f, 10.f);

		RestartGame(game);
	}

	void UpdateGame(Game& game, float deltaTime)
	{
		if (game.state == GameState::Menu)
		{
			return; 
		}

		//Update game state
		if (!game.isGameFinished)
		{
			// Handle input
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			{
				game.player.direction = PlayerDirection::Right;
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			{
				game.player.direction = PlayerDirection::Up;
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			{
				game.player.direction = PlayerDirection::Left;
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			{
				game.player.direction = PlayerDirection::Down;
			}

			// Player movement
			switch (game.player.direction)
			{
			case PlayerDirection::Right:
				game.player.position.x += game.player.speed * deltaTime;
				game.player.sprite.setRotation(0.0f);
				break;
			case PlayerDirection::Up:
				game.player.position.y -= game.player.speed * deltaTime;
				game.player.sprite.setRotation(270.0f);
				break;
			case PlayerDirection::Left:
				game.player.position.x -= game.player.speed * deltaTime;
				game.player.sprite.setRotation(180.0f);
				break;
			case PlayerDirection::Down:
				game.player.position.y += game.player.speed * deltaTime;
				game.player.sprite.setRotation(90.0f);
				break;
			}

			if (game.gameMode & static_cast<uint32_t>(GameSettingsBits::IsGameWithAcceleration))
			{
				//check apples collision 
				for (int i = 0; i < game.numApples; ++i)
				{
					if (IsCirclesCollide(game.player.position, PLAYER_SIZE / 2.f,
						game.apples[i].position, APPLE_SIZE / 2.f))
					{
						game.apples.erase(game.apples.begin() + i);
						game.numApples--;
						i--;

						++game.numEatenApples;
						game.player.speed += ACCELERATION;
						game.score++;
						game.scoreText.setString("Score: " + std::to_string(game.score));
					}
				}
				if (game.timeSinceGameFinished <= PAUSE_LENGTH && game.numApples == 0)
				{
					game.state = GameState::Victory;
					game.isGameWon = true;
					game.isGameFinished = true;
					game.timeSinceGameFinished += deltaTime;
					game.background.setFillColor(sf::Color::Green);

					game.Victorytext.setFont(game.font);
					game.Victorytext.setString("YOU WIN!");
					game.Victorytext.setCharacterSize(72);
					game.Victorytext.setFillColor(sf::Color::Yellow);
					game.Victorytext.setPosition(SCREEN_WIDTH / 3.f, SCREEN_HEIGHT / 2.8f);

					if (game.timeSinceGameFinished > PAUSE_LENGTH)
					{
						game.state = GameState::Menu;
						game.selectedMenuItem = 0;
						GameMenu(game);
					}
									
					return;
				}

				//check stones collision
				for (int i = 0; i < game.numStones; ++i)
				{
					if (IsRectanglesCollide(game.player.position, { PLAYER_SIZE, PLAYER_SIZE },
						game.stones[i].position, { STONE_SIZE, STONE_SIZE }))
					{
						game.state = GameState::GameOver;
						game.isGameFinished = true;
						game.timeSinceGameFinished = 0.f;

						if (game.timeSinceGameFinished > PAUSE_LENGTH)
						{
							game.state = GameState::Menu;
							game.selectedMenuItem = 0;
							GameMenu(game);
						}

						return;
					}
				}

				// Check screen borders collision
				if (game.player.position.x - PLAYER_SIZE / 2.f < 0.f ||
					game.player.position.x + PLAYER_SIZE / 2.f > SCREEN_WIDTH ||
					game.player.position.y - PLAYER_SIZE / 2.f < 0.f ||
					game.player.position.y + PLAYER_SIZE / 2.f > SCREEN_HEIGHT)
				{
					game.state = GameState::GameOver;
					game.isGameFinished = true;
					game.timeSinceGameFinished = 0.f;

					if (game.timeSinceGameFinished > PAUSE_LENGTH)
					{
						game.state = GameState::Menu;
						game.selectedMenuItem = 0;
						GameMenu(game);
					}

					return;
				}

			}

			if (game.gameMode & static_cast<uint32_t>(GameSettingsBits::IsGameInfinite))
			{
				//check apples collision 
				for (int i = 0; i < game.numApples; ++i)
				{
					if (IsCirclesCollide(game.player.position, PLAYER_SIZE / 2.f,
						game.apples[i].position, APPLE_SIZE / 2.f))
					{
						game.apples[i].position = GetRandomPositionInScreen(SCREEN_WIDTH, SCREEN_HEIGHT);

						++game.numEatenApples;
						game.score++;
						game.scoreText.setString("Score: " + std::to_string(game.score));
					}
				}

				//check stones collision
				for (int i = 0; i < game.numStones; ++i)
				{
					if (IsRectanglesCollide(game.player.position, { PLAYER_SIZE, PLAYER_SIZE },
						game.stones[i].position, { STONE_SIZE, STONE_SIZE }))
					{
						game.state = GameState::GameOver;
						game.isGameFinished = true;
						game.timeSinceGameFinished = 0.f;

						if (game.timeSinceGameFinished > PAUSE_LENGTH)
						{
							game.state = GameState::Menu;
							game.selectedMenuItem = 0;
							GameMenu(game);
						}

						return;
					}
				}

				// Check screen borders collision
				if (game.player.position.x - PLAYER_SIZE / 2.f < 0.f ||
					game.player.position.x + PLAYER_SIZE / 2.f > SCREEN_WIDTH ||
					game.player.position.y - PLAYER_SIZE / 2.f < 0.f ||
					game.player.position.y + PLAYER_SIZE / 2.f > SCREEN_HEIGHT)
				{
					game.state = GameState::GameOver;
					game.isGameFinished = true;
					game.timeSinceGameFinished = 0.f;

					if (game.timeSinceGameFinished > PAUSE_LENGTH)
					{
						game.state = GameState::Menu;
						game.selectedMenuItem = 0;
						GameMenu(game);
					}

					return;
				}
			}

			if (game.gameMode & static_cast<uint32_t>(GameSettingsBits::IsHardcoreMode))
			{
				game.numStones = 20;
				//check apples collision 
				for (int i = 0; i < game.numApples; ++i)
				{
					if (IsCirclesCollide(game.player.position, PLAYER_SIZE / 2.f,
						game.apples[i].position, APPLE_SIZE / 2.f))
					{
						game.apples.erase(game.apples.begin() + i);
						game.numApples--;
						i--;

						++game.numEatenApples;
						//game.player.speed += ACCELERATION;
						game.score++;
						game.scoreText.setString("Score: " + std::to_string(game.score));
					}
				}
				if (game.timeSinceGameFinished <= PAUSE_LENGTH && game.numApples == 0)
				{
					game.state = GameState::Victory;
					game.isGameWon = true;
					game.isGameFinished = true;
					game.timeSinceGameFinished += deltaTime;
					game.background.setFillColor(sf::Color::Green);

					game.Victorytext.setFont(game.font);
					game.Victorytext.setString("YOU WIN!");
					game.Victorytext.setCharacterSize(72);
					game.Victorytext.setFillColor(sf::Color::Yellow);
					game.Victorytext.setPosition(SCREEN_WIDTH / 3.f, SCREEN_HEIGHT / 2.8f);

					if (game.timeSinceGameFinished > PAUSE_LENGTH)
					{
						game.state = GameState::Menu;
						game.selectedMenuItem = 0;
						GameMenu(game);
					}

					return;
				}

				//check stones collision
				for (int i = 0; i < game.numStones; ++i)
				{
					if (IsRectanglesCollide(game.player.position, { PLAYER_SIZE, PLAYER_SIZE },
						game.stones[i].position, { STONE_SIZE, STONE_SIZE }))
					{
						game.state = GameState::GameOver;
						game.isGameFinished = true;
						game.timeSinceGameFinished = 0.f;

						if (game.timeSinceGameFinished > PAUSE_LENGTH)
						{
							game.state = GameState::Menu;
							game.selectedMenuItem = 0;
							GameMenu(game);
						}

						return;
					}
				}

				// Check screen borders collision
				if (game.player.position.x - PLAYER_SIZE / 2.f < 0.f ||
					game.player.position.x + PLAYER_SIZE / 2.f > SCREEN_WIDTH ||
					game.player.position.y - PLAYER_SIZE / 2.f < 0.f ||
					game.player.position.y + PLAYER_SIZE / 2.f > SCREEN_HEIGHT)
				{
					game.state = GameState::GameOver;
					game.isGameFinished = true;
					game.timeSinceGameFinished = 0.f;

					if (game.timeSinceGameFinished > PAUSE_LENGTH)
					{
						game.state = GameState::Menu;
						game.selectedMenuItem = 0;
						GameMenu(game);
					}

					return;
				}

			}

			if (game.gameMode & static_cast<uint32_t>(GameSettingsBits::IsZenMode))
			{
				game.numStones = 0;

				for (int i = 0; i < game.numApples; ++i)
				{
					if (IsCirclesCollide(game.player.position, PLAYER_SIZE / 2.f,
						game.apples[i].position, APPLE_SIZE / 2.f))
					{
						game.apples[i].position = GetRandomPositionInScreen(SCREEN_WIDTH, SCREEN_HEIGHT);

						game.apples.erase(game.apples.begin() + i);
						game.numApples--;
						i--;

						++game.numEatenApples;
						game.score++;
						game.scoreText.setString("Score: " + std::to_string(game.score));
					}
				}

				// Check screen borders collision
				if (game.player.position.x - PLAYER_SIZE / 2.f < 0.f ||
					game.player.position.x + PLAYER_SIZE / 2.f > SCREEN_WIDTH ||
					game.player.position.y - PLAYER_SIZE / 2.f < 0.f ||
					game.player.position.y + PLAYER_SIZE / 2.f > SCREEN_HEIGHT)
				{
					game.state = GameState::GameOver;
					game.isGameFinished = true;
					game.timeSinceGameFinished = 0.f;

					if (game.timeSinceGameFinished > PAUSE_LENGTH)
					{
						game.state = GameState::Menu;
						game.selectedMenuItem = 0;
						GameMenu(game);
					}
					return;
				}
				if (game.timeSinceGameFinished <= PAUSE_LENGTH && game.numApples == 0)
				{
					game.state = GameState::Victory;
					game.timeSinceGameFinished += deltaTime;
					game.isGameFinished = true;
					game.background.setFillColor(sf::Color::Green);

					game.Victorytext.setFont(game.font);
					game.Victorytext.setString("YOU WIN!");
					game.Victorytext.setCharacterSize(72);
					game.Victorytext.setFillColor(sf::Color::Yellow);
					game.Victorytext.setPosition(SCREEN_WIDTH / 3.f, SCREEN_HEIGHT / 2.8f);

					if (game.timeSinceGameFinished > PAUSE_LENGTH)
					{
						game.state = GameState::Menu;
						game.selectedMenuItem = 0;
						GameMenu(game);
					}

					return;
				}
			}

		}
		else
		{
			if (game.state == GameState::GameOver)
			{
				game.timeSinceGameFinished += deltaTime;
				game.background.setFillColor(sf::Color::Red);
				
				game.GameOvertext.setFont(game.font);
				game.GameOvertext.setString("Game Over!");
				game.GameOvertext.setCharacterSize(72);
				game.GameOvertext.setFillColor(sf::Color::Yellow);
				
				game.GameOvertext.setPosition(SCREEN_WIDTH / 3.f, SCREEN_HEIGHT / 2.8f);

				if (game.timeSinceGameFinished > PAUSE_LENGTH)
				{
					game.state = GameState::Menu;
					game.selectedMenuItem = 0;
					GameMenu(game);
				}
				return;
			}
			if (!(game.gameMode & static_cast<uint32_t>(GameSettingsBits::IsGameInfinite)) 
				&& game.apples.empty() && game.state == GameState::Victory)
			{
				//game.state = GameState::Victory;
				game.isGameFinished = true;
				game.isGameWon = true;
				game.timeSinceGameFinished += deltaTime;
				game.background.setFillColor(sf::Color::Green);

				game.Victorytext.setFont(game.font);
				game.Victorytext.setString("YOU WIN!");
				game.Victorytext.setCharacterSize(72);
				game.Victorytext.setFillColor(sf::Color::Yellow);
				game.Victorytext.setPosition(SCREEN_WIDTH / 3.f, SCREEN_HEIGHT / 2.8f);

				game.scoreText.setString("Final Score: " + std::to_string(game.score));

				if (game.timeSinceGameFinished > PAUSE_LENGTH)
				{
					game.state = GameState::Menu;
					game.selectedMenuItem = 0;
					GameMenu(game);
				}
				return;
			}
		}
		if (game.timeSinceGameFinished > PAUSE_LENGTH)
		{
			game.state = GameState::Menu;
			game.selectedMenuItem = 0;
			GameMenu(game);
			return;
		}
	}

	void DrawGame(Game& game, sf::RenderWindow& window)
	{
		//Draw game
		window.draw(game.background);

		if (game.state == GameState::Playing)
		{
			DrawPlayer(game.player, window);

			for (int i = 0; i < game.numApples; ++i)
			{
				DrawApples(game.apples[i], window);
			}

			for (int i = 0; i < game.numStones; ++i)
			{
				DrawStones(game.stones[i], window);
			}
		}	

		if (game.timeSinceGameFinished <= PAUSE_LENGTH && game.state == GameState::GameOver)
		{
			window.draw(game.GameOvertext);
		}

		if (game.timeSinceGameFinished <= PAUSE_LENGTH && game.state == GameState::Victory)
		{
			window.draw(game.Victorytext);
		}

		if (game.state == GameState::Menu)
		{
			// Draw menu
			window.draw(game.menuText);
			for (int i = 0; i < 4; ++i)
			{
				window.draw(game.menuItems[i]);
			}
			window.draw(game.menuText);
			return;
		}

		if (game.state != GameState::Menu)
		{
			window.draw(game.scoreText);
			window.draw(game.hintText);
		}

	}

	void DeinitializeGame(Game& game)
	{
		game.apples.clear();
		game.stones.clear();
	}

}
