#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <numeric>
#include <limits>
#include <set>

#include "../utils/utils.hpp"

constexpr char left = 'L';
constexpr char right = 'R';
constexpr auto destination = "ZZZ";
constexpr auto start = "AAA";

struct Direction
{
    std::string left{};
    std::string right{};
};

std::pair<std::string, Direction> extractData(std::string &line)
{
    std::pair<std::string, Direction> elem{"", {}};
    utils::removeWhiteSpaces(line);
    const auto eqPos = line.find("=");
    const auto openBrackerPos = line.find("(");
    const auto closeBrackerPos = line.find(")");

    elem.first = line.substr(0, eqPos);
    const auto values = utils::splitString(line.substr(openBrackerPos + 1, closeBrackerPos - openBrackerPos - 1), ",");
    elem.second = {values[0], values[1]};
    return elem;
}

void printDesertMap(const std::map<std::string, Direction> &desertMap)
{
    for (const auto &[key, value] : desertMap)
    {
        std::cout << key << " -> " << value.left << "," << value.right << "\n";
    }
}

std::uint64_t calculateLCM(const std::map<std::string, std::uint64_t> &destinations)
{
    auto result = destinations.begin()->second;
    for (const auto &[point, steps] : destinations)
    {
        result = std::lcm(result, steps);
    }
    return result;
}

std::uint64_t goThroughTheMap(const std::string &instructions, const std::map<std::string, Direction> &desertMap, const std::vector<std::string> &startingPoints)
{
    std::uint32_t stepCounter{0};
    std::vector<std::string> elems = startingPoints;
    std::map<std::string, std::uint64_t> destinationReached{};
    bool allDestinationSet = false;
    while (!allDestinationSet)
        for (char ins : instructions)
        {
            stepCounter++;
            std::uint32_t reachedDests{0};
            for (auto &elem : elems)
            {
                switch (ins)
                {
                case right:
                    elem = desertMap.at(elem).right;
                    break;
                case left:
                    elem = desertMap.at(elem).left;
                    break;
                }
                if (elem.back() == 'Z')
                {
                    if (destinationReached.find(elem) == destinationReached.end())
                    {
                        destinationReached[elem] = stepCounter;
                    }
                }
            }

            if (destinationReached.size() == elems.size())
            {
                allDestinationSet = true;
                break;
            }
        }
    return calculateLCM(destinationReached);
}

int main()
{
    std::string line{};
    std::ifstream data("../data.txt");

    std::string instructions{};
    std::map<std::string, Direction> desertMap{};
    std::vector<std::string> startingPoints{};

    std::cout << "DAY 8" << std::endl;

    if (!data.is_open())
    {
        std::cout << "Failed to open file!";
        return -1;
    }

    std::getline(data, line);
    instructions = line;
    while (std::getline(data, line))
    {
        if (line.empty())
        {
            continue;
        }
        auto data = extractData(line);
        desertMap[data.first] = data.second;
        if (data.first.back() == 'A')
        {
            startingPoints.push_back(data.first);
        }
    }

    for (const auto &s : startingPoints)
    {
        std::cout << s << std::endl;
    }

    // std::cout << instructions << std::endl;
    // printDesertMap(desertMap);

    auto result = goThroughTheMap(instructions, desertMap, startingPoints);

    std::cout << std::endl;
    std::cout
        << "Solution : "
        << result << std::endl;

    data.close();

    return 0;
}