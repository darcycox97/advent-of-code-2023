#include <iostream>
#include <fstream>

#include "util/print.h"

#include "day_01/solution.h"
#include "day_02/solution.h"
#include "day_03/solution.h"
#include "day_04/solution.h"
#include "day_05/solution.h"
#include "day_06/solution.h"

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
            aoc1::solve1_part1(input_file);
        else
            aoc1::solve1_part2(input_file);
    }
    else if (day == 2)
    {
        if (variant == 1)
            aoc2::solve2_part1(input_file);
        else
            aoc2::solve2_part2(input_file);
    }
    else if (day == 3)
    {
        if (variant == 1)
            aoc3::solve3_part1(input_file);
        else
            aoc3::solve3_part2(input_file);
    }
    else if (day == 4)
    {
        if (variant == 1)
            aoc4::solve4_part1(input_file);
        else
            aoc4::solve4_part2(input_file);
    }
    else if (day == 5)
    {
        if (variant == 1)
            aoc5::solve5_part1(input_file);
        else
            aoc5::solve5_part2(input_file);
    }
    else if (day == 6)
    {
        if (variant == 1)
            aoc6::solve6_part1(input_file);
        else
            aoc6::solve6_part2(input_file);
    }

    input_file.close();

    return 0;
}
