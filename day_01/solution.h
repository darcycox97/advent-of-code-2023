#pragma once

#include <fstream>
#include <iostream>
#include <algorithm>
#include <stack>
#include <map>
#include <set>

#include "../util/print.h"

namespace aoc1
{

    const std::array<std::string, 9> DIGITS_AS_TEXT{
        "one",
        "two",
        "three",
        "four",
        "five",
        "six",
        "seven",
        "eight",
        "nine"};

    const int MAX_DIGIT_LEN = 5;

    void solve_part1(std::ifstream &input)
    {
        std::string line;
        int sum = 0;
        while (std::getline(input, line))
        {
            int d1 = *std::find_if(line.begin(), line.end(), [](char c)
                                   { return isdigit(c); }) -
                     '0';
            int d2 = *std::find_if(line.rbegin(), line.rend(), [](char c)
                                   { return isdigit(c); }) -
                     '0';

            int combined = d1 * 10 + d2;
            sum += combined;

            util::verbose_print("first digit: %d. second digit: %d. combined: %d\n", d1, d2, combined);
        }

        std::cout << sum << std::endl;
    }

    void solve_part2(std::ifstream &input)
    {
        // same as above but the first "digit" can be spelt out with text.

        // DIGITS_AS_TEXT, and MAX_DIGIT_LEN

        std::string line;
        int sum = 0;
        while (std::getline(input, line))
        {
            int firstDigit = -1;
            int lastDigit = -1;

            for (int i = 0; i < (int)line.size(); ++i)
            {
                int d = -1;
                if (isdigit(line[i]))
                {
                    d = line[i] - '0';
                }
                else
                {
                    for (int j = i + 1; j < (int)line.size() && j - i + 1 <= MAX_DIGIT_LEN; ++j)
                    {
                        std::string candidate = line.substr(i, j - i + 1);
                        const auto it = std::find(DIGITS_AS_TEXT.begin(), DIGITS_AS_TEXT.end(), candidate);
                        if (it != DIGITS_AS_TEXT.end())
                        {
                            d = std::distance(DIGITS_AS_TEXT.begin(), it) + 1;
                            break;
                        }
                    }
                }

                if (d != -1)
                {
                    if (firstDigit == -1)
                        firstDigit = d;
                    lastDigit = d;
                }
            }

            if (firstDigit < 1 || lastDigit < 1)
                throw std::runtime_error("bad input!");

            const int combined = firstDigit * 10 + lastDigit;
            util::verbose_print("first digit: %d. second digit: %d. combined: %d\n", firstDigit, lastDigit, combined);
            sum += combined;
        }

        std::cout << sum << std::endl;
    }

}