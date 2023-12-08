#include <fstream>
#include <iostream>
#include <sstream>

#include "../util/print.h"

namespace aoc2
{
    struct CubeSet
    {
        int mRed;
        int mGreen;
        int mBlue;
    };

    struct Game
    {
        int mId;
        std::vector<CubeSet> mRounds;
    };

    Game parse_game(const std::string &line)
    {
        int gameId;
        auto idStr = line.substr(0, line.find_first_of(":"));
        sscanf(idStr.c_str(), "Game %d", &gameId);

        Game g;
        g.mId = gameId;

        auto remainder = std::istringstream(line.substr(idStr.size() + 1));

        std::string roundStr;
        while (std::getline(remainder, roundStr, ';'))
        {
            CubeSet cs{0, 0, 0};
            auto roundStream = std::istringstream(roundStr);
            std::string sampleStr; // e.g. "3 blue"
            while (std::getline(roundStream, sampleStr, ','))
            {
                auto sampleStream = std::istringstream(sampleStr);
                int count;
                std::string colour;
                sampleStream >> count;
                sampleStream >> colour;

                if (colour == "blue")
                {
                    cs.mBlue = count;
                }
                else if (colour == "red")
                {
                    cs.mRed = count;
                }
                else
                {
                    cs.mGreen = count;
                }
            }

            g.mRounds.push_back(cs);
        }

        return g;
    }

    std::vector<Game> parse_games(std::ifstream &input)
    {
        std::vector<Game> games;
        std::string line;
        while (std::getline(input, line))
        {
            auto game = parse_game(line);
            games.push_back(game);

            {
                util::verbose_print("id: %d, rounds=%lu\n", game.mId, game.mRounds.size());
                for (int i = 0; i < (int)game.mRounds.size(); ++i)
                {
                    auto r = game.mRounds[i];
                    util::verbose_print("round %d: %d blue, %d red, %d, green\n", (int)i + 1, r.mBlue, r.mRed, r.mGreen);
                }
            }
        }

        return games;
    }

    void solve2_part1(std::ifstream &input)
    {
        const int availRed = 12;
        const int availGreen = 13;
        const int availBlue = 14;

        int sum = 0;
        for (auto game : parse_games(input))
        {
            bool isPossible = true;
            for (const auto &r : game.mRounds)
            {
                if (r.mRed > availRed || r.mGreen > availGreen || r.mBlue > availBlue)
                {
                    isPossible = false;
                    break;
                }
            }

            util::verbose_print("game %d. possible=%s\n", game.mId, isPossible ? "true" : "false");

            if (isPossible)
                sum += game.mId;
        }

        std::cout << sum << std::endl;
    }

    void solve2_part2(std::ifstream &input)
    {
        // find the fewest number of cubes of each color to make each game possible.

        int sum = 0;
        for (auto game : parse_games(input))
        {
            int minRed = 0;
            int minGreen = 0;
            int minBlue = 0;

            for (auto round : game.mRounds)
            {
                minRed = std::max(minRed, round.mRed);
                minGreen = std::max(minGreen, round.mGreen);
                minBlue = std::max(minBlue, round.mBlue);
            }

            int gamePower = minRed * minGreen * minBlue;

            util::verbose_print("game %d: min red: %d, min green: %d, min blue: %d, power: %d\n", game.mId, minRed, minGreen, minBlue, gamePower);

            sum += gamePower;
        }

        std::cout << sum << std::endl;
    }

}