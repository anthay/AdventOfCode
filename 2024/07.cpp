// https://adventofcode.com/2024/day/7

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
    "some young elephants were playing nearby and stole all the operators
     from their calibration equations"

    190: 10 19
    3267: 81 40 27
    83: 17 5
    156: 15 6
    7290: 6 8 6 15
    161011: 16 10 13
    192: 17 8 14
    21037: 9 7 18 13
    292: 11 6 16 20

    part 1: 3749

    part 2: 11387
*/

struct input_data {
    struct equation {
        uint_least64_t answer;
        std::vector<unsigned> terms;
    };
    std::vector<equation> tests;
};

void solve(const input_data & input, uint_least64_t & part1_result, uint_least64_t & part2_result)
{
    part1_result = part2_result = 0;


    std::vector<bool> found_solution(input.tests.size(), false);

    // part 1

    for (int i = 0; i < input.tests.size(); ++i) {
        const auto & test = input.tests[i];
        uint_least32_t operator_combinations = 1 << (test.terms.size() - 1);
        while (operator_combinations--) {
            auto term = test.terms.begin();
            uint_least64_t result = *term++;
            for (unsigned op = 1; term != test.terms.end(); ++term, op <<= 1) {
                if (operator_combinations & op)
                    result *= *term;
                else
                    result += *term;
            }
            if (result == test.answer) {
                // there is at least one combination of operators that gives the specified answer
                part1_result += result;
                found_solution[i] = true;
                break;
            }
        }
    }


    // part 2

    part2_result = part1_result;

    for (int i = 0; i < input.tests.size(); ++i) {
        if (found_solution[i])
            continue;
        const auto & test = input.tests[i];

        uint_least64_t operator_combinations = 1 << (test.terms.size() - 1) * 2; // 2 bits per operator
        while (operator_combinations--) {
            // replace all 11b operator indexes with 10b; we only want indexes 0, 1 and 2
            operator_combinations &= ~((operator_combinations & 0xAAAAAAAAAAAAAAAAULL) >> 1);

            auto term = test.terms.begin();
            uint_least64_t result = *term++;
            for (uint_least64_t op = operator_combinations; term != test.terms.end(); ++term, op >>= 2) {
                switch (op & 3) {
                case 0: result *= *term;    break;
                case 1: result += *term;    break;
                default:
                    // concatenation operator
                    unsigned t = *term;
                    do {
                        result *= 10;
                        t /= 10;
                    } while (t);
                    result += *term;
                    break;
                }
                if (result > test.answer)
                    break; // (all terms appear to be positive - no point continuing if result is already too big)
            }

            if (result == test.answer) {
                part2_result += result;
                break;
            }
        }
    }
}



int main()
{
    std::ifstream input_file("input07.txt");
    if (!input_file.is_open())
        return EXIT_FAILURE;
    input_data input;
    std::string line;
    while (std::getline(input_file, line) && !line.empty()) {
        std::istringstream reader(line);
        input_data::equation equ;
        char colon;
        reader >> equ.answer >> colon;
        unsigned term;
        while (reader >> term)
            equ.terms.push_back(term);
        if (equ.terms.size() < 2)
            return EXIT_FAILURE; // (this implementation expects at least 2 terms)
        if (equ.terms.size() > 32)
            return EXIT_FAILURE; // (this implementation limited to 31 operators)
        input.tests.emplace_back(equ);
    }



    const auto start_time = std::chrono::high_resolution_clock::now();

    uint_least64_t part1_result = 0;
    uint_least64_t part2_result = 0;

    const int repetitions = 5;
    for (int i = 0; i < repetitions; ++i)
        solve(input, part1_result, part2_result);

    const std::chrono::duration<double, std::milli> ms = std::chrono::high_resolution_clock::now() - start_time;

    std::cout << part1_result << '\n';
    assert(part1_result == 1289579105366);
    std::cout << part2_result << '\n';
    assert(part2_result == 92148721834692);
    std::cout << ms.count() / repetitions << "ms\n";
}
