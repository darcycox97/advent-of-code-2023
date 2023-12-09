#include "../util/print.h"
#include <fstream>
#include <sstream>
#include <map>
#include <algorithm>

namespace aoc7
{
    const size_t HAND_LEN = 5;

    // rankings of each hand type. lower = worth less.
    const int HIGH_CARD = 1;
    const int ONE_PAIR = 2;
    const int TWO_PAIR = 3;
    const int THREE_OF_A_KIND = 4;
    const int FULL_HOUSE = 5;
    const int FOUR_OF_A_KIND = 6;
    const int FIVE_OF_A_KIND = 7;

    int classify_hand(const std::string &hand, bool usingJokers)
    {
        assert(hand.size() == 5);

        std::map<char, int> cards;
        for (char c : hand)
            cards[c]++;

        if (usingJokers)
        {
            auto jokerCount = cards['J'];
            cards.erase('J');

            // distribute the joker cards to increase the highest count.
            auto it = std::max_element(cards.begin(), cards.end(),
                                       [](auto l, auto r)
                                       { return l.second < r.second; });

            if (it == cards.end())
            {
                assert(jokerCount == 5);
                return FIVE_OF_A_KIND;
            }

            util::verbose_print("Hand %s: adding %d to existing count %d of card %c\n", hand.c_str(), jokerCount, it->second, it->first);

            it->second += jokerCount;
        }

        if (cards.size() == 1)
            return FIVE_OF_A_KIND;
        else if (cards.size() == 2)
        {
            auto it = cards.begin();
            int c1 = it->second;
            ++it;
            int c2 = it->second;
            if (c1 == 4 || c2 == 4)
                return FOUR_OF_A_KIND;
            else
                return FULL_HOUSE;
        }
        else if (cards.size() == 3)
        {
            int maxDupes = std::max_element(cards.begin(), cards.end(), [](auto l, auto r)
                                            { return l.second < r.second; })
                               ->second;
            if (maxDupes == 3)
                return THREE_OF_A_KIND;
            else
            {
                assert(maxDupes == 2);
                return TWO_PAIR;
            }
        }
        else if (cards.size() == 4)
        {
            return ONE_PAIR;
        }
        else if (cards.size() == 5)
        {
            return HIGH_CARD;
        }

        assert(false && "failed to classify hand");
        return -1;
    }

    bool cmp_card(char l, char r, bool usingJokers)
    {
        static constexpr std::array<char, 13> ORDERED_CARDS{
            '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A'};

        static constexpr std::array<char, 13> ORDERED_CARDS_JOKER{
            'J', '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'Q', 'K', 'A'};

        auto &order = usingJokers ? ORDERED_CARDS_JOKER : ORDERED_CARDS;

        auto l_it = std::find(order.begin(), order.end(), l);
        assert(l_it != order.end());

        auto r_it = std::find(order.begin(), order.end(), r);
        assert(r_it != order.end());

        auto l_ind = std::distance(order.begin(), l_it);
        auto r_ind = std::distance(order.begin(), r_it);
        return l_ind < r_ind;
    }

    // lower ranks first
    bool cmp_hand(const std::string &l, const std::string &r, bool usingJokers)
    {
        int lc = classify_hand(l, usingJokers);
        int rc = classify_hand(r, usingJokers);

        util::verbose_print("Hand %s classification=%d\n", l.c_str(), lc);
        util::verbose_print("Hand %s classification=%d\n", r.c_str(), rc);

        if (lc != rc)
        {
            util::verbose_print("A: hand %s < hand %s ? %d\n", l.c_str(), r.c_str(), lc < rc);
            return lc < rc;
        }

        for (size_t i = 0; i < HAND_LEN; ++i)
        {
            if (l[i] == r[i])
                continue;

            util::verbose_print("B: hand %s < hand %s ? %d\n", l.c_str(), r.c_str(), cmp_card(l[i], r[i], usingJokers));

            return cmp_card(l[i], r[i], usingJokers);
        }

        util::verbose_print("C: hand %s < hand %s ? %d\n", l.c_str(), r.c_str(), true);

        return false; // equal. not less than
    }

    // hand and bid
    std::vector<std::pair<std::string, int>> parse(std::ifstream &input)
    {
        std::vector<std::pair<std::string, int>> out;

        std::string line;
        while (std::getline(input, line))
        {
            std::istringstream lineStream(line);
            std::string hand;
            int bid;
            lineStream >> hand;
            lineStream >> bid;

            util::verbose_print("hand: %s, bid: %d\n", hand.c_str(), bid);

            assert(hand.size() == HAND_LEN);
            out.push_back({hand, bid});
        }

        return out;
    }

    void solve7_part1(std::ifstream &input)
    {
        auto hands = parse(input);

        for (const auto &hand : hands)
            assert(hand.first.size() == 5);

        std::sort(hands.begin(), hands.end(), [](const auto &h1, const auto &h2)
                  { return cmp_hand(h1.first, h2.first, false); });

        int64_t sum = 0;
        for (size_t i = 0; i < hands.size(); ++i)
        {
            int rank = i + 1;

            util::verbose_print("Hand %s rank=%d\n", hands[i].first.c_str(), rank);

            sum += rank * hands[i].second;
        }

        std::cout << sum << std::endl;
    }

    void solve7_part2(std::ifstream &input)
    {
        // now "J" is a joker - can be any card.
        // but it's the weakest for the tiebreaker rule.

        auto hands = parse(input);

        std::sort(hands.begin(), hands.end(), [](const auto &h1, const auto &h2)
                  { return cmp_hand(h1.first, h2.first, true); });

        int64_t sum = 0;
        for (size_t i = 0; i < hands.size(); ++i)
        {
            int rank = i + 1;

            util::verbose_print("Hand %s rank=%d\n", hands[i].first.c_str(), rank);

            sum += rank * hands[i].second;
        }

        std::cout << sum << std::endl;
    }

}