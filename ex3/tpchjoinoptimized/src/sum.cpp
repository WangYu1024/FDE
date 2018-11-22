#include <cassert>
#include <memory>
#include <numeric>
#include <string>
#include <string_view>

int64_t ToInt(std::string_view s) {
    int64_t result = 0;
    for (auto c : s) result = result * 10 + (c - '0');
    return result;
}

// pattern : the character to search broadcasted into a 64-bit integer
// block : memory block in which to search
// return : 64-bit integer with all the matches as seen in the lecture
inline uint64_t get_matches(uint64_t pattern, uint64_t block) {
    //-- TODO exercise 3.4 part 2
    // your code goes here
    //--
}

// pattern : the character to search broadcasted into a 64bit integer
// begin : points somewhere into the partition
// len : remaining length of the partition
// return : the position of the first matching character or -1 otherwise
ssize_t find_first(uint64_t pattern, const char* begin, size_t len) {
    // locate the position of the following character (you'll have to use the
    // pattern argument directly in the second part of the exercise)
    const char to_search = pattern & 0xff;

    // Hint: You may assume that reads from 'begin' within [len, len + 8) yield
    // zero
    //-- TODO exercise 3.4
    // your code goes here
    //--
    return -1;
}

// begin : points somewhere into the partition
// len : remaining length of the partition
// return : 64-bit integer representation of the provided numeric
int64_t read_numeric(const char* begin, size_t len) {
    constexpr uint64_t period_pattern = 0x2E2E2E2E2E2E2E2Eull;
    std::string_view numeric_view(begin, len);
    ssize_t dot_position = find_first(period_pattern, begin, len);
    assert(dot_position > 0);
    auto part1 = numeric_view.substr(0, dot_position);
    auto part2 = numeric_view.substr(dot_position + 1);
    int64_t numeric = ToInt(part1) * 100 + ToInt(part2);
    return numeric;
}
