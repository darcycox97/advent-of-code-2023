#include <fstream>
#include <iostream>
#include <sstream>
#include <set>
#include <algorithm>
#include <numeric>

namespace aoc4
{

    struct Game
    {
        std::set<int> mWinningNumbers;
        std::vector<int> mMyNumbers;
    };

    std::vector<Game> parse(std::ifstream &input, bool verbose)
    {
        std::vector<Game> out;
        std::string line;
        while (std::getline(input, line))
        {
            Game g;
            std::string token;
            std::istringstream iss(line);

            // Skip "Card x:"
            iss >> token >> token;

            // Read numbers until "|"
            while (iss >> token && token != "|")
            {
                g.mWinningNumbers.insert(std::stoi(token));
            }

            // Read remaining numbers
            while (iss >> token)
            {
                g.mMyNumbers.push_back(std::stoi(token));
            }

            if (verbose)
                printf("parsed game with %lu winning tokens and %lu given tokens\n", g.mWinningNumbers.size(), g.mMyNumbers.size());
            out.push_back(g);
        }

        return out;
    }

    void solve4_part1(std::ifstream &input, bool verbose)
    {
        std::vector<Game> games = parse(input, verbose);

        int sum = 0;
        for (const auto &g : games)
        {
            int winningCount = 0;
            for (int n : g.mMyNumbers)
                winningCount += g.mWinningNumbers.contains(n);

            if (winningCount > 0)
            {
                sum += 1 << (winningCount - 1);
            }
        }

        std::cout << sum << std::endl;
    }

    void solve4_part2(std::ifstream &input, bool verbose)
    {
        std::vector<Game> games = parse(input, verbose);
        std::vector<int> cardCopies(games.size());
        std::fill(cardCopies.begin(), cardCopies.end(), 1);

        for (int i = 0; i < games.size(); ++i)
        {
            const auto &g = games[i];

            int winning = std::count_if(g.mMyNumbers.begin(), g.mMyNumbers.end(), [&g](int n)
                                        { return g.mWinningNumbers.contains(n); });

            if (verbose)
                printf("game %d wins %d more cards\n", i, winning);

            for (int j = i + 1; j <= i + winning; ++j)
                cardCopies[j] += cardCopies[i];
        }

        int total = std::accumulate(cardCopies.begin(), cardCopies.end(), 0);

        std::cout << total << std::endl;
    }
}