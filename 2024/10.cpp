// https://adventofcode.com/2024/day/10

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

    "[...] a hiking trail is any path that starts at height 0, ends at height 9,
     and always increases by a height of exactly 1 at each step."

    "A trailhead is any position that starts one or more hiking trails - here,
     these positions will always have height 0. Assembling more fragments of pages,
     you establish that a trailhead's score is the number of 9-height positions
     reachable from that trailhead via a hiking trail."

    89010123
    78121874
    87430965
    96549874
    45678903
    32019012
    01329801
    10456732

    part 1: 9 trail heads, score 36

    "A trailhead's rating is the number of distinct hiking trails which begin at
     that trailhead."

    part 2: rating 81

*/

struct input_data {
    // each map[r * map_cols + c] in range [0..9]
    std::vector<char> map;
    int map_cols;
    int map_rows;
};

void solve(const input_data & input, uint_least64_t & part1_result, uint_least64_t & part2_result)
{
    part1_result = part2_result = 0;


    // parts 1 and 2 combined

    // return the number of unique summits and trails reachable from the given trailhead map index
    auto count_reachable_summits = [&](unsigned start_index) {

        unsigned count = 0;
        std::unordered_set<unsigned> summits_reached;
        std::vector<unsigned> unexplored;
        unexplored.push_back(start_index);

        auto check = [&](unsigned next_step, unsigned index) {
            if (input.map[index] == next_step) {
                if (next_step == 9) {
                    ++count;
                    summits_reached.insert(index);
                }
                else
                    unexplored.push_back(index);
            }
        };

        while (!unexplored.empty()) {
            const unsigned index = unexplored.back();
            unexplored.pop_back();
            const unsigned row = index / input.map_cols;
            const unsigned col = index % input.map_cols;
            const unsigned next_step = input.map[index] + 1;

            if (row > 0)                  check(next_step, index - input.map_cols);
            if (row < input.map_rows - 1) check(next_step, index + input.map_cols);
            if (col > 0)                  check(next_step, index - 1);
            if (col < input.map_cols - 1) check(next_step, index + 1);
        }

        return std::make_pair(summits_reached.size(), count);
    };

    for (unsigned index = 0; index < input.map.size(); ++index) {
        if (input.map[index] == 0) {
            const auto [reachable_summits, trails_to_summits] = count_reachable_summits(index);
            part1_result += reachable_summits;
            part2_result += trails_to_summits;
        }
    }
}



int main()
{
    std::ifstream input_file("input10.txt");
    if (!input_file.is_open())
        return EXIT_FAILURE;
    input_data input;
    std::string line;
    std::getline(input_file, line);
    input.map_cols = line.size();
    input.map_rows = 0;
    do {
        for (auto c : line)
            input.map.push_back(c - '0');
        ++input.map_rows;
    } while (std::getline(input_file, line) && !line.empty());



    const auto start_time = std::chrono::high_resolution_clock::now();

    uint_least64_t part1_result = 0;
    uint_least64_t part2_result = 0;

    const int repetitions = 1000;
    for (int i = 0; i < repetitions; ++i)
        solve(input, part1_result, part2_result);

    const std::chrono::duration<double, std::milli> ms = std::chrono::high_resolution_clock::now() - start_time;

    std::cout << part1_result << '\n';
    assert(part1_result == 754);
    std::cout << part2_result << '\n';
    assert(part2_result == 1609);
    std::cout << ms.count() / repetitions << "ms\n";
}

