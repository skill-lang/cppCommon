//
// Created by Timm Felden on 20.11.15.
//

#ifndef SKILL_CPP_COMMON_UNKNOWNBASEPOOL_H
#define SKILL_CPP_COMMON_UNKNOWNBASEPOOL_H

#include "AbstractStoragePool.h"

namespace skill {
    namespace internal {
        /**
         * This class represents unknown base types
         *
         * @author Timm Felden
         */
        class UnknownBasePool : public internal::AbstractStoragePool {

        public:
            UnknownBasePool(TypeID typeID, AbstractStoragePool *superPool, const api::string_t *name)
                    : AbstractStoragePool(typeID, superPool, name) { }

            virtual FieldDeclaration *addField(TypeID id, const FieldType *type, api::String name);
        };
    }
}


#endif //SKILL_CPP_COMMON_UNKNOWNBASEPOOL_H
