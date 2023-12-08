#include <vector>
#include <map>
#include <cstdint>
#include <sstream>
#include <fstream>
#include <limits>
#include <iostream>

#include "../util/print.h"

namespace aoc5
{
    struct RuleEntry
    {
        int64_t mStart;
        int64_t mEnd;    // inclusive
        int64_t mOffset; // source + offset = destination
    };
    // keyed by upper limit (inclusive)
    using Mapping = std::map<int64_t, RuleEntry>;

    int64_t getSourceToDestOffset(const Mapping &m, int64_t source)
    {
        auto it = m.lower_bound(source);
        if (it == m.end())
            return 0;

        assert(source <= it->second.mEnd);

        if (source >= it->second.mStart)
            return it->second.mOffset;
        else
            return 0;
    }

    struct Data
    {
        std::vector<int64_t> mSeeds;

        // TODO: should really just collapse the mapping into one seed to location map...
        // then it's easier to be smarter abuot which seeds don't need to be checked in part 2.
        Mapping mSeedToSoil;
        Mapping mSoilToFertilizer;
        Mapping mFertilizerToWater;
        Mapping mWaterToLight;
        Mapping mLightToTemperature;
        Mapping mTemperatureToHumidity;
        Mapping mHumidityToLocation;

        int64_t GetLocationFromSeed(int64_t s) const
        {
            // seed to soil
            auto value = s + getSourceToDestOffset(mSeedToSoil, s);

            // soil to fert
            value += getSourceToDestOffset(mSoilToFertilizer, value);

            // fert to water
            value += getSourceToDestOffset(mFertilizerToWater, value);

            // water to light
            value += getSourceToDestOffset(mWaterToLight, value);

            // light to temp
            value += getSourceToDestOffset(mLightToTemperature, value);

            // temp to humidity
            value += getSourceToDestOffset(mTemperatureToHumidity, value);

            // humidity to location
            value += getSourceToDestOffset(mHumidityToLocation, value);

            return value;
        }
    };

    Mapping parseMapping(std::ifstream &input)
    {
        Mapping m;
        std::string line;
        while (isdigit(input.peek()))
        {
            std::getline(input, line);

            int64_t dest;
            int64_t source;
            int64_t length;

            std::istringstream lineStream(line);
            lineStream >> dest;
            lineStream >> source;
            lineStream >> length;

            RuleEntry re{
                .mStart = source,
                .mEnd = source + length - 1,
                .mOffset = dest - source};

            m[re.mEnd] = re;

            util::verbose_print("processed rule: start=%lld end=%lld offset=%lld\n", re.mStart, re.mEnd, re.mOffset);
        }

        return m;
    }

    Data parse(std::ifstream &input)
    {
        Data out;

        std::string line;
        input >> line; // skip past "seeds"

        int64_t seed;
        while (input >> seed)
            out.mSeeds.push_back(seed);

        util::verbose_print("parsed %lu seeds\n", out.mSeeds.size());

        input.clear(); // reset the error state
        while (std::getline(input, line))
        {
            if (line.find("seed-to-soil map:") != std::string::npos)
            {

                util::verbose_print("parsing seed to soil mapping\n");

                out.mSeedToSoil = parseMapping(input);
            }
            else if (line.find("soil-to-fertilizer map:") != std::string::npos)
            {

                util::verbose_print("parsing soil to fertilizer mapping\n");

                out.mSoilToFertilizer = parseMapping(input);
            }
            else if (line.find("fertilizer-to-water map:") != std::string::npos)
            {

                util::verbose_print("parsing fertilizer to water mapping\n");

                out.mFertilizerToWater = parseMapping(input);
            }
            else if (line.find("water-to-light map:") != std::string::npos)
            {

                util::verbose_print("parsing water to light mapping\n");

                out.mWaterToLight = parseMapping(input);
            }
            else if (line.find("light-to-temperature map:") != std::string::npos)
            {

                util::verbose_print("parsing light to temp mapping\n");

                out.mLightToTemperature = parseMapping(input);
            }
            else if (line.find("temperature-to-humidity map:") != std::string::npos)
            {

                util::verbose_print("parsing temp to humidity mapping\n");

                out.mTemperatureToHumidity = parseMapping(input);
            }
            else if (line.find("humidity-to-location map:") != std::string::npos)
            {

                util::verbose_print("parsing humidity to location mapping\n");

                out.mHumidityToLocation = parseMapping(input);
            }
        }

        return out;
    }

    void solve5_part1(std::ifstream &input)
    {
        Data d = parse(input);

        int64_t minLocation = std::numeric_limits<int64_t>::max();
        for (auto s : d.mSeeds)
        {
            auto value = d.GetLocationFromSeed(s);
            if (value < minLocation)
                minLocation = value;
        }

        std::cout << minLocation << std::endl;
    }

    void solve5_part2(std::ifstream &input)
    {
        Data d = parse(input);

        // TODO: this is inefficient. should determine which ranges of seeds have different mappings.
        // for any ranges with the same mapping, the first one in the mapping will always have the lowest location.
        // so we can skip a huge number of seeds.

        // seeds sequence now represents ranges with a source and a length.
        int64_t minLocation = std::numeric_limits<int64_t>::max();
        for (int i = 0; i < (int)d.mSeeds.size(); i += 2)
        {
            auto start = d.mSeeds[i];
            auto len = d.mSeeds[i + 1];

            util::verbose_print("processing %lld seeds from %lld to %lld\n", len, start, start + len - 1);
            for (auto s = start; s < start + len; ++s)
            {
                auto value = d.GetLocationFromSeed(s);
                if (value < minLocation)
                    minLocation = value;
            }
        }
        std::cout << minLocation << std::endl;
    }
}