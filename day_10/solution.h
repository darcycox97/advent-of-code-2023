#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <set>
#include <deque>

#include "../util/print.h"

namespace aoc10
{
    enum class Direction
    {
        UP,
        DOWN,
        RIGHT,
        LEFT
    };

    // 0,0 is top left, W,H is bottom right.
    struct Point
    {
        size_t mY;
        size_t mX;
    };

    bool operator<(const Point &l, const Point &r)
    {
        return l.mX < r.mX || (l.mX == r.mX && l.mY < r.mY);
    }

    using AdjacencyList = std::map<Point, std::set<Point>>;

    struct Graph
    {
        std::vector<std::vector<char>> mRawGrid;
        AdjacencyList mAdjacencyList;
        Point mStart;
    };

    bool connects_down(char c)
    {
        return c == 'F' || c == '|' || c == '7';
    }

    bool connects_up(char c)
    {
        return c == 'L' || c == '|' || c == 'J';
    }

    bool connects_left(char c)
    {
        return c == '7' || c == '-' || c == 'J';
    }

    bool connects_right(char c)
    {
        return c == 'F' || c == '-' || c == 'L';
    }

    AdjacencyList build_connected_ground_tiles(std::vector<std::vector<char>> &grid)
    {
        AdjacencyList out;
        for (size_t y = 1; y < grid.size() - 1; ++y)
        {
            for (size_t x = 1; x < grid.back().size() - 1; ++x)
            {
                char curr = grid[y][x];
                assert(curr == '|' || curr == '-' || curr == 'J' || curr == 'L' || curr == 'F' || curr == '7' || curr == 'S' || curr == '.');

                if (curr != '.')
                    continue;

                auto &neighbours = out[{y, x}];

                char u = grid[y - 1][x];
                char d = grid[y + 1][x];
                char r = grid[y][x + 1];
                char l = grid[y][x - 1];

                if (u == '.')
                    neighbours.insert({y - 1, x});

                if (d == '.')
                    neighbours.insert({y + 1, x});

                if (r == '.')
                    neighbours.insert({y, x + 1});

                if (l == '.')
                    neighbours.insert({y, x - 1});
            }
        }

        return out;
    }

    Graph parse(std::ifstream &input)
    {
        std::vector<std::vector<char>> grid;
        std::string line;
        while (std::getline(input, line))
        {
            grid.push_back({'.'});
            for (char c : line)
                grid.back().push_back(c);
            grid.back().push_back('.');
        }

        std::vector<char> padding;
        std::fill_n(std::back_inserter(padding), grid.back().size(), '.');

        grid.insert(grid.begin(), padding);
        grid.push_back(padding);

        // build the connected graph
        Graph g;
        for (size_t y = 1; y < grid.size() - 1; ++y)
        {
            for (size_t x = 1; x < grid.back().size() - 1; ++x)
            {
                char curr = grid[y][x];
                assert(curr == '|' || curr == '-' || curr == 'J' || curr == 'L' || curr == 'F' || curr == '7' || curr == 'S' || curr == '.');

                auto &neighbours = g.mAdjacencyList[{y, x}];

                char u = grid[y - 1][x];
                char d = grid[y + 1][x];
                char r = grid[y][x + 1];
                char l = grid[y][x - 1];

                if (curr == 'S')
                {
                    util::verbose_print("found start at y=%llu x=%llu\n", y, x);
                    // s is connected to exactly two neighbours. use this to determine what s is.

                    g.mStart = {y, x};

                    bool u_connected = connects_down(u);
                    bool d_connected = connects_up(d);
                    bool r_connected = connects_left(r);
                    bool l_connected = connects_right(l);

                    // S must be one of F, -, 7, J, L, |
                    if (d_connected && r_connected)
                        curr = 'F';
                    else if (l_connected && r_connected)
                        curr = '-';
                    else if (l_connected && d_connected)
                        curr = '7';
                    else if (u_connected && l_connected)
                        curr = 'J';
                    else if (u_connected && r_connected)
                        curr = 'L';
                    else if (u_connected && d_connected)
                        curr = '|';
                    else
                        assert(false && "couldn't determine S value");

                    grid[y][x] = curr;
                    util::verbose_print("start = %c\n", curr);
                }

                if (connects_up(curr) && connects_down(u))
                    neighbours.insert({y - 1, x});

                if (connects_down(curr) && connects_up(d))
                    neighbours.insert({y + 1, x});

                if (connects_right(curr) && connects_left(r))
                    neighbours.insert({y, x + 1});

                if (connects_left(curr) && connects_right(l))
                    neighbours.insert({y, x - 1});
            }
        }

        g.mRawGrid = grid;

        return g;
    }

    void solve_part1(std::ifstream &input)
    {
        auto g = parse(input);

        std::map<Point, uint32_t> distances;
        std::deque<std::pair<Point, Point>> toVisit; // <target, added by>

        for (auto n : g.mAdjacencyList[g.mStart])
            toVisit.push_back({n, g.mStart});

        distances[g.mStart] = 0;

        while (!toVisit.empty())
        {
            auto [n, addedBy] = toVisit.front();
            toVisit.pop_front();

            uint32_t currDist = distances.at(addedBy) + 1;
            auto existingDistIt = distances.find(n);
            if (existingDistIt == distances.end())
                distances[n] = currDist;

            for (auto neighbour : g.mAdjacencyList[n])
            {
                if (distances.find(neighbour) == distances.end())
                    toVisit.push_back({neighbour, n});
            }
        }

        util::verbose_print("done\n");

        // find the furthest point from the start
        auto maxIt = std::max_element(distances.begin(), distances.end(), [](const auto &l, const auto &r)
                                      { return l.second < r.second; });

        assert(maxIt != distances.end());

        std::cout << maxIt->second << std::endl;
    }

    void printgrid(const std::vector<std::vector<char>> &g)
    {
        for (auto rowIt = g.begin(); rowIt != g.end(); ++rowIt)
        {
            for (auto colIt = rowIt->begin(); colIt != rowIt->end(); ++colIt)
            {
                util::verbose_print("%c", *colIt);
            }
            util::verbose_print("\n");
        }
    }

    void solve_part2(std::ifstream &input)
    {
        // find how many tiles (denoted by '.') are enclosed inside the loop
        // note you can go travel between pipes as long as there isn't a connected pipe in the way.

        auto res = parse(input);

        auto &grid = res.mRawGrid;

        // expand for every col/row where there are two pipes that can be squeezed through, insert a new column that adds some ground in the space.
        // this lets us do a graph search algorithm to discover all the accessible points outside the loop.

        printgrid(grid);

        /*
         insert new rows where needed.
         Possible combinations:
         ".", "J", "L" or "-" on top: "." goes below
         "|", "F", "7" on top, "|", "J", "L" on bottom: "|" goes below to maintain the connection
        */
        for (auto rowIt = grid.begin(); rowIt != grid.end(); ++rowIt)
        {
            auto &realRow = *rowIt;
            std::vector<char> interRow;
            interRow.reserve(grid.back().size());
            for (auto colIt = realRow.begin(); colIt != realRow.end(); ++colIt)
            {
                if (colIt != realRow.begin())
                {
                    char left = *std::prev(colIt);
                    char right = *colIt;
                    char interCol = '.';
                    if (connects_right(left) && connects_left(right))
                    {
                        interCol = '-';
                    }
                    colIt = std::next(realRow.insert(colIt, interCol)); // keep colIt pointing to the current col
                    interRow.push_back('.');
                }

                if (rowIt != grid.begin())
                {
                    const auto colInd = std::distance(rowIt->begin(), colIt);
                    char up = (*std::prev(rowIt))[colInd];
                    char down = (*rowIt)[colInd];
                    if (connects_down(up) && connects_up(down))
                    {
                        interRow.push_back('|');
                    }
                    else
                    {
                        interRow.push_back('.');
                    }
                }
            }

            if (rowIt != grid.begin())
            {
                assert(interRow.size() == realRow.size());
                rowIt = std::next(grid.insert(rowIt, std::move(interRow)));
            }
        }

        util::verbose_print("\n\n\n");
        printgrid(grid);

        // find every point in the main loop
        std::set<Point> mainLoop;
        {
            std::set<Point> toVisit;
            Point currPoint{res.mStart.mY * 2, res.mStart.mX * 2};
            util::verbose_print("start: y=%d x=%d char=%c\n", currPoint.mY, currPoint.mX, grid[currPoint.mY][currPoint.mX]);

            toVisit.insert(currPoint);
            while (!toVisit.empty())
            {
                auto visIter = toVisit.begin();
                assert(!mainLoop.contains(*visIter));
                mainLoop.insert(*visIter);

                const auto y = visIter->mY;
                const auto x = visIter->mX;
                const char curr = grid[y][x];
                const char u = grid[y - 1][x];
                const char r = grid[y][x + 1];
                const char d = grid[y + 1][x];
                const char l = grid[y][x - 1];

                auto add = [&mainLoop, &toVisit](auto p)
                {
                    if (!mainLoop.contains(p))
                        toVisit.insert(p);
                };

                if (connects_up(curr) && connects_down(u))
                    add(Point{y - 1, x});
                if (connects_right(curr) && connects_left(r))
                    add(Point{y, x + 1});
                if (connects_down(curr) && connects_up(d))
                    add(Point{y + 1, x});
                if (connects_left(curr) && connects_right(l))
                    add(Point{y, x - 1});

                toVisit.erase(visIter);
            }
        }

        // draw the main loop
        util::verbose_print("\n\nPrinting main loop.\n\n");
        for (size_t y = 0; y < grid.size(); ++y)
        {
            for (size_t x = 0; x < grid.back().size(); ++x)
            {
                if (mainLoop.contains({y, x}))
                {
                    util::verbose_print("%c", grid[y][x]);
                }
                else
                {
                    util::verbose_print(".");
                }
            }
            util::verbose_print("\n");
        }

        // find every reachable point outside the loop. Anything we can't reach must be inside.

        std::set<Point> visited;
        std::set<Point> toVisit;
        toVisit.insert({1, 1});
        while (!toVisit.empty())
        {
            auto vis = *toVisit.begin();
            visited.insert(vis);
            toVisit.erase(vis);

            if (vis.mX <= 0 || vis.mY <= 0 || vis.mX >= grid.back().size() - 1 || vis.mY >= grid.size() - 1)
            {
                // don't add neighbours at the bounds to avoid having to bounds check.
                // we know it will always be a ground tile anyway due to the way we built the grid initially.
                continue;
            }

            // we can visit any neighbour that isn't in the main loop
            auto add = [&mainLoop, &visited, &toVisit](auto p)
            {
                if (!mainLoop.contains(p) && !visited.contains(p))
                    toVisit.insert(p);
            };

            const auto y = vis.mY;
            const auto x = vis.mX;
            add(Point{y - 1, x});
            add(Point{y, x + 1});
            add(Point{y + 1, x});
            add(Point{y, x - 1});
        }

        util::verbose_print("finished discovering ground tiles. visited %llu\n", visited.size());

        // TODO: need to fix the code so that we are able to travel from ground to a neighbour
        // as long as it's not part of the main loop. This is because any tile inside the loop can count

        int totalInLoop = 0;
        util::verbose_print("\n\n\n");
        for (size_t y = 0; y < grid.size(); ++y)
        {
            for (size_t x = 0; x < grid.back().size(); ++x)
            {
                if (y % 2 == 0 && x % 2 == 0 && !visited.contains({y, x}) && !mainLoop.contains({y, x}) && x != 0 && y != 0 && x != grid.back().size() - 1 && y != grid.size() - 1)
                {
                    ++totalInLoop;
                    util::verbose_print("x");
                }
                else
                {
                    util::verbose_print("%c", grid.at(y).at(x));
                }
            }
            util::verbose_print("\n");
        }

        std::cout << totalInLoop << std::endl;
    }

}