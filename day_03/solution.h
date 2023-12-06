#include <fstream>
#include <iostream>
#include <sstream>

namespace aoc3
{

    struct Point
    {
        int x;
        int y;
    };
    bool operator<(const Point &l, const Point &r)
    {
        return l.x < r.x || (l.x == r.x && l.y < r.y);
    }

    struct Digit
    {
        int mValue;
        int mX1;
        int mX2;
        int mY;
    };

    std::pair<std::vector<Digit>, std::map<Point, char>> parse(std::ifstream &input, bool verbose)
    {
        std::string line;
        int y = 0;
        std::map<Point, char> symbolLocs;
        std::vector<Digit> digits;
        while (std::getline(input, line))
        {
            std::string num;
            for (int x = 0; x < (int)line.size(); ++x)
            {
                if (isdigit(line[x]))
                {
                    num.push_back(line[x]);
                }
                else if (line[x] != '.')
                {
                    // then we've found a symbol. grab the coordinates
                    symbolLocs[Point{x, y}] = line[x];
                    if (verbose)
                        printf("found symbol %c at x=%d y=%d\n", line[x], x, y);
                }

                // peek ahead to check if we have reached the end of a number
                if (!num.empty() && (x + 1 == (int)line.size() || !isdigit(line[x + 1])))
                {
                    // flush the current number
                    int start = x - num.size() + 1;
                    int end = x;
                    digits.push_back(Digit{std::stoi(num), start, end, y});
                    num.clear();

                    if (verbose)
                    {
                        const auto &d = digits.back();
                        printf("found digit %d x1=%d x2=%d y=%d\n", d.mValue, d.mX1, d.mX2, d.mY);
                    }
                }
            }
            ++y;
        }

        return std::make_pair(digits, symbolLocs);
    }

    void solve3_part1(std::ifstream &input, bool verbose)
    {
        auto [digits, symbols] = parse(input, verbose);

        int sum = 0;
        for (const auto &d : digits)
        {
            // check for symbols above
            bool found = false;
            for (int x = d.mX1 - 1; x <= d.mX2 + 1; ++x)
            {
                for (int y = d.mY - 1; y <= d.mY + 1; ++y)
                {
                    if (symbols.contains({x, y}))
                    {
                        found = true;
                        break;
                    }
                }

                if (found)
                    break;
            }

            if (found)
            {
                if (verbose)
                    printf("found part with value %d\n", d.mValue);

                sum += d.mValue;
            }
        }

        std::cout << sum << std::endl;
    }

    void solve3_part2(std::ifstream &input, bool verbose)
    {
        auto [digits, symbols] = parse(input, verbose);

        // find all gear ratios (multiplied part numbers) and add them.
        // gear = any * symbol adjacent to two parts.

        std::map<Point, std::map<Point, int>> starAdjacency;

        for (const auto &d : digits)
        {
            for (int x = d.mX1 - 1; x <= d.mX2 + 1; ++x)
            {
                for (int y = d.mY - 1; y <= d.mY + 1; ++y)
                {
                    if (auto it = symbols.find({x, y}); it != symbols.end() && it->second == '*')
                    {
                        // found star next to a digit. record that digit in the adjacency list
                        auto &adjDigits = starAdjacency[{x, y}];
                        adjDigits[{d.mX1, d.mY}] = d.mValue;
                    }
                }
            }
        }

        int sum = 0;
        for (auto [xy, adjDigits] : starAdjacency)
        {
            if (adjDigits.size() != 2)
                continue;

            auto it = adjDigits.begin();
            int part1 = it->second;
            int part2 = (++it)->second;

            if (verbose)
                printf("found gear at x=%d y=%d, part1 = %d part2 = %d\n", xy.x, xy.y, part1, part2);

            sum += (part1 * part2);
        }

        std::cout << sum << std::endl;
    }

}