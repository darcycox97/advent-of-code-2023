#include "../util/print.h"
#include <iostream>
#include <fstream>

namespace aoc14
{
    using Platform = std::vector<std::vector<char>>;

    enum class Direction
    {
        N,
        S,
        E,
        W
    };

    std::pair<int, int> find_cycle(const std::vector<int> &sequence)
    {
        int n = sequence.size();

        for (int start = 0; start < n; ++start)
        {
            for (int cycle_length = 1; cycle_length <= (n - start) / 2; ++cycle_length)
            {
                bool is_cycle = true;
                for (int i = start; i < n - cycle_length; ++i)
                {
                    if (sequence[i] != sequence[i + cycle_length])
                    {
                        is_cycle = false;
                        break;
                    }
                }
                if (is_cycle)
                {
                    return {start, cycle_length};
                }
            }
        }
        return {-1, -1}; // No cycle found
    }

    void print_platform(const Platform &p)
    {
        util::verbose_print("\n");

        for (const auto &row : p)
        {
            for (const auto c : row)
                util::verbose_print("%c", c);
            util::verbose_print("\n");
        }

        util::verbose_print("\n");
    }

    void roll_platform(Platform &p, Direction dir)
    {
        size_t start, end; // [start, end)
        int step;
        switch (dir)
        {
        case Direction::N:
            start = 0;
            end = p.size();
            step = 1;
            break;
        case Direction::S:
            start = p.size() - 1;
            end = -1;
            step = -1;
            break;
        case Direction::W:
            start = 0;
            end = p.back().size();
            step = 1;
            break;
        case Direction::E:
            start = p.back().size() - 1;
            end = -1;
            step = -1;
            break;
        }

        // # = cube. O = ball, . = ground
        const bool vertical = dir == Direction::N || dir == Direction::S;
        size_t i_end = vertical ? p.back().size() : p.size();
        for (size_t i = 0; i < i_end; ++i)
        {
            size_t lastBarrier = start - step;
            for (size_t j = start; j != end; j += step)
            {
                const bool jIsRow = vertical;
                char &curr = p[jIsRow ? j : i][jIsRow ? i : j];
                if (curr == '#')
                {
                    lastBarrier = j;
                }
                else if (curr == 'O')
                {
                    lastBarrier += step;
                    auto &onePastBarrier = p[jIsRow ? lastBarrier : i][jIsRow ? i : lastBarrier];
                    std::swap(onePastBarrier, curr);
                }
            }
        }
    }

    int calc_load(const Platform &p)
    {
        int load = 0;
        for (size_t row = 0; row < p.size(); ++row)
        {
            for (size_t col = 0; col < p.back().size(); ++col)
            {
                if (p[row][col] == 'O')
                {
                    load += p.size() - row;
                }
            }
        }
        return load;
    }

    Platform parse_platform(std::ifstream &input)
    {
        Platform out;
        std::string line;
        while (std::getline(input, line))
        {
            std::vector<char> row;
            std::copy(line.begin(), line.end(), std::back_inserter(row));
            out.push_back(row);
        }

        print_platform(out);
        return out;
    }

    void solve_part1(std::ifstream &input)
    {
        auto platform = parse_platform(input);

        roll_platform(platform, Direction::N);
        print_platform(platform);

        std::cout << calc_load(platform) << std::endl;
    }

    void solve_part2(std::ifstream &input)
    {
        // each cycle rolls 4 times: N, W, S, E
        // find the load after 1000000000 cycles.

        auto platform = parse_platform(input);

        // run on a small-ish sample size to hopefully detect a pattern.
        // assume this pattern continues up to 1 billion cycles.
        std::vector<int> loads;
        for (size_t i = 0; i < 1000; ++i)
        {
            roll_platform(platform, Direction::N);
            roll_platform(platform, Direction::W);
            roll_platform(platform, Direction::S);
            roll_platform(platform, Direction::E);

            loads.push_back(calc_load(platform));
        }

        auto [seqStart, seqLen] = find_cycle(loads);

        assert(loads.at(seqStart) == loads.at(seqStart + seqLen));
        assert(loads.at(seqStart) == loads.at(seqStart + 2 * seqLen));
        util::verbose_print("found cycle: startIndex=%d, start=%d, end=%d, length=%d\n", seqStart, loads.at(seqStart), loads.at(seqStart + seqLen - 1), seqLen);

        util::verbose_print("\nloads:\n");
        for (auto l : loads)
            util::verbose_print("%d,", l);
        util::verbose_print("\n");

        int totalCycles = 1e9;
        int stateIndex = seqStart + ((totalCycles - seqStart - 1) % seqLen);

        std::cout << loads.at(stateIndex) << std::endl;
    }

}