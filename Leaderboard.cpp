#include "Leaderboard.h"
#include "Game.h"
#include "Constants.h"
#include <algorithm>
#include <string>

namespace ApplesGame
{
	void AddPlayerToLeaderboard(Game& game, Leaderboard& leaderboard, const std::string& playerName, int score)
	{
		leaderboard.leaderboard[playerName] = score;
	};

	void SortLeaderboard(std::vector<Record>& records)
	{
		//insert sort
		for (size_t i = 1; i < records.size(); ++i)
		{
			Record key = records[i];
			int j = static_cast<int>(i) - 1;

			while (j >= 0 && records[j].score < key.score)
			{
				records[j + 1] = records[j];
				--j;
			}
			records[j + 1] = key;
		}
	};

	void TextLeaderboard(Game& game, Leaderboard& leaderboard)
	{
		std::vector<Record> sorted;
		sorted.reserve(leaderboard.leaderboard.size());

		for (const auto& pair : leaderboard.leaderboard)
		{
			Record r;
			r.name = pair.first;
			r.score = pair.second;
			sorted.push_back(r);
		}

		SortLeaderboard(sorted);

		std::string text = "===== LEADERBOARD =====\n";
		for (size_t i = 0; i < sorted.size(); ++i)
		{
			text += std::to_string(i + 1) + ". "
				+ sorted[i].name
				+ " ......... "
				+ std::to_string(sorted[i].score) + "\n";
		}
		text += "=======================";
		
		leaderboard.leaderboardText.setFont(game.font);
		leaderboard.leaderboardText.setString(text);
		leaderboard.leaderboardText.setCharacterSize(26);
		leaderboard.leaderboardText.setFillColor(sf::Color::White);
		leaderboard.leaderboardText.setPosition(SCREEN_WIDTH / 2.f - 200.f, 80.f);
	};

	void GameLeaderboard(Game& game)
	{
		AddPlayerToLeaderboard(game, game.leaderboard, "Player", game.score);
		TextLeaderboard(game, game.leaderboard);
	}
}
