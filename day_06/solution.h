#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <cassert>
#include <cmath>

namespace aoc6
{

    struct BoatRace
    {
        long long mDuration;
        long long mRecordDistance;
    };

    std::vector<BoatRace> parse(std::ifstream &input, bool verbose)
    {
        std::vector<int> durations;
        std::vector<int> records;

        std::string line;
        std::getline(input, line);

        std::istringstream lineStream(line);
        lineStream >> line; // skip past "time"

        int v;
        while (lineStream >> v)
            durations.push_back(v);

        std::getline(input, line);

        lineStream = std::istringstream(line);
        lineStream >> line; // skip past "distances"

        while (lineStream >> v)
            records.push_back(v);

        if (verbose)
            printf("parsed %d races\n", (int)durations.size());
        assert(durations.size() == records.size());

        std::vector<BoatRace> out;
        for (size_t i = 0; i < durations.size(); ++i)
        {
            out.push_back({durations[i], records[i]});
        }

        return out;
    }

    long long solve_race(BoatRace race)
    {
        double a = -1;
        double b = race.mDuration;
        double c = -race.mRecordDistance;

        double sqrt_d = sqrt(b * b - 4 * a * c);

        double t1 = (-b - sqrt_d) / (2 * a);
        double t2 = (-b + sqrt_d) / (2 * a);

        assert(t1 >= 0 && t1 <= race.mDuration);
        assert(t2 >= 0 && t2 <= race.mDuration);
        assert(t1 != t2);

        if (t1 > t2)
            std::swap(t1, t2);

        long long t1Int = floor(t1);
        long long t2Int = ceil(t2);

        return t2Int - t1Int - 1;
    }

    void solve6_part1(std::ifstream &input, bool verbose)
    {
        auto races = parse(input, verbose);

        /*
          determine number of ways you can beat the record. multiply this value together across all races

          r = the rate we gain speed
          t = the time we gain speed for
          d = race duration
          distance = r * t * (d - t)

          in standard quadratic form we get:
          distance = -r * t^2 + r * d * t

          note in this case r = 1
        */

        long long mult = 1;
        for (const auto &race : races)
        {
            mult *= solve_race(race);
        }

        std::cout << mult << std::endl;
    }

    void solve6_part2(std::ifstream &input, bool verbose)
    {
        // there's really just one race. all the numbers are bunched up.

        auto races = parse(input, verbose);

        // combine races
        std::string duration;
        std::string record;
        for (const auto &r : races)
        {
            duration += std::to_string(r.mDuration);
            record += std::to_string(r.mRecordDistance);
        }

        if (verbose)
            printf("duration=%s record=%s\n", duration.c_str(), record.c_str());

        BoatRace r{std::stoll(duration), std::stoll(record)};
        std::cout << solve_race(r) << std::endl;
    }

}