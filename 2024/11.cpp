// https://adventofcode.com/2024/day/11

#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <unordered_map>
#include <cassert>
#include <chrono>
#include <string>
#include <unordered_set>
#include <utility>


/*
    "Every time you blink, the stones each simultaneously change according to the
     first applicable rule in this list:

     [1] - If the stone is engraved with the number 0, it is replaced by a stone
           engraved with the number 1.
     [2] - If the stone is engraved with a number that has an even number of digits,
           it is replaced by two stones. The left half of the digits are engraved on
           the new left stone, and the right half of the digits are engraved on the
           new right stone. (The new numbers don't keep extra leading zeroes: 1000
           would become stones 10 and 0.)
     [3] - If none of the other rules apply, the stone is replaced by a new stone;
           the old stone's number multiplied by 2024 is engraved on the new stone."

    initial arrangement of stones: 125 17
    after 25 blinks: 55312 stones

*/

struct input_data {
    std::vector<uint_least64_t> stones;
};


// return the number of decimal digits needed to represent n
unsigned number_of_decimal_digits(uint_least64_t n)
{
    unsigned count = 1;
    while (n /= 10)
        ++count;
    return count;
};

constexpr uint_least64_t decimal_divisors[] = {
    1ULL,
    10ULL,
    100ULL,
    1000ULL,
    10000ULL,
    100000ULL,
    1000000ULL,
    10000000ULL,
    100000000ULL,
    1000000000ULL,
    10000000000ULL,
    100000000000ULL,
    1000000000000ULL,
    10000000000000ULL,
    100000000000000ULL,
    1000000000000000ULL,
    10000000000000000ULL,
    100000000000000000ULL,
    1000000000000000000ULL,
    10000000000000000000ULL,
};

// used to combine stone value and number of blinks into one 64-bit number
constexpr unsigned blink_bit_shift = 7;


// return the number of stones the given stone will become after the given number of blinks
uint_least64_t total_stones_after_n_blinks(uint_least64_t stone_value, unsigned number_of_blinks)
{
    // Looking at the stones proliferate you notice the same face-values oft repeat.
    // memoize results: map (stone_value, number_of_blinks) -> total_stones
    static std::unordered_map<uint_least64_t, uint_least64_t> memo;

    if (number_of_blinks == 0)
        return 1; // one given stone remains unchanged unless you blink

    const uint_least64_t p = (stone_value << blink_bit_shift) | number_of_blinks;
    if (memo.contains(p))
        return memo[p];

    uint_least64_t result = 0;

    if (stone_value == 0) {
        // rule 1 applies: replace 0 with 1
        result = total_stones_after_n_blinks(1, number_of_blinks - 1);
    }
    else {
        const auto digits = number_of_decimal_digits(stone_value);
        if ((digits & 1) == 0) {
            // rule 2 applies: replace stone with two new stones
            const uint_least64_t m = decimal_divisors[digits / 2];
            result = total_stones_after_n_blinks(stone_value / m, number_of_blinks - 1)  // left stone
                   + total_stones_after_n_blinks(stone_value % m, number_of_blinks - 1); // right stone
        }
        else {
            // rule 3 applies: multiply by 2024
            result = total_stones_after_n_blinks(stone_value * 2024, number_of_blinks - 1);
        }
    }

    return memo[p] = result;
}


// return the number of stones the given stones will become after the given number of blinks
uint_least64_t total_stones_after_n_blinks(std::vector<uint_least64_t> stones, unsigned number_of_blinks)
{
    assert(number_of_blinks < (1 << blink_bit_shift));
    uint_least64_t total = 0;
    for (auto s : stones)
        total += total_stones_after_n_blinks(s, number_of_blinks);
    return total;
};



void solve(const input_data & input, uint_least64_t & part1_result, uint_least64_t & part2_result)
{
    part1_result = part2_result = 0;


#ifdef MY_FIRST_ATTEMPT
    std::vector<uint_least64_t> stones{ input.stones };

    auto number_of_digits = [](uint_least64_t n) {
        unsigned count = 1;
        while (n /= 10)
            ++count;
        return count;
    };

    auto blink = [&](std::vector<uint_least64_t> & stones) {
        const unsigned end = stones.size();
        for (unsigned s = 0; s < end; ++s) {
            if (stones[s] == 0)
                stones[s] = 1;
            else {
                auto digits = number_of_digits(stones[s]);
                if ((digits & 1) == 0) {
                    digits /= 2;
                    uint_least64_t m = 1;
                    for (unsigned d = 0; d < digits; ++d)
                        m *= 10;
                    stones.push_back(stones[s] % m);
                    stones[s] /= m;
                }
                else {
                    //assert((stones[s] * 2024) / 2024 == stones[s]);
                    stones[s] *= 2024;
                }
            }
        }
    };

    for (int b = 1; b <= 25; ++b)
        blink(stones);
    part1_result = stones.size();
#endif

    part1_result = total_stones_after_n_blinks(input.stones, 25);

    part2_result = total_stones_after_n_blinks(input.stones, 75);
}



int main()
{
    std::ifstream input_file("input11.txt");
    if (!input_file.is_open())
        return EXIT_FAILURE;
    input_data input;
    for (uint_least64_t n = 0; input_file >> n; )
        input.stones.push_back(n);


    const auto start_time = std::chrono::high_resolution_clock::now();

    uint_least64_t part1_result = 0;
    uint_least64_t part2_result = 0;

    const int repetitions = 100;
    for (int i = 0; i < repetitions; ++i)
        solve(input, part1_result, part2_result);

    const std::chrono::duration<double, std::milli> ms = std::chrono::high_resolution_clock::now() - start_time;

    std::cout << part1_result << '\n';
    assert(part1_result == 193899);
    std::cout << part2_result << '\n';
    assert(part2_result == 229682160383225);
    std::cout << ms.count() / repetitions << "ms\n";
}

