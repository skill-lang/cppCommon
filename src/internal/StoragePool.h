//
// Created by Timm Felden on 04.11.15.
//

#ifndef SKILL_CPP_COMMON_STORAGEPOOL_H
#define SKILL_CPP_COMMON_STORAGEPOOL_H

#include "AbstractStoragePool.h"

namespace skill {
    namespace internal {
/**
 * @author Timm Felden
 * @note maybe, we could omit B, but we will keep it, just for the sake of type level
 * verification and architectural compatibility to other implementations
 */
        template<class T, class B>
        class StoragePool : public AbstractStoragePool {

        public:
            StoragePool(TypeID typeID, AbstractStoragePool *superPool)
                    : AbstractStoragePool(typeID, superPool) { }
        };
    }
}


#endif //SKILL_CPP_COMMON_STORAGEPOOL_H
