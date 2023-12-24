#include "../util/print.h"
#include <iostream>
#include <fstream>
#include <sstream>

namespace aoc12
{

    std::vector<std::pair<std::string, std::vector<int>>> parse(std::ifstream &input)
    {
        std::vector<std::pair<std::string, std::vector<int>>> out;
        std::string line;
        while (std::getline(input, line))
        {
            std::istringstream iss(line);
            std::string canvas;
            iss >> canvas;

            std::string num;
            iss >> num; // get the next block of text

            std::vector<int> structure;
            std::istringstream numStream(num);
            while (std::getline(numStream, num, ','))
                structure.push_back(std::stoi(num));

            out.push_back(std::make_pair(canvas, structure));
        }

        return out;
    }

    int get_unique_combinations(std::string canvas, std::vector<int> structure, int &numCalls)
    {
        ++numCalls;
        auto structureIt = structure.begin();
        auto canvasIt = canvas.begin();
        int contigCount = 0;
        for (; canvasIt != canvas.end(); ++canvasIt)
        {
            if (*canvasIt == '#')
            {
                ++contigCount;
                if (structureIt == structure.end() || contigCount > *structureIt)
                    return 0; // invalid configuration
            }
            else if (*canvasIt == '.')
            {
                if (contigCount > 0)
                {
                    if (structureIt == structure.end() || *structureIt != contigCount)
                        return 0; // invalid configuration
                    else
                        ++structureIt; // matched a structure. move to the next
                    contigCount = 0;
                }
            }
            else
            {
                assert(*canvasIt == '?');
                break; // found an unknown
            }
        }

        if (canvasIt == canvas.end())
        {
            // reached the end with no unknowns. return 1 if valid, 0 if not.
            if (structureIt == structure.end())
            {
                return contigCount == 0 ? 1 : 0;
            }
            else if (std::distance(structure.begin(), structureIt) != (int)(structure.size() - 1))
            {
                return 0; // multiple structures remaining that weren't matched
            }
            else
            {
                return *structureIt == contigCount ? 1 : 0;
            }
        }

        int ways = 0;
        *canvasIt = '#';
        ways += get_unique_combinations(canvas, structure, numCalls);
        *canvasIt = '.';
        ways += get_unique_combinations(canvas, structure, numCalls);
        return ways;
    }

    void solve_part1(std::ifstream &input)
    {
        auto problem = parse(input);

        int total = 0;
        for (auto &[canvas, structure] : problem)
        {
            int totalCalls = 0;
            total += get_unique_combinations(canvas, structure, totalCalls);
            util::verbose_print("called get_unique_combinations %d times for canvas %s\n", totalCalls, canvas.c_str());
        }

        std::cout << total << std::endl;
    }

    void solve_part2(std::ifstream &input)
    {
        int total = 0;
        auto problem = parse(input);
        for (auto [canvas, structure] : problem)
        {
            // expand the structure 5 times
            // e.g.
            // original: ##.?? 2, 1
            // unfolded: ##.???##.???##.???##.???##,?? 2, 1, 2, 1, 2, 1, 2, 1, 2, 1

            std::string unfoldedCanvas;
            std::vector<int> unfoldedStructure;
            for (int i = 0; i < 5; ++i)
            {
                unfoldedCanvas += canvas;
                if (i < 4)
                    unfoldedCanvas += "?";

                std::copy(structure.begin(), structure.end(), std::back_inserter(unfoldedStructure));
            }

            int totalCalls = 0;
            util::verbose_print("folded=%s, %llu groups\nunfolded=%s, %llu groups\n", canvas.c_str(), structure.size(), unfoldedCanvas.c_str(), unfoldedStructure.size());
            total += get_unique_combinations(unfoldedCanvas, unfoldedStructure, totalCalls);
            util::verbose_print("called get_unique_combinations %d times for %s\n", totalCalls, unfoldedCanvas.c_str());
        }

        std::cout << total << std::endl;
    }

}