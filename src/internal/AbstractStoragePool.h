//
// Created by Timm Felden on 04.11.15.
//

#ifndef SKILL_CPP_COMMON_ABSTRACTSTORAGEPOOL_H
#define SKILL_CPP_COMMON_ABSTRACTSTORAGEPOOL_H

#include <vector>

#include "../common.h"
#include "FileStructure.h"
#include "FieldDeclaration.h"
#include "../fieldTypes/FieldType.h"

namespace skill {
    namespace internal {
/**
 * this class reflects all storage pool properties, that do not depend on types
 *
 * @note let us assume, that the Abstract storage pool is of type T and its base pool is of type B
 */
        class AbstractStoragePool : public fieldTypes::FieldType {
        protected:
            AbstractStoragePool(TypeID typeID, AbstractStoragePool *superPool, api::String const name)
                    : FieldType(typeID),
                      name(name),
                      superPool(superPool),
                      basePool(superPool ? superPool->basePool : this) { }

        public:

            /**
             * the name of this pool
             */
            const api::String name;

            /**
             * offset into the type array
             */
            const int poolOffset() const {
                return typeID - 32;
            }

            /**
             * in fact a Pool[? >: T,B]
             */
            AbstractStoragePool *const superPool;

            /**
             * in fact a BasePool[B]
             */
            AbstractStoragePool *const basePool;

            //! internal use only
            SKilLID cachedSize = 0;

            //! internal use only
            SKilLID staticDataInstnaces = 0;

            //! internal use only
            std::vector<Block> blocks;


            //! internal use only
            std::vector<FieldDeclaration *> dataFields;

            //!internal use only
            virtual FieldDeclaration *addField(TypeID id, FieldType *type, api::String name) = 0;
        };
    }
}


#endif //SKILL_CPP_COMMON_ABSTRACTSTORAGEPOOL_H
