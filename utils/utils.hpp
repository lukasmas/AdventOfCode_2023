#include <string>
#include <vector>
#include <algorithm>

namespace utils
{
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
}
