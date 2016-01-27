//
// Created by feldentm on 03.11.15.
//

#include <gtest/gtest.h>
#include "../../skill/api/Arrays.h"

using namespace skill;
using namespace api;

TEST(Array, IntArray) {
    Array<int32_t> arr(32);
    arr[2] = 32;
    GTEST_ASSERT_EQ(32, arr.at(2));

    BoxedArray *b = &arr;
    GTEST_ASSERT_EQ(32, b->get(2).i32);
    arr[1] = 1;
    GTEST_ASSERT_EQ(1, b->get(1).i32);
}