// https://adventofcode.com/2024/day/6

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



constexpr char off_the_map = 0;

struct input_data {
    // map is the puzzle input surrounded by off_the_map sentinels
    std::vector<char> map;
    int map_width;
    int start_at;
};

void solve(const input_data & input, unsigned & part1_result, unsigned & part2_result)
{
    part1_result = part2_result = 0;

    // part 1

    enum heading {N, E, S, W};
    heading head = N;
    const int stride[] = {
        -input.map_width,   // N
        +1,                 // E
        +input.map_width,   // S
        -1                  // W
    };
    heading turn_right[] = {E, S, W, N};

    std::vector<bool> visited(input.map.size(), false);
    visited[input.start_at] = true;
    part1_result = 1;

    for (int location = input.start_at;;) {
        int new_location = location + stride[head];
        if (input.map[new_location] == off_the_map)
            break;
        if (input.map[new_location] == '#')
            head = turn_right[head];
        else {
            location = new_location;
            if (!visited[location]) {
                visited[location] = true;
                ++part1_result;
            }
        }
    }


    // part 2

    constexpr char untrodden = -1;
    std::vector<char> path(input.map.size());
    const int last_location = input.map.size() - input.map_width;
    for (int obstacle_location = input.map_width; obstacle_location < last_location; ) {
        if (visited[obstacle_location] && obstacle_location != input.start_at) {
            heading head = N;
            std::fill(path.begin(), path.end(), untrodden);
            path[input.start_at] = head;
            for (int location = input.start_at;;) {
                int new_location = location + stride[head];
                if (input.map[new_location] == off_the_map)
                    break;
                if (input.map[new_location] == '#' || new_location == obstacle_location)
                    head = turn_right[head];
                else
                    location = new_location;
                if (path[location] == untrodden)
                    path[location] = head;
                else if (path[location] == head) {
                    // we previously passed by this spot going in this direction; must be a loop
                    ++part2_result;
                    break;
                }
            }
        }
        ++obstacle_location;
        if (input.map[obstacle_location] == off_the_map)
           ++obstacle_location;
    }
}



int main()
{
    std::string input_text;
    std::ifstream input_file("input06.txt");
    if (!input_file.is_open())
        return EXIT_FAILURE;

    input_data input;
    std::string line;
    std::getline(input_file, line);
    input.map_width = line.size() + 1;
    input.map.resize(input.map_width);
    std::fill(input.map.begin(), input.map.end(), off_the_map);
    for (auto c : line)
        input.map.push_back(c);
    input.map.push_back(off_the_map);

    while (std::getline(input_file, line) && !line.empty()) {
        for (auto c : line)
            input.map.push_back(c);
        input.map.push_back(off_the_map);
    }

    for (unsigned i = 0; i < input.map_width; ++i)
        input.map.push_back(off_the_map);

    input.start_at = std::distance(input.map.begin(), std::find(input.map.begin(), input.map.end(), '^'));


    const auto start_time = std::chrono::high_resolution_clock::now();

    unsigned part1_result = 0;
    unsigned part2_result = 0;

    const int repetitions = 10;
    for (int i = 0; i < repetitions; ++i)
        solve(input, part1_result, part2_result);

    const std::chrono::duration<double, std::milli> ms = std::chrono::high_resolution_clock::now() - start_time;

    std::cout << part1_result << '\n';
    assert(part1_result == 5212);
    std::cout << part2_result << '\n';
    assert(part2_result == 1767);
    std::cout << ms.count() / repetitions << "ms\n";
}
