// https://adventofcode.com/2024/day/1

#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <unordered_map>
#include <cassert>
#include <chrono>


void solve(std::vector<int> left_list, std::vector<int> right_list,
    int_least64_t & total_distance, int_least64_t & similarity_score)
{
    total_distance = similarity_score = 0;

    // part 1

    std::ranges::sort(left_list);
    std::ranges::sort(right_list);
    for (int i = 0; i < left_list.size(); ++i)
        total_distance += std::abs(left_list[i] - right_list[i]);

    // part 2

#ifdef USE_MAP
    std::unordered_map<int, int> right_unique_count;
    for (const int right : right_list)
        right_unique_count[right]++;
    for (const int left : left_list)
        if (right_unique_count.contains(left))
            similarity_score += static_cast<int_least64_t>(left) * right_unique_count[left];
#else
    int iright = 0;
    const int iright_end = right_list.size();
    for (const int left : left_list) {
        while (iright < iright_end && left > right_list[iright])
            ++iright;
        if (iright == iright_end)
            break;
        const int iright_start = iright;
        while (iright < iright_end && left == right_list[iright])
            ++iright;
        similarity_score += static_cast<int_least64_t>(left) * (iright - iright_start);
    }
#endif
}


int main()
{
    std::vector<int> left_list, right_list;
    std::ifstream input("input01.txt");
    if (!input.is_open())
        return EXIT_FAILURE;
    for (int left, right; input >> left >> right;) {
        left_list.push_back(left);
        right_list.push_back(right);
    }

    const auto start_time = std::chrono::high_resolution_clock::now();

    int_least64_t total_distance = 0;
    int_least64_t similarity_score = 0;
    const int repetitions = 1000;
    for (int i = 0; i < repetitions; ++i)
        solve(left_list, right_list, total_distance, similarity_score);

    const std::chrono::duration<double, std::milli> ms = std::chrono::high_resolution_clock::now() - start_time;

    std::cout << total_distance << '\n';
    assert(total_distance == 1258579);
    std::cout << similarity_score << '\n';
    assert(similarity_score == 23981443);
    std::cout << "(" << ms.count() / repetitions << " ms)\n";
}
