#include <iostream>
#include <string>
#include <string_view>

#include "thirdparty/parser.hpp"

using namespace aria::csv;

int64_t ToInt(std::string_view s) {
    int64_t result = 0;
    for (auto c : s) result = result * 10 + (c - '0');
    return result;
}

int64_t sum_extendedprice(const std::string& file_name) {
    std::ifstream f(file_name);
    CsvParser lineitem = CsvParser(f).delimiter('|');

    int64_t price_sum = 0;
    uint64_t field_number = 0;
    for (auto row : lineitem) {
        for (const auto& field : row) {
            //-- TODO exercise 2.5
            // your code goes here
            //--
        }
    }

    return price_sum;
}

// Tasks:
// * Compare this to awk execution time
// * Estimate how fast the program can possibly be on you local machine. Assume
//   that the input file is read from memory
// * Find out where most of the time is spent and why this program is not even
//   close to your estimates. Tools that can help here: perf stat, perf record,
//   perf report
// *
