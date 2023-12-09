#include <fstream>
#include <iostream>
#include <sstream>
#include <map>
#include <set>
#include <numeric>
#include "../util/print.h"

namespace aoc8
{
    enum class Direction
    {
        LEFT,
        RIGHT
    };

    struct Node
    {
        std::string mLeft;
        std::string mRight;

        const std::string &operator[](Direction d)
        {
            return d == Direction::LEFT ? mLeft : mRight;
        }
    };

    struct Input
    {
        std::vector<Direction> mDirections;
        std::map<std::string, Node> mNodes;
    };

    Input parse(std::ifstream &input)
    {
        Input res;

        std::string line;
        std::getline(input, line);

        for (char c : line)
        {
            assert(c == 'L' || c == 'R');
            res.mDirections.push_back(c == 'L' ? Direction::LEFT : Direction::RIGHT);
        }

        std::getline(input, line); // skip empty line after the directions

        while (std::getline(input, line))
        {
            std::istringstream lineStream(line);

            std::string nodeLabel;
            std::string token;

            lineStream >> nodeLabel;

            lineStream >> token; // = sign

            Node n;
            lineStream >> token; // (<left>,
            n.mLeft = token.substr(1, 3);
            assert(n.mLeft.size() == 3);
            lineStream >> token; // <right>)
            n.mRight = token.substr(0, 3);
            assert(n.mRight.size() == 3);

            util::verbose_print("%s: left=%s, right=%s\n", nodeLabel.c_str(), n.mLeft.c_str(), n.mRight.c_str());

            [[maybe_unused]] auto it = res.mNodes.insert(std::make_pair(nodeLabel, n));
            assert(it.second);
        }

        return res;
    }

    void solve_part1(std::ifstream &input)
    {
        auto puzzle = parse(input);

        size_t directionInd = 0;
        int count = 0;
        std::string node = "AAA";
        while (node != "ZZZ")
        {
            auto dir = puzzle.mDirections[directionInd];
            ++directionInd;
            if (directionInd == puzzle.mDirections.size())
                directionInd = 0;

            ++count;

            auto currNode = puzzle.mNodes.at(node);
            node = currNode[dir];
        }

        std::cout << count << std::endl;
    }

    struct Cycle
    {
        std::string mSource;
        int mNonCyclicSteps; // i.e. how long until the cyclic path starts
        int mCycleLength;
        std::set<int> mPaths; // only including the first cycle (and the non-cyclic part before it)
    };

    void solve_part2(std::ifstream &input)
    {
        // start on all nodes ending with A.
        // stop when all nodes end on B.

        // determine the length for each source -> dest pair.
        // lcm of all of them gives

        auto puzzle = parse(input);

        std::set<std::string> startNodes;
        std::set<std::string> endNodes;

        for (const auto &[label, _] : puzzle.mNodes)
        {
            if (label.ends_with('A'))
                startNodes.insert(label);
            else if (label.ends_with('Z'))
                endNodes.insert(label);
        }

        assert(startNodes.size() == endNodes.size());
        assert(!startNodes.empty());

        std::vector<Cycle> sourceInfo;
        for (auto s : startNodes)
        {
            Cycle cycle;
            cycle.mSource = s;

            // visit all the unique end nodes that we possibly can
            // <direction index, node label>
            std::map<std::pair<size_t, std::string>, int> cycleDetector;

            size_t directionInd = 0;
            std::string node = s;
            int journeyIndex = 0;
            while (cycleDetector.insert(std::make_pair(std::make_pair(directionInd, node), journeyIndex)).second)
            {
                if (node.ends_with('Z'))
                {
                    util::verbose_print("found new path from %s to %s: steps = %d\n", s.c_str(), node.c_str(), journeyIndex);
                    cycle.mPaths.insert(journeyIndex);
                }

                auto dir = puzzle.mDirections[directionInd];

                std::string prevNode = node;
                node = puzzle.mNodes.at(node)[dir];
                util::verbose_print("moving from node %s to %s, directionInd=%llu\n", prevNode.c_str(), node.c_str(), directionInd);

                ++journeyIndex;
                ++directionInd;
                if (directionInd == puzzle.mDirections.size())
                    directionInd = 0;
            }

            // we're now at the beginning of the cycle. record some info about it to help with the math later
            const auto &cycleStart = cycleDetector.at(std::make_pair(directionInd, node));
            cycle.mNonCyclicSteps = cycleStart;
            cycle.mCycleLength = journeyIndex - cycleStart;

            util::verbose_print("Finished cycle detection for node %s. Cycle start = %d, cycle length = %d, total steps = %d, total paths = %d\n",
                                s.c_str(), cycle.mNonCyclicSteps, cycle.mCycleLength, journeyIndex, cycle.mPaths.size());

            sourceInfo.push_back(cycle);
        }

        /*
            conveniently, all sources have a single path to a destination.
            the destination occurs at the beginning of a loop of length l.
            so if it takes us n steps to get from source s to dest d,
            it will take us n + l steps, n + 2l, n + 3l steps to get back to d, etc.

            also very conveniently, for all paths in this input, n == l.

            So the question just becomes what's the lowest common multiple of all loop lenghths?
            i.e. when will all of the loops land at the exact same spot.
        */

        int64_t acc_lcm = 1;
        for (auto s : sourceInfo)
            acc_lcm = std::lcm(acc_lcm, s.mCycleLength);

        std::cout << acc_lcm << std::endl;
    }
}