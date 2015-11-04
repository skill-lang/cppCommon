//
// Created by feldentm on 03.11.15.
//

#include <gtest/gtest.h>
#include "../../src/utils.h"
#include "../../src/streams/streams.h"
#include "../../src/api/SkillException.h"

using namespace skill::streams;

InStream *open(std::string path = std::string("date.sf")) {
    return new FileInputStream(path);
}

TEST(Streams, Open) {
    auto s = open("emptyBlocks.sf");
    ASSERT_TRUE(nullptr != s);
    delete s;
}

TEST(Streams, OpenNonexisting) {
    ASSERT_THROW(open("non.existing.file.that.should.throw.an.exception"),
                 skill::SkillException);
}

TEST(Streams, ReadI8) {
    auto s = open();
    ASSERT_EQ(1, s->i8());
    delete s;
}

TEST(Streams, ReadI16) {
    auto s = open();
    ASSERT_EQ(1, s->i8());
    ASSERT_EQ(0, s->i16());
    ASSERT_EQ(4, s->i16());
    delete s;
}

TEST(Streams, ReadI16) {
    auto s = open();
    ASSERT_EQ(1, s->i8());
    ASSERT_EQ(0, s->i16());
    ASSERT_EQ(4, s->i16());
    delete s;
}

TEST(Streams, Eof) {
    auto s = open("emptyBlocks.sf");
    ASSERT_EQ(0, s->i8());
    ASSERT_EQ(0, s->i8());
    ASSERT_EQ(0, s->i8());
    ASSERT_FALSE(s->eof());
    ASSERT_EQ(0, s->i8());
    ASSERT_TRUE(s->eof());
    delete s;
}

TEST(Streams, ReadV64) {
    FAIL() << "not yet implemented";
}