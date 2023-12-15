#include "../util/print.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <array>
#include <utility>
#include <vector>

namespace aoc15
{

    void solve_part1(std::ifstream &input)
    {
        int sum = 0;
        int hash = 0;
        char currChar;
        while (input.get(currChar))
        {
            if (currChar != ',')
            {
                hash += currChar;
                hash *= 17;
                hash %= 256;
            }

            if (currChar == ',' || input.peek() == EOF)
            {
                util::verbose_print("%d\n", hash);
                sum += hash;
                hash = 0;
            }
        }

        std::cout << sum << std::endl;
    }

    void solve_part2(std::ifstream &input)
    {
        std::array<std::vector<std::pair<std::string, int>>, 256> buckets;

        std::string opDescriptor;
        while (std::getline(input, opDescriptor, ','))
        {
            const size_t opIndex = opDescriptor.find_first_of("=-");
            const std::string label = opDescriptor.substr(0, opIndex);
            const char op = opDescriptor[opIndex];

            int hash = 0;
            for (char c : label)
            {
                hash += c;
                hash *= 17;
                hash %= 256;
            }

            util::verbose_print("label=%s hash=%d operator=%c\n", label.c_str(), hash, op);

            if (op == '-')
            {
                auto &b = buckets[hash];
                auto erased = std::erase_if(b, [label](const auto &e)
                                            { return e.first == label; });
                assert(erased <= 1);
            }
            else
            {
                assert(op == '=');
                assert(opDescriptor.size() == opIndex + 2);
                int operand = std::stoi(opDescriptor.substr(opIndex + 1, 1));

                auto &b = buckets[hash];
                auto elemIt = std::find_if(b.begin(), b.end(), [&label](const auto &e)
                                           { return e.first == label; });
                if (elemIt != b.end() && elemIt->first == label)
                {
                    elemIt->second = operand;
                }
                else
                {
                    b.push_back(std::make_pair(label, operand));
                }
            }
        }
        int sum = 0;
        for (size_t i = 0; i < buckets.size(); ++i)
        {
            const auto &b = buckets[i];
            for (size_t j = 0; j < b.size(); ++j)
            {
                const int focusPow = (i + 1) * (j + 1) * b[j].second;
                util::verbose_print("%s = %d\n", b[j].first.c_str(), focusPow);

                sum += focusPow;
            }
        }

        std::cout << sum << std::endl;
    }

}