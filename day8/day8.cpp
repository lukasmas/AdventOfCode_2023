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

std::uint32_t goThroughTheMap(const std::string &instructions, const std::map<std::string, Direction> &desertMap)
{
    std::uint32_t stepCounter{0};
    auto key = std::string(start);
    while (key != destination)
        for (char ins : instructions)
        {
            // std::cout << ins << " : " << key << " -> ";
            stepCounter++;
            if (ins == right)
            {
                key = desertMap.at(key).right;
            }
            else
            {
                key = desertMap.at(key).left;
            }
            // std::cout << key << " | " << stepCounter << std::endl;

            if (key == destination)
            {
                break;
            }
        }
    return stepCounter;
}

int main()
{
    std::string line{};
    std::ifstream data("../data.txt");

    std::string instructions{};
    std::map<std::string, Direction> desertMap{};
    std::string firstElem{};

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
    }

    std::cout << instructions << std::endl;
    printDesertMap(desertMap);

    auto result = goThroughTheMap(instructions, desertMap);

    std::cout << std::endl;
    std::cout
        << "Solution : "
        << result << std::endl;

    data.close();

    return 0;
}