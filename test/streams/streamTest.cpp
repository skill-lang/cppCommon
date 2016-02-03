//
// Created by feldentm on 03.11.15.
//

#include <gtest/gtest.h>
#include "../../skill/streams/streams.h"
#include "../../skill/streams/FileOutputStream.h"
#include "../../skill/streams/MappedOutStream.h"

using namespace skill::streams;

namespace streamTest {
    FileInputStream *open(std::string path = std::string("date.sf")) {
        return new FileInputStream(path);
    }

    FileOutputStream *write() {
        return new FileOutputStream(std::string(tmpnam(nullptr)), false);
    }
}
using namespace streamTest;

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
    auto str = s->string(s->i32(), 1);
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
    auto str = map->string(length, 1);
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

#include <chrono>

TEST(Streams, Write10MV64) {
    auto s = write();
    auto t = std::chrono::system_clock::now();
    for (int i = 0; i < 1e7; i++)
        s->v64(random());
    std::chrono::duration<double> took(std::chrono::system_clock::now() - t);
    std::cout << "throughput: " << (s->fileSize() * 1e-6 / took.count()) << " MB/s" << std::endl;
}

TEST(Streams, Write100MBofV64Mapped) {
    auto s = write();
    {
        auto t = std::chrono::system_clock::now();
        auto map = s->jumpAndMap((long) 1e8);
        while (map->has(9))
            map->v64(random());
        while (map->has(1))
            map->i8(0);
        s->unmap(map);
        std::chrono::duration<double> took(std::chrono::system_clock::now() - t);
        std::cout << "write throughput: " << (s->fileSize() * 1e-6 / took.count()) << " MB/s" << std::endl;
    }
    {
        auto in = open(s->filePath());
        auto t = std::chrono::system_clock::now();
        while (!in->eof())
            in->v64();
        std::chrono::duration<double> took(std::chrono::system_clock::now() - t);
        std::cout << "read throughput: " << (s->fileSize() * 1e-6 / took.count()) << " MB/s" << std::endl;
    }
    delete s;
}