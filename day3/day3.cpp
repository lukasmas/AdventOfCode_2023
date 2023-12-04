// day3.cpp : Defines the entry point for the application.
//

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <set>

class SymbolCoridates
{

public:
    std::uint32_t x{};
    std::uint32_t y{};
    char sym{};

    void print() const
    {
        std::cout << x << ", " << y << ", " << sym << "\n";
    }
};

void printEngineSchema(const std::vector<std::vector<char>> &engineSchema)
{
    for (const auto &engine : engineSchema)
    {
        for (const auto &e : engine)
        {
            std::cout << e << "|";
        }
        std::cout << std::endl;
    }
}

std::vector<SymbolCoridates> findSymbols(const std::vector<std::vector<char>> &engineSchema)
{
    std::vector<SymbolCoridates> symbolCoridates{};

    std::uint32_t x{}, y{};
    for (const auto &row : engineSchema)
    {
        x = 0;
        for (const auto &c : row)
        {
            if (c != '.' and !isdigit(c))
            {
                symbolCoridates.emplace_back(SymbolCoridates{x, y, c});
            }
            x++;
        }
        y++;
    }

    return symbolCoridates;
}

std::uint32_t combineNumber(std::uint32_t x, const std::vector<char> &row)
{

    std::set<std::uint32_t> indexes{x};
    bool rightEndFound = false;
    bool leftEndFound = false;
    std::string number{};

    for (int i{1}; i < row.size() / 2; i++)
    {
        int newX = x - i;
        if (!leftEndFound && newX >= 0)
        {
            if (isdigit(row.at(newX)))
            {
                indexes.insert(newX);
            }
            else
            {
                leftEndFound = true;
            }
        }
        newX = x + i;
        if (!rightEndFound && newX < row.size())
        {
            if (isdigit(row.at(newX)))
            {
                indexes.insert(newX);
            }
            else
            {
                rightEndFound = true;
            }
        }

        if (rightEndFound && leftEndFound)
        {
            break;
        }
    }
    for (const auto i : indexes)
    {
        number += row.at(i);
    }

    return atoi(number.c_str());
}

bool isPointInRange(int x, int y, int xMax, int yMax)
{
    if (y <= yMax && x <= xMax && x >= 0 && y >= 0)
    {
        return true;
    }

    return false;
}

std::uint32_t findAdjacentNumbersSum(const std::vector<std::vector<char>> &engineSchema, const std::vector<SymbolCoridates> &symbolsCordinates)
{
    std::uint32_t sum{};
    std::vector<int> rangeToSearch{-1, 0, 1};
    std::uint32_t prevNumber{};
    int prevY{-1};
    const auto yMax = engineSchema.size();
    const auto xMax = engineSchema.at(0).size();
    for (const auto &symbol : symbolsCordinates)
    {
        for (const auto y : rangeToSearch)
        {
            for (const auto x : rangeToSearch)
            {
                const int newX = symbol.x + x;
                const int newY = symbol.y + y;

                if (isPointInRange(newX, newY, xMax, yMax))
                {
                    if (isdigit(engineSchema[newY][newX]))
                    {

                        auto number = combineNumber(newX, engineSchema.at(newY));
                        if (prevNumber == number && newY == prevY)
                        {
                            continue;
                        }
                        sum += number;
                        prevNumber = number;
                        std::cout << number << " ";
                    }
                }
                prevY = newY;
            }
        }
    }

    return sum;
}

std::uint32_t findGearRatioSum(const std::vector<std::vector<char>> &engineSchema, const std::vector<SymbolCoridates> &symbolsCordinates)
{
    std::uint32_t sum{};
    std::vector<int> rangeToSearch{-1, 0, 1};
    std::uint32_t prevNumber{};
    int prevY{-1};
    const auto yMax = engineSchema.size();
    const auto xMax = engineSchema.at(0).size();
    for (const auto &symbol : symbolsCordinates)
    {
        if (symbol.sym != '*')
        {
            continue;
        }
        std::vector<std::uint32_t> nums{};
        for (const auto y : rangeToSearch)
        {
            for (const auto x : rangeToSearch)
            {
                const int newX = symbol.x + x;
                const int newY = symbol.y + y;

                if (isPointInRange(newX, newY, xMax, yMax))
                {
                    if (isdigit(engineSchema[newY][newX]))
                    {

                        auto number = combineNumber(newX, engineSchema.at(newY));
                        if (prevNumber == number && newY == prevY)
                        {
                            continue;
                        }
                        nums.push_back(number);
                        prevNumber = number;
                        // std::cout << number << " ";
                    }
                }
                prevY = newY;
            }
        }
        if (nums.size() == 2)
        {
            sum += (nums[0] * nums[1]);
        }
    }

    return sum;
}

int main()
{
    std::uint32_t sum{};

    std::vector<std::vector<char>> engineSchema{};
    engineSchema.reserve(140 * 141 * sizeof(char));

    std::cout << "DAY 3" << std::endl;

    std::string line{};
    std::ifstream data("../data.txt");
    if (!data.is_open())
    {
        std::cout << "Failed to open file" << std::endl;
        return -1;
    }

    while (getline(data, line))
    {
        std::vector<char> row{};
        for (const char c : line)
        {
            row.push_back(c);
        }
        engineSchema.push_back(row);
    }

    const auto symbols = findSymbols(engineSchema);

    std::cout << std::endl;

    // sum = findAdjacentNumbersSum(engineSchema, symbols);

    sum = findGearRatioSum(engineSchema, symbols);

    std::cout << "Sum: " << sum << std::endl;

    return 0;
}
