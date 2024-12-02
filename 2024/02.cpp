// https://adventofcode.com/2024/day/2

#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <unordered_map>
#include <cassert>
#include <chrono>


int main()
{
    std::vector<std::vector<int>> reports;
    std::ifstream input("input02.txt");
    if (!input.is_open())
        return EXIT_FAILURE;
    for (std::string report_text; std::getline(input, report_text); ) {
        std::vector<int> report;
        std::istringstream report_stream(report_text);
        for (int level; report_stream >> level; )
            report.push_back(level);
        reports.push_back(report);
    }

    const auto start_time = std::chrono::high_resolution_clock::now();

    // part 1
    auto safe_report = [](const std::vector<int> & report) -> bool {
        // (assume all reports have at least two levels)
        const bool increasing = report[1] > report[0];
        for (unsigned i = 1; i < report.size(); ++i) {
            int difference = increasing ? report[i] - report[i-1] : report[i-1] - report[i];
            if (difference < 1 || difference > 3)
                return false;
        }
        return true;
    };
    int total_safe_reports = 0;
    for (const auto & report : reports)
        if (safe_report(report))
            ++total_safe_reports;


    // part 2
    int total_safe_reports_with_dampener = 0;

#ifdef USE_COPY_ERASE
    for (const auto & report : reports)
        if (safe_report(report))
            ++total_safe_reports_with_dampener;
        else
            for (unsigned i = 0; i < report.size(); ++i) {
                auto dampened_report{report};
                dampened_report.erase(dampened_report.begin() + i);
                if (safe_report(dampened_report)) {
                    ++total_safe_reports_with_dampener;
                    break;
                }
            }
#else
    auto safe_report_with_dampener = [](const std::vector<int> & report) -> bool {
        for (unsigned avoid_index = 0; avoid_index < report.size(); ++avoid_index) {
            auto dampened_report = [&](unsigned index) {
                return report[(index < avoid_index) ? index : index + 1];
            };
            auto safe_dampened_report = [&](const std::vector<int> & report) {
                const bool increasing = dampened_report(1) > dampened_report(0);
                for (unsigned i = 1; i < report.size() - 1; ++i) {
                    const int difference = increasing
                        ? dampened_report(i) - dampened_report(i-1)
                        : dampened_report(i-1) - dampened_report(i);
                    if (difference < 1 || difference > 3)
                        return false;
                }
                return true;
            };
            if (safe_dampened_report(report))
                return true;
        }
        return false;
    };
    for (const auto & report : reports)
        if (safe_report(report) || safe_report_with_dampener(report))
            ++total_safe_reports_with_dampener;
#endif

    const std::chrono::duration<double, std::milli> ms = std::chrono::high_resolution_clock::now() - start_time;

    std::cout << total_safe_reports << '\n';
    assert(total_safe_reports == 502);
    std::cout << total_safe_reports_with_dampener << '\n';
    assert(total_safe_reports_with_dampener == 544);
    std::cout << ms.count() << "ms\n";
}
