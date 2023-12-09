#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <deque>
#include "../util/print.h"

namespace aoc9
{
    std::vector<std::vector<int>> parse(std::ifstream &input)
    {
        std::vector<std::vector<int>> out;
        std::string line;
        while (std::getline(input, line))
        {
            std::vector<int> seq;
            int value;
            std::istringstream lineStream(line);

            while (lineStream >> value)
                seq.push_back(value);

            for (auto n : seq)
                util::verbose_print("%d ", n);

            util::verbose_print("\n");

            out.push_back(seq);
        }

        return out;
    }

    std::pair<int, int> extrapolate(const std::vector<int> &seq)
    {
        std::map<int, std::deque<int>> diffs;
        for (auto s : seq)
            diffs[0].push_back(s);
        int depth = 0;
        for (;; ++depth)
        {
            const std::deque<int> &currSeq = diffs.at(depth);

            std::deque<int> nextSeq;
            bool allZero = true;
            for (size_t i = 1; i < currSeq.size(); ++i)
            {
                nextSeq.push_back(currSeq[i] - currSeq[i - 1]);
                if (nextSeq.back() != 0)
                    allZero = false;
            }

            if (allZero)
                break;
            else
                diffs[depth + 1] = std::move(nextSeq);
        }

        util::verbose_print("went %d levels deep before reaching all zeros\n", depth);

        // extrapolate now that we have the diffs
        auto &d = diffs.at(depth);
        d.push_back(d.back());
        d.push_front(d.front());
        for (; depth > 0; --depth)
        {
            const auto &curr = diffs.at(depth);
            auto &prev = diffs.at(depth - 1);

            prev.push_front(prev.front() - curr.front());
            prev.push_back(prev.back() + curr.back());
        }

        const auto &finalSeq = diffs.at(0);
        util::verbose_print("extrapolated values: front: %d, back: %d\n", finalSeq.front(), finalSeq.back());

        return {finalSeq.front(), finalSeq.back()};
    }

    void solve_part1(std::ifstream &input)
    {
        auto data = parse(input);
        int sum = 0;
        for (auto seq : data)
        {
            auto [back, fwd] = extrapolate(seq);
            sum += fwd;
        }

        std::cout << sum << std::endl;
    }

    void solve_part2(std::ifstream &input)
    {
        auto data = parse(input);
        int sum = 0;
        for (auto seq : data)
        {
            auto [back, fwd] = extrapolate(seq);
            sum += back;
        }

        std::cout << sum << std::endl;
    }

}