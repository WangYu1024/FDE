#include <iostream>
#include <string>

#include "sum.hpp"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <lineitem.tbl>";
        return 1;
    }

    int64_t price_sum = sum_extendedprice(argv[1]);
    std::cout << price_sum << std::endl;
    return 0;
}
