#include "Stone.h"
#include "Game.h"

namespace ApplesGame
{
	void InitStone(Stone& stone, const Game& game)
	{
		//Init stones
		stone.position = GetRandomPositionInScreen(SCREEN_WIDTH, SCREEN_HEIGHT);

		//Init stone sprite
		stone.sprite.setTexture(game.stoneTexture);
		SetSpriteSize(stone.sprite, STONE_SIZE, STONE_SIZE);
		SetSpriteRelativeOrigin(stone.sprite, 0.5f, 0.5f);
	}

	void DrawStones(Stone& stone, sf::RenderWindow& window)
	{
		stone.sprite.setPosition(stone.position.x, stone.position.y);
		window.draw(stone.sprite);
	}
}