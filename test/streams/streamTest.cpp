//
// Created by feldentm on 03.11.15.
//

#include <gtest/gtest.h>
#include "../../src/utils.h"
#include "../../src/streams/streams.h"
#include "../../src/api/SkillException.h"

using namespace skill::streams;

FileInputStream *open(std::string path = std::string("date.sf")) {
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

TEST(Streams, ReadI32) {
    auto s = open();
    ASSERT_EQ(1, s->i8());
    ASSERT_EQ(4, s->i32());
    delete s;
}

TEST(Streams, ReadI64) {
    auto s = open();
    ASSERT_EQ(72057594111615348L, s->i64());
    delete s;
}

TEST(Streams, ReadV64) {
    auto s = open();
    ASSERT_EQ(1, s->i8());
    ASSERT_EQ(4, s->i32());
    while (!s->eof())
        s->v64();
    delete s;
}

TEST(Streams, ReadF32) {
    auto s = open();
    // ~2.35098870e-38
    auto f = s->f32();
    ASSERT_TRUE(2.35e-38 < f && f < 2.36e-38);
    delete s;
}

TEST(Streams, ReadF32Empty) {
    auto s = open("emptyBlocks.sf");
    ASSERT_EQ(0.0f, s->f32());
    delete s;
}

TEST(Streams, ReadF64) {
    auto s = open();
    // ~7.2911221388529506e-304
    auto f = s->f64();
    ASSERT_TRUE(7.2911e-304 < f && f < 7.2912e-304);
    delete s;
}

TEST(Streams, Boolean) {
    auto s = open();
    ASSERT_TRUE(s->boolean());
    ASSERT_FALSE(s->boolean());
    ASSERT_FALSE(s->boolean());
    ASSERT_FALSE(s->boolean());
    ASSERT_TRUE(s->boolean());
    ASSERT_TRUE(s->boolean());
    delete s;
}

TEST(Streams, String) {
    auto s = open();
    ASSERT_TRUE(s->boolean());
    auto str = s->string(s->i32());
    ASSERT_STREQ(str->c_str(), "date");
    delete str;
    delete s;
}

TEST(Streams, StringMapped) {
    auto s = open();
    ASSERT_TRUE(s->boolean());
    int length = s->i32();
    auto map = s->jumpAndMap(length);
    ASSERT_EQ(9, s->getPosition());
    auto str = map->string(length);
    ASSERT_TRUE(map->eof());
    delete map;
    ASSERT_FALSE(s->eof());
    ASSERT_STREQ(str->c_str(), "date");
    delete str;
    delete s;
}


TEST(Streams, ReadV64Large) {
    auto s = open("age16.sf");
    while (!s->eof())
        s->v64();
    delete s;
}