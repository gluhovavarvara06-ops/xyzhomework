#pragma once
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include <map>

namespace ApplesGame
{
    struct Record
    {
        std::string name;
        int score = 0;
    };

    struct Leaderboard
    {
        sf::Text leaderboardTitle;
        sf::Text leaderboardText;
        std::map<std::string, int> leaderboard = {
            {"Mark",  120},
            {"Bob",    89},
            {"Carol",  67},
            {"Dave",   52},
            {"Alice",  42}
        };
    };

    struct Game;
   
    void AddPlayerToLeaderboard(Game& game, Leaderboard& leaderboard, const std::string& playerName, int score);
    void SortLeaderboard(std::vector<Record>& records);
    void TextLeaderboard(Game& game, Leaderboard& leaderboard);
    void GameLeaderboard(Game& game);
}