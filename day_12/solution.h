#include "../util/print.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

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

    using CacheKey = std::pair<std::string, std::vector<int>>;
    using Cache = std::map<CacheKey, uint64_t>;

    uint64_t get_unique_combinations(std::string canvas, std::vector<int> structure, int &cacheMisses, Cache &cache)
    {
        CacheKey k{canvas, structure};
        if (cache.contains(k))
        {
            util::verbose_print("cache hit for %s\n", canvas.c_str());
            return cache.at(k);
        }

        ++cacheMisses;

        if (canvas.empty())
            return structure.empty();

        if (structure.empty())
            return canvas.find('#') == std::string::npos;

        if (canvas[0] == '?')
        {
            uint64_t ways = 0;
            canvas[0] = '.';
            ways += get_unique_combinations(canvas, structure, cacheMisses, cache);
            canvas[0] = '#';
            ways += get_unique_combinations(canvas, structure, cacheMisses, cache);
            cache[k] = ways;
            return ways;
        }

        if (canvas[0] == '.')
        {
            uint64_t ways = get_unique_combinations(canvas.substr(1, canvas.size() - 1), structure, cacheMisses, cache);
            cache[k] = ways;
            return ways;
        }

        assert(canvas[0] == '#');

        int contiguous = 0;
        for (char c : canvas)
        {
            if (contiguous == structure[0] && c == '#')
            {
                return 0; // invalid because we've got too many contiguous hashes
            }
            else if ((contiguous == structure[0] && c == '?'))
            {
                assert(canvas[contiguous] == '?');
                canvas[contiguous] = '.'; // the only valid option. another # would extend the contiguous group
                break;
            }
            else if (c == '.')
            {
                break;
            }

            ++contiguous;
        }

        if (contiguous < structure[0])
            return 0; // not enough to fill the required structure

        assert(contiguous == structure[0]);
        std::vector<int> reducedStructure(std::next(structure.begin()), structure.end());
        uint64_t ways = get_unique_combinations(canvas.substr(contiguous, canvas.size() - contiguous), reducedStructure, cacheMisses, cache);
        cache[k] = ways;
        return ways;
    }

    void solve_part1(std::ifstream &input)
    {
        auto problem = parse(input);

        uint64_t total = 0;
        for (auto &[canvas, structure] : problem)
        {
            Cache cache;
            int totalCalls = 0;
            uint64_t ways = get_unique_combinations(canvas, structure, totalCalls, cache);
            total += ways;

            std::string structStr;
            for (auto s : structure)
            {
                structStr += std::to_string(s);
                structStr += " ";
            }

            util::verbose_print("%llu ways to satisfy %s %s\n", ways, canvas.c_str(), structStr.c_str());
            util::verbose_print("called get_unique_combinations %d times for canvas %s\n", totalCalls, canvas.c_str());
        }

        std::cout << total << std::endl;
    }

    void solve_part2(std::ifstream &input)
    {
        uint64_t total = 0;
        auto problem = parse(input);
        for (auto [canvas, structure] : problem)
        {
            Cache cache;
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
            total += get_unique_combinations(unfoldedCanvas, unfoldedStructure, totalCalls, cache);
            util::verbose_print("called get_unique_combinations %d times for %s\n", totalCalls, unfoldedCanvas.c_str());
        }

        std::cout << total << std::endl;
    }

}