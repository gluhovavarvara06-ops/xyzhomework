#pragma once
#include <string>

namespace ApplesGame
{
	const std::string RESOURCES_PATH = "Resources/"; //константа верхним регистром для удобства понимания, что это НЕ переменная
	const int SCREEN_WIDTH = 800;
	const int SCREEN_HEIGHT = 600;
	const float INITIAL_SPEED = 100.f; //движение в пикселях в секунду, без прописания времени - за кадр
	const float PLAYER_SIZE = 20.f;
	const float ACCELERATION = 10.f; //ускорение в секунду в пикселях в секунду
	//const int NUM_APPLES = 20;
	const float APPLE_SIZE = 20.f;
	//const int NUM_STONES = 7;
	const float STONE_SIZE = 20.f;
	const float PAUSE_LENGTH = 3.f;

	const int MIN_APPLES = 1;
	const int MAX_APPLES = 30;

	const int MIN_STONES = 7;
	const int MAX_STONES = 15;
}