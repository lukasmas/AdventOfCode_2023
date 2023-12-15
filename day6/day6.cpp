#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <numeric>
#include <limits>

#include "../utils/utils.hpp"

struct RaceData
{
    std::uint64_t Time;
    std::uint64_t Distance;
};

void removeWhiteSpaces(std::string &line)
{
    auto pos = line.find(" ");
    while (pos != std::string::npos)
    {
        line.erase(pos, 1);
        pos = line.find(" ");
    }
}

std::vector<RaceData> createRaceData(const std::vector<std::vector<std::uint64_t>> &numbers)
{
    std::vector<RaceData> raceDatas{};
    const int timeIndex = 0;
    const int distanceIndex = 1;

    for (int i = 0; i < numbers[0].size(); i++)
    {
        RaceData raceData{};
        raceData.Time = numbers[timeIndex][i];
        raceData.Distance = numbers[distanceIndex][i];
        raceDatas.push_back(raceData);
    }
    return raceDatas;
}

void printNums(const std::vector<std::vector<std::uint64_t>> &numbers)
{
    for (const auto &c : numbers)
    {
        for (const auto v : c)
        {
            std::cout << v << " ";
        }
        std::cout << std::endl;
    }
}

std::uint64_t CalculateDistanse(std::uint64_t holdTime, std::uint64_t raceDuration)
{
    return holdTime * (raceDuration - holdTime);
}

std::uint64_t GetNumberOfPossibleBetterResults(RaceData raceData)
{
    std::uint64_t possibleBetterResultsCount = 0u;

    for (std::uint64_t i = 1u; i < raceData.Time; i++)
    {
        if (CalculateDistanse(i, raceData.Time) > raceData.Distance)
        {
            possibleBetterResultsCount++;
        }
    }

    return possibleBetterResultsCount;
}

std::uint64_t CalculateResult(const std::vector<RaceData> &raceDatas)
{
    std::uint64_t result = 1;
    for (const auto &raceData : raceDatas)
    {
        result *= GetNumberOfPossibleBetterResults(raceData);
    }

    return result;
}

int main()
{
    std::string line{};
    std::ifstream data("../data.txt");
    std::vector<std::vector<std::uint64_t>> numbers{};

    std::cout << "DAY 6" << std::endl;

    if (!data.is_open())
    {
        std::cout << "Failed to open file!";
        return -1;
    }

    while (std::getline(data, line))
    {
        removeWhiteSpaces(line);
        numbers.push_back(utils::extractNumbers(line.substr(line.find(":") + 1)));
    }
    const auto raceDatas = createRaceData(numbers);

    std::cout << "Solution : "
              << CalculateResult(raceDatas) << std::endl;

    data.close();

    return 0;
}