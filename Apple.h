#pragma once
#include <SFML/Graphics.hpp>
#include "Math.h"
#include "Constants.h"

namespace ApplesGame
{
	struct Apple
	{
		Position2D position;
		sf::Sprite sprite;
	};

	struct Game;

	void InitApple(Apple& apple, const Game& game);
	void DrawApples(Apple& apple, sf::RenderWindow& window);
}