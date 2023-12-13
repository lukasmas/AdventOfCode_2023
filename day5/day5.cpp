#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <numeric>
#include <limits>

using SeedInfo = std::vector<std::uint64_t>;

namespace keywords
{
    constexpr auto seeds{"seeds:"};
    constexpr auto map{"map:"};
    constexpr auto locationId{7U};

    constexpr auto destination{0U};
    constexpr auto source{1U};
    constexpr auto range{2U};
    constexpr auto mapSize{3U};
}

auto splitString(const std::string &str, const std::string &delimiter)
{
    std::vector<std::string> splitStr{};
    auto beginPos{0};
    auto endPos{0};
    bool endReached{false};

    while (!endReached)
    {
        endPos = str.find(delimiter, beginPos);
        if (endPos == std::string::npos)
        {
            splitStr.push_back(str.substr(beginPos, endPos));
            endReached = true;
        }
        else
        {
            splitStr.push_back(str.substr(beginPos, endPos - beginPos));
            beginPos = endPos + 1;
        }
    }
    return splitStr;
}

bool is_number(const std::string &s)
{
    return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
}

auto extractNumbers(const std::string &numbers)
{
    std::vector<std::uint64_t> nums{};
    const auto numsStr = splitString(numbers, " ");
    for (const auto n : numsStr)
    {
        if (is_number(n))
        {
            nums.push_back(atoll(n.c_str()));
        }
    }
    return nums;
}

std::vector<std::pair<std::uint64_t, std::uint64_t>> mapValues(const std::vector<std::uint64_t> &mapInfo, std::uint64_t start, std::uint64_t range, std::vector<std::pair<std::uint64_t, std::uint64_t>> &nextRanges)
{
    std::vector<std::pair<std::uint64_t, std::uint64_t>> newRanges{};
    const auto sourceFromMap = mapInfo.at(keywords::source);
    const auto rangeFromMap = mapInfo.at(keywords::range);
    const auto destFromMap = mapInfo.at(keywords::destination);
    const auto end = start + range - 1;
    const auto endMap = sourceFromMap + rangeFromMap - 1;
    if (end < sourceFromMap || start > endMap)
    {
        newRanges.push_back({start, range});
        return newRanges;
    }

    if (start >= sourceFromMap)
    {
        const auto startOffset = start - sourceFromMap;

        if (range <= rangeFromMap - startOffset)
        {
            nextRanges.push_back({destFromMap + startOffset, range});
        }
        else
        {
            const auto elementsToMap = rangeFromMap - startOffset;
            const auto rangeOffset = range - elementsToMap;
            nextRanges.push_back({destFromMap + startOffset, elementsToMap});
            newRanges.push_back({start + elementsToMap, rangeOffset});
        }
    }
    else
    {
        const auto startOffset = sourceFromMap - start;
        const auto rangeOffset = range - startOffset;

        newRanges.push_back({start, startOffset});
        if (rangeOffset <= rangeFromMap)
        {
            nextRanges.push_back({destFromMap, rangeOffset});
        }
        else
        {
            nextRanges.push_back({destFromMap, rangeFromMap});
            newRanges.push_back({start + rangeFromMap + startOffset, rangeOffset - rangeFromMap});
        }
    }

    return newRanges;
}

std::uint64_t getLowestLocation(const std::vector<std::pair<std::uint64_t, std::uint64_t>> &startsAndRanges, const std::vector<std::vector<SeedInfo>> &maps)
{
    std::uint64_t minLocation{std::numeric_limits<std::uint64_t>::max()};
    std::vector<std::pair<std::uint64_t, std::uint64_t>> nextRanges{}, currentRanges{}, newRanges{};

    for (const auto &ranges : startsAndRanges)
    {
        const auto expectedRange = ranges.second;
        nextRanges.clear();
        nextRanges.push_back(ranges);
        int map = 1;
        for (const auto &sourceToDestMap : maps)
        {
            auto rangesToprocess = nextRanges;
            nextRanges.clear();

            for (auto &rtp : rangesToprocess)
            {

                currentRanges.clear();
                currentRanges.push_back(rtp);

                for (auto &mapInfo : sourceToDestMap)
                {
                    newRanges.clear();

                    if (mapInfo.size() != 3)
                    {
                        return 0;
                    }
                    for (const auto [start, range] : currentRanges)
                    {
                        auto values = mapValues(mapInfo, start, range, nextRanges);
                        for (const auto &v : values)
                        {
                            newRanges.push_back(v);
                        }
                    }

                    currentRanges = newRanges;
                }
                for (const auto &cr : currentRanges)
                {
                    nextRanges.push_back(cr);
                }
            }
        }

        for (const auto [s, r] : nextRanges)
        {
            minLocation = std::min(minLocation, s);
        }
    }

    return minLocation;
}

int main()
{
    std::string line{};
    std::ifstream data("../data.txt");
    std::vector<std::vector<SeedInfo>> maps{};

    std::vector<std::pair<std::uint64_t, std::uint64_t>> startsAndRanges{};
    std::cout << "DAY 5" << std::endl;

    if (!data.is_open())
    {
        std::cout << "Failed to open file!";
        return -1;
    }

    while (std::getline(data, line))
    {
        if (const auto pos = line.find(keywords::seeds); pos != std::string::npos)
        {
            if (startsAndRanges.empty())
            {
                auto counter{1U};
                std::pair<std::uint32_t, std::uint32_t> startAndRange{};
                for (const auto n : extractNumbers(line.substr(pos + 1)))
                {
                    if (counter++ % 2 != 0)
                    {
                        startAndRange.first = n;
                    }
                    else
                    {
                        startAndRange.second = n;
                        startsAndRanges.push_back(startAndRange);
                    }
                }
            }

            continue;
        }
        else if (const auto pos = line.find(keywords::map); pos != std::string::npos)
        {
            std::vector<SeedInfo> mapInfo{};
            while (std::getline(data, line) && !line.empty())
            {
                mapInfo.push_back(extractNumbers(line));
            }
            maps.push_back(mapInfo);
        }
    }

    std::cout << "Solution 2 result: "
              << getLowestLocation(startsAndRanges, maps) << std::endl;

    data.close();
    return 0;
}