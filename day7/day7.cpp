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

const std::map<char, int> cards{{'A', 13}, {'K', 12}, {'Q', 11}, {'J', 10}, {'T', 9}, {'9', 8}, {'8', 7}, {'7', 6}, {'6', 5}, {'5', 4}, {'4', 3}, {'3', 2}, {'2', 1}};

enum class Type
{
    HighCard,
    OnePair,
    TwoPair,
    ThreeOf,
    Full,
    FourOf,
    FiveOf
};

struct Hand
{
    std::string cards;
    uint64_t bid;
    Type type;

    bool operator<(const Hand &hand) const
    {
        return this->cards < hand.cards;
    }
};

// 0 -> equal
// 1 -> card1 is stronger
// 2 -> card2 is stronger
int compareCards(char card1, char card2)
{
    if (cards.at(card1) == cards.at(card2))
    {
        return 0;
    }
    return cards.at(card1) > cards.at(card2) ? 1 : 2;
}

std::map<char, std::uint32_t> calculateCards(const std::string &cards)
{
    std::map<char, std::uint32_t> cardsMap{};

    for (char c : cards)
    {
        if (cardsMap.find(c) != cardsMap.end())
        {
            cardsMap[c]++;
        }
        else
        {
            cardsMap[c] = 1;
        }
    }
    return cardsMap;
}

Type getHandType(const std::string &cards)
{
    std::uint32_t pairsCount{0};
    bool threeExist{false};

    for (auto [card, count] : calculateCards(cards))
    {
        switch (count)
        {
        case 5:
            return Type::FiveOf;
            break;
        case 4:
            return Type::FourOf;
            break;
        case 3:
            threeExist = true;
            break;
        case 2:
            pairsCount++;
            break;
        case 1:
            break;
        default:
            break;
        }
    }
    if (threeExist)
    {
        if (pairsCount == 1)
        {
            return Type::Full;
        }
        return Type::ThreeOf;
    }
    if (pairsCount == 2)
    {
        return Type::TwoPair;
    }
    if (pairsCount == 1)
    {
        return Type::OnePair;
    }

    return Type::HighCard;
}

void printGames(const std::vector<Hand> &games)
{
    const auto to_str = [](Type type)
    {
        switch (type)
        {
        case Type::FiveOf:
            return "Type::FiveOf";
        case Type::FourOf:
            return "Type::FourOf";
        case Type::ThreeOf:
            return "Type::ThreeOf";
        case Type::Full:
            return "Type::Full";
        case Type::TwoPair:
            return "Type::TwoPair";
        case Type::OnePair:
            return "Type::OnePair";
        case Type::HighCard:
            return "Type::HighCard";
        default:
            break;
        }
        return "";
    };

    for (const auto &hand : games)
    {
        std::cout << hand.cards << " : " << hand.bid << " : " << to_str(hand.type) << std::endl;
    }
}

std::uint64_t calculateResult(const std::vector<Hand> &games)
{
    std::uint64_t result{0};
    for (std::uint64_t i{0}; i < games.size(); i++)
    {
        // std::cout << i + 1 << " * " << games[i].bid << std::endl;
        result += (i + 1) * games[i].bid;
    }
    return result;
}

int main()
{
    std::string line{};
    std::ifstream data("../data.txt");

    std::vector<Hand> games{};

    std::cout << "DAY 6" << std::endl;

    if (!data.is_open())
    {
        std::cout << "Failed to open file!";
        return -1;
    }

    while (std::getline(data, line))
    {
        Hand hand{};
        auto gameData = utils::splitString(line, " ");
        hand.cards = gameData[0];
        hand.bid = utils::extractNumbers(gameData[1])[0];
        hand.type = getHandType(hand.cards);
        games.push_back(hand);
    }

    std::sort(games.begin(), games.end(), [](const auto &game1, const auto &game2)
              {
                  if (game1.type != game2.type)
                  {
                      return game1.type > game2.type ? false : true;
                  }
                  else{
                    auto& g1Card = game1.cards;
                    auto& g2Card = game2.cards;

                    for(int i{0}; i< g1Card.length(); i++){
                        if(g1Card[i] == g2Card[i]){
                            continue;
                        }
                        else{
                            return cards.at(g1Card[i]) >  cards.at(g2Card[i]) ? false : true;
                        }
                    }
                  }
                  return false; });

    printGames(games);
    std::cout << std::endl;
    std::cout
        << "Solution : " << calculateResult(games) << std::endl;

    data.close();

    return 0;
}