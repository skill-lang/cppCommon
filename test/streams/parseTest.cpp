//
// Created by feldentm on 03.11.15.
//

#include <gtest/gtest.h>
#include "../../src/streams/streams.h"
#include "../../src/api/SkillException.h"
#include "../../src/internal/FileParser.h"
#include "../../src/internal/UnknownBasePool.h"

using namespace skill::streams;

namespace parseTest {
    using namespace skill::api;
    using namespace skill::internal;

    //!create a new pool in the target type system
    AbstractStoragePool *testPool(skill::TypeID typeID,
                                  skill::api::String name,
                                  AbstractStoragePool *superPool,
                                  std::set<TypeRestriction> *restrictions) {
        return new UnknownBasePool(typeID, superPool, name);
    }

    //! create a new state in the target type system
    SkillFile *testMake(FileInputStream *in,
                        WriteMode mode,
                        StringPool *String,
                        AnnotationType *Annotation,
                        std::vector<AbstractStoragePool *> *types,
                        skill::api::typeByName_t *typesByName,
                        std::vector<MappedInStream *> &dataList) {
        return nullptr;
    }

    SkillFile *open(std::string path) {
        return skill::internal::parseFile<testPool, testMake>(
                new FileInputStream(path), readOnly);
    }
}
using namespace parseTest;

TEST(Parser, Empty) {
    auto s = open("emptyBlocks.sf");
    ASSERT_TRUE(nullptr == s);
    //delete s;
}

TEST(Parser, Date) {
    auto s = open("date.sf");
    ASSERT_TRUE(nullptr == s);
    //delete s;
}
