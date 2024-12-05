// https://adventofcode.com/2024/day/5

#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <unordered_map>
#include <cassert>
#include <chrono>
#include <string>
#include <unordered_set>


struct input_data {
    std::unordered_map<unsigned, std::unordered_set<unsigned>> ordering;
    std::vector<std::vector<unsigned>> updates;
};

void solve(const input_data & input, unsigned & part1_result, unsigned & part2_result)
{
    part1_result = part2_result = 0;

    // part 1

    auto must_preceed = [&](unsigned first, unsigned second) {
        auto order = input.ordering.find(first);
        if (order != input.ordering.end())
            return order->second.contains(second);
        return false;
    };
    
    for (const auto update : input.updates)
        if (std::is_sorted(update.begin(), update.end(), must_preceed))
            part1_result += update[update.size() / 2];


    // part 2

    for (const auto update : input.updates) {
        if (!std::is_sorted(update.begin(), update.end(), must_preceed)) {
            std::vector<unsigned> u{update};
            std::stable_sort(u.begin(), u.end(), must_preceed);
            part2_result += u[u.size() / 2];
        }
    }
}



int main()
{
    std::string input_text;
    std::ifstream input_file("input05.txt");
    if (!input_file.is_open())
        return EXIT_FAILURE;
    input_data input;
    std::string line;
    while (std::getline(input_file, line) && !line.empty()) {
        std::istringstream reader(line);
        unsigned a, b; char bar;
        reader >> a >> bar >> b;
        input.ordering[a].insert(b);
    }
    while (std::getline(input_file, line)) {
        std::istringstream reader(line);
        std::vector<unsigned> update;
        unsigned a;
        while (reader >> a) {
            update.push_back(a);
            char comma;
            reader >> comma;
        }
        input.updates.emplace_back(update);
    }


    const auto start_time = std::chrono::high_resolution_clock::now();

    unsigned part1_result = 0;
    unsigned part2_result = 0;

    const int repetitions = 1000;
    for (int i = 0; i < repetitions; ++i)
        solve(input, part1_result, part2_result);

    const std::chrono::duration<double, std::milli> ms = std::chrono::high_resolution_clock::now() - start_time;

    std::cout << part1_result << '\n';
    assert(part1_result == 4185);
    std::cout << part2_result << '\n';
    assert(part2_result == 4480);
    std::cout << ms.count() / repetitions << "ms\n";
}
