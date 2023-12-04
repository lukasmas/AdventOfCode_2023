#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>

constexpr auto red{"red"};
constexpr auto green{"green"};
constexpr auto blue{"blue"};
constexpr auto coma{","};
constexpr auto colon{":"};
constexpr auto semicolon{";"};

constexpr auto space{" "};

const std::map<std::string, std::uint32_t> maxCubesCountPerGame{
    {red, 12},
    {green, 13},
    {blue, 14}};

std::vector<std::string> splitString(const std::string &str, const std::string &delimiter)
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
std::uint32_t getGameID(const std::string &line)
{
    auto spacePos = line.find_first_of(space);
    auto semicolonPos = line.find(semicolon);
    if (spacePos != std::string::npos && semicolonPos != std::string::npos and spacePos < semicolonPos)
    {
        return atoi(line.substr(spacePos, semicolonPos).c_str());
    }
    return 0;
}

bool isPossbile(const std::string &linePart)
{

    const auto splitStr = splitString(linePart, coma);

    for (const auto &s : splitStr)
    {
        auto secondSpacePos = s.rfind(space);
        const auto color = s.substr(secondSpacePos + 1);
        const auto cubeCount = atoi(s.substr(1, secondSpacePos).c_str());
        if (maxCubesCountPerGame.at(color) < cubeCount)
        {
            return false;
        }
    }

    return true;
}

std::uint32_t getGameIdIfGameIsPossible(const std::string &line)
{
    const auto gameID = getGameID(line);
    auto startPos = line.find(colon);
    std::cout << gameID << " : ";

    const auto sets = splitString(line.substr(startPos + 1), semicolon);

    for (const auto &s : sets)
    {
        if (!isPossbile(s))
        {
            std::cout << "not possible\n";
            return 0;
        }
    }
    std::cout << "possible\n";

    return gameID;
}

void getBiggestNumberOfCubesUsed(const std::string sets, std::map<std::string, std::uint32_t> &cubes)
{

    const auto splitStr = splitString(sets, coma);

    for (const auto &s : splitStr)
    {
        auto secondSpacePos = s.rfind(space);
        const auto color = s.substr(secondSpacePos + 1);
        const auto cubeCount = atoi(s.substr(1, secondSpacePos).c_str());
        if (cubes.at(color) < cubeCount)
        {
            cubes[color] = cubeCount;
        }
    }
}

std::uint32_t getMinimalNumberOfCubesInGame(const std::string &line)
{
    std::map<std::string, std::uint32_t> cubes{{red, 0}, {green, 0}, {blue, 0}};
    const auto gameID = getGameID(line);
    auto startPos = line.find(colon);
    std::uint32_t multi = 1;

    const auto sets = splitString(line.substr(startPos + 1), semicolon);

    for (const auto &s : sets)
    {
        getBiggestNumberOfCubesUsed(s, cubes);
    }

    for (const auto &[color, count] : cubes)
    {
        if (count > 0)
        {
            multi *= count;
        }
    }
    std::cout << gameID << " : " << multi << std::endl;

    return multi;
}

int main()
{
    std::uint32_t sum{};
    std::string line{};

    std::cout << "DAY 2" << std::endl;

    std::ifstream data("../data.txt");
    if (!data.is_open())
    {
        std::cout << "Failed to open file" << std::endl;
        return -1;
    }

    while (getline(data, line))
    {
        // sum += getGameIdIfGameIsPossible(line);
        sum += getMinimalNumberOfCubesInGame(line);
    }

    std::cout << "Result: " << sum << std::endl;

    return 0;
}
