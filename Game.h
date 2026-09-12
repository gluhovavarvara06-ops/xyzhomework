#pragma once
#include <SFML/Graphics.hpp>
#include "Math.h"
#include "Constants.h"
#include "Player.h"
#include "Apple.h"
#include "Stone.h"
#include <vector>

namespace ApplesGame
{
	enum class GameState
	{
		Menu,
		Playing,
		GameOver,
		Victory
	};

	enum class GameSettingsBits
	{
		IsGameInfinite = 1 << 0,
		IsGameWithAcceleration = 1 << 1,
		IsHardcoreMode = 1 << 2,
		IsZenMode = 1 << 3,
	};

	struct Game
	{
		GameState state = GameState::Menu;
		int selectedMenuItem = 0; 
		sf::Text menuText;
		sf::Text menuItems[4];
		std::vector<std::string> menuOptions;

		uint32_t gameMode = 0;

		//Global game data
		int numEatenApples = 0;
		bool isGameFinished = false;
		bool isGameWon = false;
		float timeSinceGameFinished = 0.f;
		sf::RectangleShape background;

		
		int numApples;
		int numStones;

		Player player;
		//Apple apples[NUM_APPLES];
		//Stone stones[NUM_STONES];
		std::vector<Apple> apples;
		std::vector<Stone> stones;

		//Resources
		sf::Texture playerTexture;
		sf::Texture appleTexture;
		sf::Texture stoneTexture;
		sf::Font font;
		sf::Text GameOvertext;
		sf::Text Victorytext;
		sf::Text scoreText;
		sf::Text hintText;
		
		int score = 0;

	};

	void RestartGame(Game& game);
	void GameMenu(Game& game);
	void ProcessMenuInput(Game& game, sf::Event& event);
	void SetGameMode(Game& game, uint32_t flags);

	void InitGame(Game& game);
	void UpdateGame(Game& game, float deltaTime);

	void DrawGame(Game& game, sf::RenderWindow& window);
	void DeinitializeGame(Game& game);
}