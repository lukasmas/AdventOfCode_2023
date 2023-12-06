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
            nums.push_back(atoi(n.c_str()));
        }
    }
    return nums;
}

void getValueFromMap(const std::vector<std::uint64_t> &mappingInstuctions, SeedInfo &seedInfos, std::vector<bool> &updated)
{
    if (mappingInstuctions.size() != keywords::mapSize)
    {
        return;
    }

    auto index{0U};
    for (auto &seedInfo : seedInfos)
    {
        const auto source = mappingInstuctions.at(keywords::source);
        const auto range = mappingInstuctions.at(keywords::range);
        // std::cout << seedInfo << " | " << source << " | " << range << std::endl;
        // std::cout << seedInfo << " -> ";
        if (updated[index])
        {
            index++;
            continue;
        }
        if (seedInfo >= source && seedInfo < (source + range))
        {
            const auto destinationOffset = seedInfo - source;
            // std::cout << "Dest offset: " << destinationOffset << std::endl;
            seedInfo = (mappingInstuctions.at(keywords::destination) + destinationOffset);
            updated[index++] = true;
            // std::cout << "Result: " << mappingInstuctions.at(keywords::destination) + destinationOffset << std::endl;
        }

        // std::cout << seedInfo << std::endl;
    }
}

void fillMiisingData(std::vector<SeedInfo> &seedInfos, std::uint32_t expectedSize = 2)
{
    for (auto &seedInfo : seedInfos)
    {
        if (seedInfo.size() != expectedSize)
        {
            seedInfo.push_back(seedInfo.back());
        }
        auto back = seedInfo.back();
        seedInfo.clear();
        seedInfo.push_back(back);
    }
}

void printSeedInfos(const SeedInfo &seedsInfo)
{
    for (const auto &seedInfo : seedsInfo)
    {

        std::cout << seedInfo << " ";
    }
    std::cout << std::endl;
}

std::uint64_t getLowestLocation(const SeedInfo &seedsInfo)
{
    std::uint64_t min = std::numeric_limits<std::uint64_t>::max();
    for (const auto &seedInfo : seedsInfo)
    {
        min = std::min(min, seedInfo);
    }
    return min;
}

int main()
{
    std::string line{};
    std::ifstream data("../data.txt");
    SeedInfo seedInfos{};
    std::uint32_t mapProcessed{};
    std::uint32_t batch{0};

    std::uint64_t minLocation{std::numeric_limits<std::uint64_t>::max()};

    std::vector<std::pair<std::uint64_t, std::uint64_t>> startsAndRanges{};
    bool allBatchesProccesed{false};

    std::cout << "DAY 5" << std::endl;

    if (!data.is_open())
    {
        std::cout << "Failed to open file!";
        return -1;
    }

    while (!allBatchesProccesed)
    {
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
                        // seedInfos.push_back(SeedInfo{n});
                        if (counter++ % 2 != 0)
                        {
                            startAndRange.first = n;
                        }
                        else
                        {
                            startAndRange.second = n;
                            startsAndRanges.push_back(startAndRange);
                            // std::cout << startAndRange.first << " " << startAndRange.second << std::endl;
                        }
                    }
                    std::cout << "Ranges created" << std::endl;
                }
                seedInfos.clear();
                for (std::uint64_t i{startsAndRanges[batch].first}; i < (startsAndRanges[batch].first + startsAndRanges[batch].second); i++)
                {
                    seedInfos.push_back(i);
                }
                std::cout << "Seed info done" << std::endl;
                // printSeedInfos(seedInfos);

                batch++;
                if (batch == startsAndRanges.size())
                {
                    allBatchesProccesed = true;
                }
                mapProcessed = 1;

                continue;
            }
            else if (const auto pos = line.find(keywords::map); pos != std::string::npos)
            {
                ++mapProcessed;
                // printSeedInfos(seedInfos);
                std::vector<bool> updated(seedInfos.size(), false);
                while (std::getline(data, line) && !line.empty())
                {
                    getValueFromMap(extractNumbers(line), seedInfos, updated);
                }
                // printSeedInfos(seedInfos);

                // printSeedInfos(seedInfos);

                // fillMiisingData(seedInfos);
                std::cout << "Map " << mapProcessed - 1 << " done" << std::endl;
                // break;
            }
        }

        minLocation = std::min(minLocation, getLowestLocation(seedInfos));
        // printSeedInfos(seedInfos);
        std::cout << std::endl;
        std::cout << "Batch min location: " << minLocation << std::endl;
        data.clear();
        data.seekg(0);
    }
    // std::cout << "Solution 1 result: " << getLowestLocation(seedInfos) << std::endl;
    std::cout << "Solution 2 result: " << minLocation << std::endl;

    data.close();
    return 0;
}