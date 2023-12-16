#include "../util/print.h"

#include <iostream>
#include <fstream>
#include <set>

namespace aoc11
{

    using Image = std::vector<std::vector<char>>;
    using Point = std::pair<int, int>; // y, x

    Image parse(std::ifstream &input)
    {
        Image image;
        std::string line;
        while (std::getline(input, line))
        {
            std::vector<char> row;
            std::copy(line.begin(), line.end(), std::back_inserter(row));
            image.push_back(row);
        }
        return image;
    }

    int64_t solve(std::ifstream &input, int expansionFactor)
    {
        auto image = parse(input);

        std::set<Point> galaxies;
        std::set<int> expandedRows;
        std::set<int> expandedCols;

        for (int y = 0; y < (int)image.size(); ++y)
        {
            for (int x = 0; x < (int)image.back().size(); ++x)
            {
                if (image[y][x] == '#')
                {
                    galaxies.insert({y, x});
                    util::verbose_print("found galaxy at {%d, %d}\n", y, x);
                }
            }
        }

        for (int y = 0; y < (int)image.size(); ++y)
        {
            if (std::find_if(galaxies.begin(), galaxies.end(), [y](auto g)
                             { return g.first == y; }) == galaxies.end())
            {
                util::verbose_print("no galaxies in row %d\n", y);
                expandedRows.insert(y);
            }
        }
        for (int x = 0; x < (int)image.size(); ++x)
        {
            if (std::find_if(galaxies.begin(), galaxies.end(), [x](auto g)
                             { return g.second == x; }) == galaxies.end())
            {
                util::verbose_print("no galaxies in col %d\n", x);
                expandedCols.insert(x);
            }
        }

        util::verbose_print("found %llu galaxies\n", galaxies.size());

        // find shortest paths to every galaxy
        int64_t sum = 0;
        int sourceId = 1;
        for (auto source = galaxies.begin(); source != std::prev(galaxies.end()); ++source)
        {
            int destId = sourceId + 1;
            for (auto dest = std::next(source); dest != galaxies.end(); ++dest)
            {
                auto [source_y, source_x] = *source;
                auto [dest_y, dest_x] = *dest;
                int y_dist = std::abs(source_y - dest_y);
                int x_dist = std::abs(source_x - dest_x);

                int64_t dist = x_dist + y_dist;

                for (auto r : expandedRows)
                {
                    int minY = std::min(source_y, dest_y);
                    int maxY = std::max(source_y, dest_y);
                    if (r > minY && r < maxY)
                    {
                        dist += (expansionFactor - 1);
                    }
                }

                for (auto c : expandedCols)
                {
                    int minX = std::min(source_x, dest_x);
                    int maxX = std::max(source_x, dest_x);
                    if (c > minX && c < maxX)
                    {
                        dist += (expansionFactor - 1);
                    }
                }

                util::verbose_print("source %d, dest %d, distance=%d\n",
                                    sourceId, destId, dist);

                sum += dist;

                ++destId;
            }
            ++sourceId;
        }

        return sum;
    }

    void solve_part1(std::ifstream &input)
    {
        std::cout << solve(input, 2) << std::endl;
    }

    void solve_part2(std::ifstream &input)
    {
        std::cout << solve(input, 1e6) << std::endl;
    }
}