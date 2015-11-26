//
// Created by feldentm on 03.11.15.
//

#include <gtest/gtest.h>
#include "../../src/streams/streams.h"
#include "../../src/api/SkillException.h"
#include "../../src/internal/FileParser.h"
#include "../../src/internal/SkillState.h"
#include "../../src/internal/UnknownBasePool.h"

using namespace skill::streams;

namespace parseTest {
    using namespace skill::api;
    using namespace skill::internal;
    using namespace skill::restrictions;

    //!create a new pool in the target type system
    AbstractStoragePool *testPool(skill::TypeID typeID,
                                  skill::api::String name,
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
                        AnnotationType *Annotation,
                        std::vector<AbstractStoragePool *> *types,
                        skill::api::typeByName_t *typesByName,
                        std::vector<MappedInStream *> &dataList) {
        //! TODO read field data
        for (auto map : dataList)
            delete map;

        // trigger allocation and instance creation
        for (auto t : *types) {
            t->allocateData();
            //if (nullptr==t->superPool)
            //  StoragePool.setNextPools(t);
        }

        return new SkillState(in, mode, String, Annotation, types, typesByName);
    }

    SkillFile *open(std::string path) {
        return skill::internal::parseFile<testPool, testMake>(
                new FileInputStream(path), readOnly);
    }
}
using namespace parseTest;

TEST(Parser, Empty) {
    auto s = open("emptyBlocks.sf");
    ASSERT_TRUE(s);
    delete s;
}

TEST(Parser, Date) {
    auto s = open("date.sf");
    ASSERT_TRUE(s);
    delete s;
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
