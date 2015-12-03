//
// Created by feldentm on 03.11.15.
//

#include <gtest/gtest.h>
#include "../../skill/streams/streams.h"
#include "../../skill/internal/FileParser.h"
#include "../../skill/internal/SkillState.h"
#include "../../skill/internal/UnknownBasePool.h"
#include "../../skill/fieldTypes/AnnotationType.h"

using namespace skill::streams;

namespace parseTest {
    using namespace skill;
    using namespace skill::api;
    using namespace skill::internal;
    using namespace skill::restrictions;

    String date;
    struct StringKeeper : public AbstractStringKeeper {
        String date;
    };

    StringPool *initializeStrings(FileInputStream *in) {
        auto keeper = new StringKeeper;
        StringPool *pool = new StringPool(in, keeper);
        keeper->date = pool->add("date");

        date = keeper->date;
        assert(-1 == date->getID());
        return pool;
    }

    //!create a new pool in the target type system
    AbstractStoragePool *testPool(TypeID typeID,
                                  String name,
                                  AbstractStoragePool *superPool,
                                  std::set<TypeRestriction *> *restrictions) {
        if (nullptr == superPool)
            return new UnknownBasePool(typeID, name, restrictions);
        else
            return superPool->makeSubPool(typeID, name, restrictions);
    }

    //! create a new state in the target type system
    SkillFile *testMake(FileInputStream *in,
                        WriteMode mode,
                        StringPool *String,
                        fieldTypes::AnnotationType *Annotation,
                        std::vector<std::unique_ptr<AbstractStoragePool>> *types,
                        typeByName_t *typesByName,
                        std::vector<std::unique_ptr<MappedInStream>> &dataList) {
        //! TODO read field data

        // trigger allocation and instance creation
        for (auto &t : *types) {
            t.get()->allocateData();
            //if (nullptr==t->superPool)
            //  StoragePool.setNextPools(t);
        }

        return new SkillState(in, mode, String, Annotation, types, typesByName);
    }

    SkillFile *open(std::string path) {
        return skill::internal::parseFile<initializeStrings, testPool, testMake>(
                std::unique_ptr<FileInputStream>(new FileInputStream(path)), readOnly);
    }
}
using namespace parseTest;

TEST(Parser, Empty) {
    auto s = open("emptyBlocks.sf");
    // date must not have been used by now
    GTEST_ASSERT_EQ(-1, date->getID());
    ASSERT_TRUE(s);
    delete s;
}

TEST(Parser, Date) {
    try {
        auto s = open("date.sf");
        delete s;
    } catch (skill::SkillException e) {
        GTEST_FAIL() << "an exception was thrown:" << std::endl << e.message;
    }
    GTEST_SUCCEED();
}

TEST(Parser, DatePreloadUsage) {
    try {
        auto s = open("date.sf");
        // if date was used, then it must have a nonzero id now
        GTEST_ASSERT_NE(-1, date->getID());
        delete s;
    } catch (skill::SkillException e) {
        GTEST_FAIL() << "an exception was thrown:" << std::endl << e.message;
    }
    GTEST_SUCCEED();
}

TEST(Parser, Age) {
    auto s = open("age.sf");
    ASSERT_TRUE(s);
    delete s;
}

TEST(Parser, Age16) {
    auto s = open("age16.sf");
    ASSERT_TRUE(s);
    delete s;
}

TEST(Parser, LBPO) {
    auto s = open("lbpo.sf");
    ASSERT_TRUE(s);
    delete s;
}
