#include <gtest/gtest.h>
#include <cstring>
#include <fstream>
#include "sum.hpp"
#include "util.hpp"

//---------------------------------------------------------------------------
TEST(SumTest, SumExtendedPrice)
/// test if sum_extendedprice()
{
    auto lineitem_file = getDir(__FILE__) + "/data/tpch/sf0_001/lineitem.tbl";
    int64_t result = sum_extendedprice(lineitem_file);
    ASSERT_EQ(result, 15277439838ul);
}
//---------------------------------------------------------------------------
