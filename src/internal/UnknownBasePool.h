//
// Created by Timm Felden on 20.11.15.
//

#ifndef SKILL_CPP_COMMON_UNKNOWNBASEPOOL_H
#define SKILL_CPP_COMMON_UNKNOWNBASEPOOL_H

#include "AbstractStoragePool.h"
#include "StoragePool.h"
#include "UnknownObject.h"
#include "BasePool.h"

namespace skill {
    namespace internal {
        /**
         * This class represents unknown base types
         *
         * @author Timm Felden
         */
        class UnknownBasePool : public BasePool<UnknownObject> {

        public:
            UnknownBasePool(TypeID typeID, const api::string_t *name)
                    : BasePool<UnknownObject>(typeID, name) { }

            virtual FieldDeclaration *addField(TypeID id, const FieldType *type,
                                               api::String name);

            virtual void allocateInstances() {
                for (const auto& b : blocks) {
                    SKilLID i = b.bpo + 1;
                    const auto last = i + b.staticCount;
                    for(;i < last;i++)
                        data[i] = new UnknownObject(i + 1, name);
                }
            }
        };
    }
}


#endif //SKILL_CPP_COMMON_UNKNOWNBASEPOOL_H
