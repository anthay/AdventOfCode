// https://adventofcode.com/2024/day/9

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

    "The digits alternate between indicating the length of a file and the
     length of free space."

    free space condenced map:

    2333133121414131402


    free space expanded:

    00...111...2...333.44.5555.6666.777.888899

    compacted:

    00...111...2...333.44.5555.6666.777.888899
    009..111...2...333.44.5555.6666.777.88889.
    0099.111...2...333.44.5555.6666.777.8888..
    00998111...2...333.44.5555.6666.777.888...
    009981118..2...333.44.5555.6666.777.88....
    0099811188.2...333.44.5555.6666.777.8.....
    009981118882...333.44.5555.6666.777.......
    0099811188827..333.44.5555.6666.77........
    00998111888277.333.44.5555.6666.7.........
    009981118882777333.44.5555.6666...........
    009981118882777333644.5555.666............
    00998111888277733364465555.66.............
    0099811188827773336446555566..............

    part 1 checksum: 1928


    part 2 compacted:

    00...111...2...333.44.5555.6666.777.888899
    0099.111...2...333.44.5555.6666.777.8888..
    0099.1117772...333.44.5555.6666.....8888..
    0099.111777244.333....5555.6666.....8888..
    00992111777.44.333....5555.6666.....8888..

    part 2 checksum: 2858
*/

struct input_data {
    std::vector<char> map;

    input_data() = default;
    input_data(const std::string & s)
        : map(s.begin(), s.end())
    {}
};

void solve(const input_data & input, uint_least64_t & part1_result, uint_least64_t & part2_result)
{
    part1_result = part2_result = 0;


    // part 1

    // create block id map (00...111...2...333.44.5555.6666.777.888899)
    std::vector<int> block_id_map;
    block_id_map.reserve(input.map.size() * 9);
    constexpr int free_block_id = -1;
    int file_id = 0;
    for (int i = 0; i < input.map.size(); ++i) {
        const int length = input.map[i] - '0';
        const int id = (i & 1) ? free_block_id : file_id++;
        for (int j = 0; j < length; ++j)
            block_id_map.push_back(id);
    }

    // compact map
    std::vector<int> compacted_map{block_id_map};
    for (int left = 0, right = compacted_map.size() - 1; left < right; ) {
        if (compacted_map[left] != free_block_id) {
            ++left;
            continue;
        }
        while (left < right && compacted_map[right] == free_block_id)
            --right;
        if (left == right)
            break;
        compacted_map[left++] = compacted_map[right];
        compacted_map[right--] = free_block_id;
    }

    // calculate checksum
    auto checksum = [&](const std::vector<int> & map) {
        uint_least64_t csum = 0;
        for (unsigned block_index = 0; block_index < map.size(); ++block_index) {
            if (map[block_index] == free_block_id)
                continue;
            csum += static_cast<uint_least64_t>(block_index) * map[block_index];
        }
        return csum;
    };

    part1_result = checksum(compacted_map);


    // part 2

    int first_free = 0;  // block index of first block in first free space
    int last_file = block_id_map.size() - 1; // index of first block in last file

    while (first_free < last_file) {
        // move first_free forward, if necessary (i.e. if it does not point to free space)
        while (first_free < block_id_map.size() && block_id_map[first_free] != free_block_id)
            ++first_free;
        if (first_free == block_id_map.size())
            break; // no more free space - can't move any(more) files

        // find the last block in the next last file
        while (last_file > first_free && block_id_map[last_file] == free_block_id)
            --last_file;
        const int file_id = block_id_map[last_file];
        const int last_file_last_index = last_file;

        // find the first block in this last file
        while (last_file > first_free && block_id_map[last_file - 1] == file_id)
            --last_file;
        if (last_file == first_free)
            break; // there were no more files to move
        const int last_file_length = last_file_last_index - last_file + 1;

        // find a contiguous block of free space big enough to contain this file
        for (int next_free = first_free; next_free < last_file; ) {
            // find length of free block
            int free_length = 0;
            for (int i = next_free; i < last_file && block_id_map[i] == free_block_id; ++i)
                ++free_length;

            // is it big enough?
            if (free_length >= last_file_length) {
                // yes, move file
                for (int source = last_file, count = 0; count < last_file_length; ++count) {
                    block_id_map[next_free++] = file_id;
                    block_id_map[source++] = free_block_id;
                }
                break; // this file successfully moved
            }

            // this block of free space not big enough; move on to the next block
            next_free += free_length;
            while (next_free < last_file && block_id_map[next_free] != free_block_id)
                ++next_free;
        }

        if (last_file == 0)
            break;
        --last_file;
    }

    part2_result = checksum(block_id_map);
}



int main()
{
    std::ifstream input_file("input09.txt");
    if (!input_file.is_open())
        return EXIT_FAILURE;
    std::string line;
    std::getline(input_file, line);
    input_data input{line};



    const auto start_time = std::chrono::high_resolution_clock::now();

    uint_least64_t part1_result = 0;
    uint_least64_t part2_result = 0;

    const int repetitions = 10;
    for (int i = 0; i < repetitions; ++i)
        solve(input, part1_result, part2_result);

    const std::chrono::duration<double, std::milli> ms = std::chrono::high_resolution_clock::now() - start_time;

    std::cout << part1_result << '\n';
    assert(part1_result == 6225730762521);
    std::cout << part2_result << '\n';
    assert(part2_result == 6250605700557);
    std::cout << ms.count() / repetitions << "ms\n";
}

