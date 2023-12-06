#include <vector>
#include <map>
#include <cstdint>
#include <sstream>
#include <fstream>
#include <limits>

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

    // lower bound
    // 19: 20 not less than 19 -> lower bound = 20
    // 20: 20 not less than 20 -> lower bound = 20
    // 25: 20 less than 25. 40 not less than 25 -> lower bound = 40.

    // upper bound
    // 19: 20 greater than 19 -> upper bound = 20
    // 20: 20 not greater than 20. 40 greater than 20 -> upper bound = 40

    struct Data
    {
        std::vector<int64_t> mSeeds;

        Mapping mSeedToSoil;
        Mapping mSoilToFertilizer;
        Mapping mFertilizerToWater;
        Mapping mWaterToLight;
        Mapping mLightToTemperature;
        Mapping mTemperatureToHumidity;
        Mapping mHumidityToLocation;
    };

    Mapping parseMapping(std::ifstream &input, bool verbose)
    {
        // TODO

        // e.g. 40 20 5 means [20, 24] maps to [40, 44]

        // if we have:
        // [20, 24] -> [40, 44]
        // [35, 40] -> [90, 95]

        // can represent as:
        // 0 -> +0
        // 20 -> +20
        // 25 -> +0
        // 35 -> +55
        // 41 -> +0

        // then map.lower_bound(x) is the offset to apply.
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
            if (verbose)
                printf("processed rule: start=%lld end=%lld offset=%lld\n", re.mStart, re.mEnd, re.mOffset);
        }

        return m;
    }

    Data parse(std::ifstream &input, bool verbose)
    {
        Data out;

        std::string line;
        input >> line; // skip past "seeds"

        int64_t seed;
        while (input >> seed)
            out.mSeeds.push_back(seed);

        if (verbose)
            printf("parsed %lu seeds\n", out.mSeeds.size());

        input.clear(); // reset the error state
        while (std::getline(input, line))
        {
            if (line.find("seed-to-soil map:") != std::string::npos)
            {
                if (verbose)
                    printf("parsing seed to soil mapping\n");

                out.mSeedToSoil = parseMapping(input, verbose);
            }
            else if (line.find("soil-to-fertilizer map:") != std::string::npos)
            {
                if (verbose)
                    printf("parsing soil to fertilizer mapping\n");

                out.mSoilToFertilizer = parseMapping(input, verbose);
            }
            else if (line.find("fertilizer-to-water map:") != std::string::npos)
            {
                if (verbose)
                    printf("parsing fertilizer to water mapping\n");

                out.mFertilizerToWater = parseMapping(input, verbose);
            }
            else if (line.find("water-to-light map:") != std::string::npos)
            {
                if (verbose)
                    printf("parsing water to light mapping\n");

                out.mWaterToLight = parseMapping(input, verbose);
            }
            else if (line.find("light-to-temperature map:") != std::string::npos)
            {
                if (verbose)
                    printf("parsing light to temp mapping\n");

                out.mLightToTemperature = parseMapping(input, verbose);
            }
            else if (line.find("temperature-to-humidity map:") != std::string::npos)
            {
                if (verbose)
                    printf("parsing temp to humidity mapping\n");

                out.mTemperatureToHumidity = parseMapping(input, verbose);
            }
            else if (line.find("humidity-to-location map:") != std::string::npos)
            {
                if (verbose)
                    printf("parsing humidity to location mapping\n");

                out.mHumidityToLocation = parseMapping(input, verbose);
            }
        }

        return out;
    }

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

    void solve5_part1(std::ifstream &input, bool verbose)
    {
        Data d = parse(input, verbose);

        int64_t minLocation = std::numeric_limits<int64_t>::max();
        for (auto s : d.mSeeds)
        {
            // seed to soil
            auto value = s + getSourceToDestOffset(d.mSeedToSoil, s);

            // soil to fert
            value += getSourceToDestOffset(d.mSoilToFertilizer, value);

            // fert to water
            value += getSourceToDestOffset(d.mFertilizerToWater, value);

            // water to light
            value += getSourceToDestOffset(d.mWaterToLight, value);

            // light to temp
            value += getSourceToDestOffset(d.mLightToTemperature, value);

            // temp to humidity
            value += getSourceToDestOffset(d.mTemperatureToHumidity, value);

            // humidity to location
            value += getSourceToDestOffset(d.mHumidityToLocation, value);

            if (value < minLocation)
                minLocation = value;
        }

        std::cout << minLocation << std::endl;
    }

    void solve5_part2(std::ifstream &input, bool verbose)
    {
        Data d = parse(input, verbose);

        // TODO: this is inefficient. should determine which ranges of seeds have different mappings.
        // for any ranges with the same mapping, the first one in the mapping will always have the lowest location.
        // so we can skip a huge number of seeds.

        // seeds sequence now represents ranges with a source and a length.
        int64_t minLocation = std::numeric_limits<int64_t>::max();
        for (int i = 0; i < d.mSeeds.size(); i += 2)
        {
            auto start = d.mSeeds[i];
            auto len = d.mSeeds[i + 1];

            printf("processing %lld seeds from %lld to %lld\n", len, start, start + len - 1);
            for (auto s = start; s < start + len; ++s)
            {
                // seed to soil
                auto value = s + getSourceToDestOffset(d.mSeedToSoil, s);

                // soil to fert
                value += getSourceToDestOffset(d.mSoilToFertilizer, value);

                // fert to water
                value += getSourceToDestOffset(d.mFertilizerToWater, value);

                // water to light
                value += getSourceToDestOffset(d.mWaterToLight, value);

                // light to temp
                value += getSourceToDestOffset(d.mLightToTemperature, value);

                // temp to humidity
                value += getSourceToDestOffset(d.mTemperatureToHumidity, value);

                // humidity to location
                value += getSourceToDestOffset(d.mHumidityToLocation, value);

                if (value < 0)
                    continue;

                if (value < minLocation)
                    minLocation = value;
            }
        }
        std::cout << minLocation << std::endl;
    }
}