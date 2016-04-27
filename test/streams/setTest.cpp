//
// Created by feldentm on 03.11.15.
//

#include <gtest/gtest.h>
#include "../../skill/api/Sets.h"

using namespace skill;
using namespace api;

TEST(Set, Char) {
    Set<int8_t> store;
    std::unordered_set<int8_t> &set = store;
    set.insert('3');
    set.insert('a');
    GTEST_ASSERT_NE(set.end(), set.find('3'));
    GTEST_ASSERT_EQ(set.end(), set.find('5'));

    BoxedSet *b = &store;
    GTEST_ASSERT_EQ(true, b->contains(box('3')));
    GTEST_ASSERT_EQ(true, b->contains(box('a')));
    GTEST_ASSERT_EQ(2, b->length());
    set.insert('1');
    GTEST_ASSERT_EQ(true, b->contains(box('1')));
    GTEST_ASSERT_EQ(3, b->length());
}