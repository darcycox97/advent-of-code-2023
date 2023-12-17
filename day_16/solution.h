#include "../util/print.h"
#include <iostream>
#include <fstream>
#include <set>
#include <deque>

namespace aoc16
{
    using Grid = std::vector<std::vector<char>>;

    Grid parse(std::ifstream &input)
    {
        Grid g;
        std::string line;
        while (std::getline(input, line))
        {
            std::vector<char> row;
            std::copy(line.begin(), line.end(), std::back_inserter(row));
            g.push_back(row);
        }
        return g;
    }

    bool check_bounds(const Grid &g, std::pair<int, int> p)
    {
        return p.first >= 0 && p.second >= 0 && p.first < (int)g.size() && p.second < (int)g.back().size();
    }

    int count_energized_tiles(const Grid &grid, std::pair<int, int> initialSource, char initialDir)
    {
        std::set<std::pair<int, int>> energizedTiles;
        std::deque<std::pair<std::pair<int, int>, char>> lightSources;
        lightSources.push_back({initialSource, initialDir});

        for (size_t sourceInd = 0; sourceInd < lightSources.size(); ++sourceInd)
        {
            auto [sourceLoc, sourceDir] = lightSources[sourceInd];
            assert(sourceDir == 'N' || sourceDir == 'S' || sourceDir == 'E' || sourceDir == 'W');

            bool vertical = sourceDir == 'N' || sourceDir == 'S';
            int step = sourceDir == 'S' || sourceDir == 'E' ? 1 : -1;
            // assume we already added to source to our energized tiles when we added it to the sources list.
            // so move one tile initially
            std::pair<int, int> currPoint = vertical ? std::make_pair(sourceLoc.first + step, sourceLoc.second) : std::make_pair(sourceLoc.first, sourceLoc.second + step);
            while (check_bounds(grid, currPoint))
            {
                energizedTiles.insert(currPoint);

                char currChar = grid[currPoint.first][currPoint.second];
                bool canPassThrough = currChar == '.' || (currChar == '-' && !vertical) || (currChar == '|' && vertical);
                if (canPassThrough)
                {
                    // move to the next
                    if (vertical)
                    {
                        currPoint.first += step;
                    }
                    else
                    {
                        currPoint.second += step;
                    }

                    continue;
                }

                assert(currChar != '.');
                assert(!canPassThrough);

                // we've hit a splitter or mirror, add the new light sources.
                std::vector<char> nextDirs;
                if (currChar == '\\')
                {
                    if (sourceDir == 'N')
                        nextDirs = {'W'};
                    else if (sourceDir == 'E')
                        nextDirs = {'S'};
                    else if (sourceDir == 'S')
                        nextDirs = {'E'};
                    else
                        nextDirs = {'N'};
                }
                else if (currChar == '/')
                {
                    if (sourceDir == 'N')
                        nextDirs = {'E'};
                    else if (sourceDir == 'E')
                        nextDirs = {'N'};
                    else if (sourceDir == 'S')
                        nextDirs = {'W'};
                    else
                        nextDirs = {'S'};
                }
                else if (currChar == '-')
                {
                    assert(vertical); // otherwise we should have already moved on to the next loop
                    nextDirs.push_back('W');
                    nextDirs.push_back('E');
                }
                else if (currChar == '|')
                {
                    assert(!vertical); // otherwise we should have already moved on to the next loop
                    nextDirs.push_back('N');
                    nextDirs.push_back('S');
                }

                for (char dir : nextDirs)
                {
                    auto nextSource = std::make_pair(currPoint, dir);
                    if (std::find(lightSources.begin(), lightSources.end(), nextSource) == lightSources.end())
                    {
                        util::verbose_print("new light source at {%d, %d}: direction=%c\n", currPoint.first, currPoint.second, dir);
                        lightSources.push_back(nextSource);
                    }
                    else
                    {
                        util::verbose_print("re-encountered light source at {%d, %d}: direction=%c. Skipping.\n", currPoint.first, currPoint.second, dir);
                    }
                }

                break;
            }
        }

        return energizedTiles.size();
    }

    void solve_part1(std::ifstream &input)
    {
        auto grid = parse(input);

        // light enters from the top left
        std::cout << count_energized_tiles(grid, {0, -1}, 'E');
    }

    void solve_part2(std::ifstream &input)
    {
        auto grid = parse(input);

        // find the maximum number of energized tiles given the initial source can be anywhere.
        int maxTiles = 0;

        for (size_t row = 0; row < grid.size(); ++row)
        {
            // from the left
            maxTiles = std::max(maxTiles, count_energized_tiles(grid, {row, -1}, 'E'));

            // from the right
            maxTiles = std::max(maxTiles, count_energized_tiles(grid, {row, grid.back().size()}, 'W'));
        }

        for (size_t col = 0; col < grid.back().size(); ++col)
        {
            // from above
            maxTiles = std::max(maxTiles, count_energized_tiles(grid, {-1, col}, 'S'));

            // from below
            maxTiles = std::max(maxTiles, count_energized_tiles(grid, {grid.size(), col}, 'N'));
        }

        std::cout << maxTiles << std::endl;
    }
}