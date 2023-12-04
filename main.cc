#include <iostream>
// #include "nest/test.h"

#include <fstream>
#include "day_01/solution.h"
#include "day_02/solution.h"
#include "day_03/solution.h"

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

    printf("Running day %d part %d with input file %s. verbose=%s\n", day, variant, input_filename, verbose ? "true" : "false");

    std::ifstream input_file;
    input_file.open(input_filename, std::ios::in);

    if (day == 1)
    {
        if (variant == 1)
            aoc::solve1_part1(input_file, verbose);
        else
            aoc::solve1_part2(input_file, verbose);
    }
    else if (day == 2)
    {
        if (variant == 1)
            aoc::solve2_part1(input_file, verbose);
        else
            aoc::solve2_part2(input_file, verbose);
    }
    else if (day == 3)
    {
        if (variant == 1)
            aoc::solve3_part1(input_file, verbose);
        else
            aoc::solve3_part2(input_file, verbose);
    }

    input_file.close();

    return 0;
}
