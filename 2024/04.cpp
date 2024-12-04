// https://adventofcode.com/2024/day/4

#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <unordered_map>
#include <cassert>
#include <chrono>
#include <string>
#include <regex>


struct input_data {
    std::string text;
    unsigned rows = 0;
    unsigned columns = 0;
};

void solve(const input_data & input, unsigned & total_xmases, unsigned & total_x_mases)
{
    total_xmases = total_x_mases = 0;

    // part 1
    
    const char x_forward[] = { "XMAS" };
    const char x_backward[] = { "SAMX" };
    const unsigned x_length = 4;

    auto point = [&](unsigned r, unsigned c) {
        return input.text.c_str() + r * input.columns + c;
    };

    // return true iff x_forward or x_backward exists at (r, c)..(r, c)+stride
    auto look = [&](unsigned r, unsigned c, unsigned stride) {
        const char * start = point(r, c);
        unsigned i = 0;
        for (const char * s = start; i < x_length; ++i, s += stride)
            if (*s != x_forward[i])
                break;
        if (i == x_length)
            return true;
        i = 0;
        for (const char * s = start; i < x_length; ++i, s += stride)
            if (*s != x_backward[i])
                break;
        return i == x_length;
    };

    for (unsigned r = 0; r < input.rows; ++r)
        for (unsigned c = 0; c < input.columns; ++c) {
            if (c <= input.columns - x_length) {
                total_xmases += look(r, c, 1);                      // east
                if (c <= input.columns - x_length)
                    total_xmases += look(r, c, input.columns + 1);  // south-east
            }
            if (r <= input.rows - x_length) {
                total_xmases += look(r, c, input.columns);          // south
                if (c >= x_length - 1)
                    total_xmases += look(r, c, input.columns - 1);  // south-west
            }
        }


    // part 2

    // return true iff X-MAS cross exists in 3x3 grid at (r, c) (top left corner of grid)
    auto look2 = [&](unsigned r, unsigned c) {
        const char * start = point(r, c);
        return (*(start + input.columns + 1) == 'A')
            && (   (*start == 'M' && *(start + 2 * input.columns + 2) == 'S')
                || (*start == 'S' && *(start + 2 * input.columns + 2) == 'M'))
            && (   (*(start + 2) == 'M' && *(start + 2 * input.columns) == 'S')
                || (*(start + 2) == 'S' && *(start + 2 * input.columns) == 'M'));
    };

    for (unsigned r = 0; r < input.rows - 2; ++r)
        for (unsigned c = 0; c < input.columns - 2; ++c)
            total_x_mases += look2(r, c);
}



int main()
{
    std::string input_text;
    //input_text.reserve(total_rows * total_columns);
    std::ifstream input_file("input04.txt");
    if (!input_file.is_open())
        return EXIT_FAILURE;
    input_data input;
    for (std::string line; std::getline(input_file, line); ) {
        input.text += line;
        ++input.rows;
        input.columns = line.size();
    }


    const auto start_time = std::chrono::high_resolution_clock::now();

    unsigned part1_count = 0;
    unsigned part2_count = 0;

    const int repetitions = 1000;
    for (int i = 0; i < repetitions; ++i)
        solve(input, part1_count, part2_count);

    const std::chrono::duration<double, std::milli> ms = std::chrono::high_resolution_clock::now() - start_time;

    std::cout << part1_count << '\n';
    assert(part1_count == 2521);
    std::cout << part2_count << '\n';
    assert(part2_count == 1912);
    std::cout << ms.count() / repetitions << "ms\n";
}
