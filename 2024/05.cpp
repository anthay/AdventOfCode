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
    
    // Note: although std::is_sorted() gives the correct result with my input,
    // (which is suprising?) it would not detect that 10|42 42,99,10 is not
    // sorted. The puzzle text says "The notation X|Y means that if both page
    // number X and page number Y are to be produced as part of an update,
    // page number X must be printed at some point before page number Y." And
    // also "47|53 [...] 47 doesn't necessarily need to be immediately before
    // 53; other pages are allowed to be between them."
    auto is_sorted = [&](auto begin, auto end, auto less) {
        for (; begin != end; ++begin)
            for (auto i = std::next(begin); i != end; ++i)
                if (less(*i, *begin))
                    return false;
        return true;
    };

    for (const auto & update : input.updates)
        if (is_sorted(update.begin(), update.end(), must_preceed))
            part1_result += update[update.size() / 2];


    // part 2

    // Note: std::stable_sort() gives the correct puzzle answer for my input!
    // But std::stable_sort() will NOT correctly sort
    //     10|42
    //     20|10
    //     42,99,98,10,20
    // it produces
    //     42,99,98,20,10
    // instead of the required
    //     20,10,42,99,98

    for (const auto & update : input.updates) {
        if (!is_sorted(update.begin(), update.end(), must_preceed)) {
            std::vector<unsigned> sorted{update};

            for (bool found_out_of_order = true; found_out_of_order;) {
                found_out_of_order = false;
                constexpr unsigned removed = 999999;
                std::vector<unsigned> unsorted{sorted};
                for (int src = 0, dst = 0; src != unsorted.size(); ++src) {
                    if (unsorted[src] == removed)
                        continue;
                    for (int s = src + 1; s != unsorted.size(); ++s) {
                        if (unsorted[s] == removed)
                            continue;
                        if (must_preceed(unsorted[s], unsorted[src])) {
                            sorted[dst++] = unsorted[s];
                            unsorted[s] = removed;
                            found_out_of_order = true;
                        }
                    }
                    sorted[dst++] = unsorted[src];
                }
            }

            part2_result += sorted[sorted.size() / 2];
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
