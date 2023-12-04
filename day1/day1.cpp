#include <iostream>
#include <fstream>
#include <string>
#include <map>

std::map<std::string, std::uint32_t> strToNum{
    {"one", 1}, {"two", 2}, {"three", 3}, {"four", 4}, {"five", 5}, {"six", 6}, {"seven", 7}, {"eight", 8}, {"nine", 9}};

std::uint32_t getValueFromLine(const std::string &line)
{

    std::uint32_t pos{0};
    std::uint32_t leftNumber{0};
    std::uint32_t leftPos{0};
    std::uint32_t rightNumber{0};
    std::uint32_t rightPos{0};

    for (const char c : line)
    {
        if (isdigit(c))
        {
            if (leftNumber == 0)
            {
                leftNumber = atoi(&c);
                leftPos = pos;
            }
            rightNumber = atoi(&c);
            rightPos = pos;
        }
        pos++;
    }

    // Extension for 2 stars
    for (const auto &[str, num] : strToNum)
    {
        auto res = line.find(str);
        if (res != std::string::npos)
        {
            if (res <= leftPos)
            {
                leftNumber = num;
                leftPos = res;
            }
        }
        res = line.rfind(str);
        if (res != std::string::npos)
        {
            if (res > rightPos)
            {
                rightNumber = num;
                rightPos = res;
            }
        }
    }

    auto digit = leftNumber * 10 + rightNumber;

    std::cout << line << " : " << leftNumber << " : " << rightNumber << " : " << digit << std::endl;

    return digit;
}

int main()
{
    std::string line{};
    std::uint32_t lineProcessed{0};
    std::uint32_t sum{0};

    std::cout << "DAY 1" << std::endl;

    std::ifstream data("../data.txt");
    if (!data.is_open())
    {
        std::cout << "Failed to open file" << std::endl;
        return -1;
    }

    while (getline(data, line))
    {
        sum += getValueFromLine(line);
        ++lineProcessed;
    }

    std::cout << sum << std::endl;

    data.close();
    return 0;
}
