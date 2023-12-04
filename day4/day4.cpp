#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <set>
#include <algorithm>
#include <numeric>

struct Card
{
	std::uint32_t id{};
	std::uint32_t matches{};
	std::uint32_t copies{1};
};

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

std::set<int> extractNumbers(const std::string &numbers)
{
	std::set<int> nums{};
	const auto numsStr = splitString(numbers, " ");
	for (const auto n : numsStr)
	{
		if (is_number(n))
		{
			nums.insert(atoi(n.c_str()));
		}
	}
	return nums;
}

std::uint32_t getNumberOfMatches(std::set<int> given, std::set<int> winning)
{
	std::uint32_t counter{};
	for (const auto w : winning)
	{
		if (given.find(w) != given.end())
		{
			counter++;
		}
	}
	// std::cout << counter << std::endl;
	return counter;
}

std::uint32_t calculatePoints(std::uint32_t numberOfMatches)
{
	std::uint32_t points{};
	for (std::uint32_t i{0}; i < numberOfMatches; i++)
	{
		if (i == 0)
		{
			points = 1;
		}
		else
		{
			points *= 2;
		}
	}
	return points;
}

std::uint32_t calculateTotalScratches(std::vector<Card> &cards)
{
	for (std::uint32_t i{0}; i < cards.size(); i++)
	{
		for (int copies{0}; copies < cards[i].copies; copies++)
		{
			for (std::uint32_t m{1}; m < cards[i].matches + 1; m++)
			{
				cards[i + m].copies++;
			}
		}
	}
	return std::accumulate(cards.begin(), cards.end(), 0U, [](std::uint32_t sum, const Card &c)
						   { return sum + c.copies; });
}

int main()
{
	// std::uint32_t sum{};
	std::vector<Card> cards{};
	std::string line{};
	std::ifstream data("../data.txt");

	std::cout << "DAY 4" << std::endl;

	if (!data.is_open())
	{
		std::cout << "Failed to open file!";
		return -1;
	}

	while (std::getline(data, line))
	{
		Card card{};
		auto colonPos = line.find(":");
		auto spacePos = line.find(" ");
		if (colonPos == std::string::npos || spacePos == std::string::npos)
		{
			continue;
		}
		card.id = atoi(line.substr(spacePos, colonPos - spacePos).c_str());
		const auto sets = splitString(line.substr(colonPos + 1), "|");
		const auto winningNums = extractNumbers(sets.at(0));
		const auto yourNums = extractNumbers(sets.at(1));

		card.matches = getNumberOfMatches(yourNums, winningNums);
		cards.push_back(card);
	}

	// Solution 1*:
	std::uint32_t sum{};
	for (const auto &c : cards)
	{
		sum += calculatePoints(c.matches);
	}
	std::cout << "Sum: " << sum << std::endl;
	// Solution 2*:
	std::cout << "Total scratches: " << calculateTotalScratches(cards) << std::endl;

	data.close();
	return 0;
}