//
// Created by feldentm on 03.11.15.
//

#include <gtest/gtest.h>
#include "../../skill/utils.h"
#include "../../skill/streams/streams.h"
#include "../../skill/api/SkillException.h"
#include "../../skill/internal/StringPool.h"

using namespace skill;
using namespace internal;

TEST(String, AddLiteral) {
    auto pool = new StringPool(nullptr, new AbstractStringKeeper);
    auto s = "literal string";
    auto str = pool->addLiteral(s);
    GTEST_ASSERT_EQ(s, str->c_str());
    delete pool;
}