//
// Created by feldentm on 03.11.15.
//

#include <gtest/gtest.h>
#include "../../skill/api/Maps.h"

using namespace skill;
using namespace api;

TEST(Map, IntFloat) {
    Map<int32_t, float> store;
    std::map<int32_t, float> &map = store;
    map[2] = 32;
    GTEST_ASSERT_EQ(32, map[2]);

    BoxedMap *b = &store;
    GTEST_ASSERT_NE(32, b->get(box(2)).i32);
    GTEST_ASSERT_EQ(32, b->get(box(2)).f32);
    map[1] = 1;
    GTEST_ASSERT_EQ(1, b->get(box(1)).f32);
}

TEST(Map, FloatIntInt) {
    Map<float, Map<int64_t, int64_t> *> store;
    Map<int64_t, int64_t> storeInner;

    std::map<float, Map<int64_t, int64_t> *> *map = &store;
    (*map)[2] = &storeInner;
    storeInner[1] = 1;
    GTEST_ASSERT_EQ(1, map->at(2)->at(1));

    BoxedMap *b = &store;
    GTEST_ASSERT_NE(32, b->get(box(2.0f)).i32);
    GTEST_ASSERT_EQ(1, b->get(box(2.0f)).map->get(box(1L)).i64);
    GTEST_ASSERT_NE(true, b->contains(box(1.0f)));
    (*map)[1] = &storeInner;
    GTEST_ASSERT_EQ(true, b->contains(box(1.0f)));
}