// https://adventofcode.com/2024/day/8

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
    antennas:

    ............
    ........0...
    .....0......
    .......0....
    ....0.......
    ......A.....
    ............
    ............
    ........A...
    .........A..
    ............
    ............


    part 1: 14 antinodes:

    ......#....#
    ...#....0...
    ....#0....#.
    ..#....0....
    ....0....#..
    .#....A.....
    ...#........
    #......#....
    ........A...
    .........A..
    ..........#.
    ..........#.


    "After updating your model, it turns out that an antinode occurs at any grid
     position exactly in line with at least two antennas of the same frequency,
     regardless of distance. This means that some of the new antinodes will occur
     at the position of each antenna (unless that antenna is the only one of its
     frequency)."

    part 2: 34 antinodes:

    ##....#....#
    .#.#....0...
    ..#.#0....#.
    ..##...0....
    ....0....#..
    .#...#A....#
    ...#..#.....
    #....#.#....
    ..#.....A...
    ....#....A..
    .#........#.
    ...#......##
*/

struct input_data {
    std::vector<char> map;
    int map_cols;
    int map_rows;
};

void solve(const input_data & input, uint_least64_t & part1_result, uint_least64_t & part2_result)
{
    part1_result = part2_result = 0;


    // part 1

    auto on_map = [&](int r, int c) {
        return 0 <= r && r < input.map_rows
            && 0 <= c && c < input.map_cols;
    };

    // find the antennas
    std::unordered_map<char, std::vector<std::pair<int, int>>> antennas;
    for (int r = 0; r < input.map_rows; ++r) {
        for (int c = 0; c < input.map_cols; ++c) {
            const char frequency = input.map[r * input.map_cols + c];
            if (frequency != '.')
                antennas[frequency].emplace_back(std::make_pair(r, c));
        }
    }

    // find the antinodes
    std::unordered_set<int> antinodes;
    for (const auto & [_, locations] : antennas) {
        for (auto [r1, c1] : locations) {
            for (auto [r2, c2] : locations) {
                if (r1 == r2 && c1 == c2)
                    continue;
                const int antinode_r = r2 + (r2 - r1);
                const int antinode_c = c2 + (c2 - c1);
                if (on_map(antinode_r, antinode_c))
                    antinodes.insert(antinode_r * input.map_cols + antinode_c);
            }
        }
    }

    part1_result = antinodes.size();


    // part 2

    // find the antinodes
    antinodes.clear();
    for (const auto & [_, locations] : antennas) {
        for (auto [r1, c1] : locations) {
            for (auto [r2, c2] : locations) {
                if (r1 == r2 && c1 == c2)
                    continue;
                const int delta_r = r2 - r1;
                const int delta_c = c2 - c1;
                for (int antinode_r = r2, antinode_c = c2;
                         on_map(antinode_r, antinode_c);
                         antinode_r += delta_r, antinode_c += delta_c) {
                    antinodes.insert(antinode_r * input.map_cols + antinode_c);
                }
            }
        }
    }

    part2_result = antinodes.size();
}



int main()
{
    std::ifstream input_file("input08.txt");
    if (!input_file.is_open())
        return EXIT_FAILURE;

    input_data input;
    std::string line;
    std::getline(input_file, line);
    input.map_cols = line.size();
    input.map_rows = 0;
    do {
        input.map.insert(input.map.end(), line.begin(), line.end());
        ++input.map_rows;
    } while (std::getline(input_file, line) && !line.empty());



    const auto start_time = std::chrono::high_resolution_clock::now();

    uint_least64_t part1_result = 0;
    uint_least64_t part2_result = 0;

    const int repetitions = 100;
    for (int i = 0; i < repetitions; ++i)
        solve(input, part1_result, part2_result);

    const std::chrono::duration<double, std::milli> ms = std::chrono::high_resolution_clock::now() - start_time;

    std::cout << part1_result << '\n';
    assert(part1_result == 376);
    std::cout << part2_result << '\n';
    assert(part2_result == 1352);
    std::cout << ms.count() / repetitions << "ms\n";
}

