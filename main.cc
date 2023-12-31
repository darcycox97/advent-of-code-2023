#include <iostream>
#include <fstream>

#include "util/print.h"

#include "day_01/solution.h"
#include "day_02/solution.h"
#include "day_03/solution.h"
#include "day_04/solution.h"
#include "day_05/solution.h"
#include "day_06/solution.h"
#include "day_07/solution.h"
#include "day_08/solution.h"
#include "day_09/solution.h"
#include "day_10/solution.h"
#include "day_11/solution.h"
#include "day_12/solution.h"
#include "day_14/solution.h"
#include "day_15/solution.h"
#include "day_16/solution.h"

int main(int argc, char **argv)
{
    if (argc < 4)
    {
        std::cout << "Usage: " << argv[0] << " <day> <variant(1|2)> <path_to_input> [-v]" << std::endl;
        return 1;
    }

    const int day = std::stoi(argv[1]);
    const int variant = std::stoi(argv[2]);
    const char *input_filename = argv[3];

    bool verbose = false;
    if (argc == 5 && strcmp(argv[4], "-v") == 0)
        verbose = true;

    util::set_verbose(verbose);

    printf("Running day %d part %d with input file %s. verbose=%s\n", day, variant, input_filename, verbose ? "true" : "false");

    std::ifstream input_file;
    input_file.open(input_filename, std::ios::in);

    if (day == 1)
    {
        if (variant == 1)
            aoc1::solve_part1(input_file);
        else
            aoc1::solve_part2(input_file);
    }
    else if (day == 2)
    {
        if (variant == 1)
            aoc2::solve_part1(input_file);
        else
            aoc2::solve_part2(input_file);
    }
    else if (day == 3)
    {
        if (variant == 1)
            aoc3::solve_part1(input_file);
        else
            aoc3::solve_part2(input_file);
    }
    else if (day == 4)
    {
        if (variant == 1)
            aoc4::solve_part1(input_file);
        else
            aoc4::solve_part2(input_file);
    }
    else if (day == 5)
    {
        if (variant == 1)
            aoc5::solve_part1(input_file);
        else
            aoc5::solve_part2(input_file);
    }
    else if (day == 6)
    {
        if (variant == 1)
            aoc6::solve_part1(input_file);
        else
            aoc6::solve_part2(input_file);
    }
    else if (day == 7)
    {
        if (variant == 1)
            aoc7::solve_part1(input_file);
        else
            aoc7::solve_part2(input_file);
    }
    else if (day == 8)
    {
        if (variant == 1)
            aoc8::solve_part1(input_file);
        else
            aoc8::solve_part2(input_file);
    }
    else if (day == 9)
    {
        if (variant == 1)
            aoc9::solve_part1(input_file);
        else
            aoc9::solve_part2(input_file);
    }
    else if (day == 10)
    {
        if (variant == 1)
            aoc10::solve_part1(input_file);
        else
            aoc10::solve_part2(input_file);
    }
    else if (day == 11)
    {
        if (variant == 1)
            aoc11::solve_part1(input_file);
        else
            aoc11::solve_part2(input_file);
    }
    else if (day == 12)
    {
        if (variant == 1)
            aoc12::solve_part1(input_file);
        else
            aoc12::solve_part2(input_file);
    }
    else if (day == 14)
    {
        if (variant == 1)
            aoc14::solve_part1(input_file);
        else
            aoc14::solve_part2(input_file);
    }
    else if (day == 15)
    {
        if (variant == 1)
            aoc15::solve_part1(input_file);
        else
            aoc15::solve_part2(input_file);
    }
    else if (day == 16)
    {
        if (variant == 1)
            aoc16::solve_part1(input_file);
        else
            aoc16::solve_part2(input_file);
    }

    input_file.close();

    return 0;
}
