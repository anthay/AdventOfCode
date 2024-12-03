// https://adventofcode.com/2024/day/3

#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <unordered_map>
#include <cassert>
#include <chrono>
#include <string>
#include <regex>



void solve(const std::string & input_text, unsigned & part1_sum, unsigned & part2_sum)
{
    part1_sum = part2_sum = 0;

#ifdef USE_REGEX
    // part 1
    std::regex mul_regex("mul\\(([0-9]{1,3}),([0-9]{1,3})\\)");
    auto words_begin = std::sregex_iterator(input_text.begin(), input_text.end(), mul_regex);
    auto words_end = std::sregex_iterator();
    for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
        std::smatch match = *i;
        part1_sum += std::stoi(match[1]) * std::stoi(match[2]);
    }

    // part 2
    std::regex do_dont_mul_regex("do\\(\\)|don't\\(\\)|mul\\(([0-9]{1,3}),([0-9]{1,3})\\)");
    words_begin = std::sregex_iterator(input_text.begin(), input_text.end(), do_dont_mul_regex);
    words_end = std::sregex_iterator();
    bool enabled = true;
    for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
        std::smatch match = *i;
        if (match[0] == "do()")
            enabled = true;
        else if (match[0] == "don't()")
            enabled = false;
        else if (enabled)
            part2_sum += std::stoi(match[1]) * std::stoi(match[2]);
    }
#else
    // part 1
    auto match_here = [](const char * c, const char * const c_end, const char * str) -> bool {
        for (; *str && c != c_end && *str == *c; ++str, ++c)
            ;
        return *str == '\0';
    };

    auto read_digits = [](const char * c, const char * const c_end, unsigned & value) -> int {
        value = 0;
        int digit_count = 0;
        for (; c != c_end && std::isdigit(*c); ++c, ++digit_count)
            value = value * 10 + *c - '0';
        return digit_count;
    };

    auto read_mul = [&](const char * const c_begin, const char * const c_end, unsigned & value) -> int {
        const char * c = c_begin;
        if (!match_here(c, c_end, "mul("))
            return 0;
        c += 4;
        if (c == c_end)
            return 0;
        unsigned a, b;
        int digit_count = read_digits(c, c_end, a);
        if (digit_count < 1 || digit_count > 3)
            return 0;
        c += digit_count;
        if (c == c_end)
            return 0;
        if (*c != ',')
            return 0;
        ++c;
        if (c == c_end)
            return 0;
        digit_count = read_digits(c, c_end, b);
        if (digit_count < 1 || digit_count > 3)
            return 0;
        c += digit_count;
        if (c == c_end)
            return 0;
        if (*c != ')')
            return 0;
        ++c;
        value = a * b;
        return c - c_begin;
    };

    const char * c = input_text.c_str();
    const char * const c_end = c + input_text.size();
    while (c != c_end) {
        unsigned product = 0;
        const unsigned mul_length = read_mul(c, c_end, product);
        if (mul_length == 0)
            ++c;
        else {
            c += mul_length;
            part1_sum += product;
        }
    }

    // part 2
    c = input_text.c_str();
    while (c != c_end) {
        if (match_here(c, c_end, "don't()")) {
            c += 7;
            while (c != c_end && !match_here(c, c_end, "do()"))
                ++c;
            if (c == c_end)
                break;
            c += 4;
            continue;
        }
        unsigned product = 0;
        const unsigned mul_length = read_mul(c, c_end, product);
        if (mul_length == 0)
            ++c;
        else {
            c += mul_length;
            part2_sum += product;
        }
    }
#endif
}



int main()
{
    std::string input_text;
    std::ifstream input("input03.txt");
    if (!input.is_open())
        return EXIT_FAILURE;
    for (std::string line; std::getline(input, line); )
        input_text += line;


    const auto start_time = std::chrono::high_resolution_clock::now();

    unsigned part1_sum = 0;
    unsigned part2_sum = 0;

    const int repetitions = 20;
    for (int i = 0; i < repetitions; ++i)
        solve(input_text, part1_sum, part2_sum);

    const std::chrono::duration<double, std::milli> ms = std::chrono::high_resolution_clock::now() - start_time;

    std::cout << part1_sum << '\n';
    assert(part1_sum == 178794710);
    std::cout << part2_sum << '\n';
    assert(part2_sum == 76729637);
    std::cout << ms.count() / repetitions << "ms\n";
}
