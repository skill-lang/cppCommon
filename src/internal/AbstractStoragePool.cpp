//
// Created by Timm Felden on 04.11.15.
//

#include "AbstractStoragePool.h"

using namespace skill;

skill::internal::AbstractStoragePool::AbstractStoragePool(TypeID typeID, AbstractStoragePool *superPool,
                                                          api::String const name,
                                                          std::set<int> *restrictions)
        : FieldType(typeID), restrictions(restrictions),
          name(name),
          superPool(superPool),
          basePool(superPool ? superPool->basePool : this) {
    if (superPool)
        superPool->subPools.push_back(this);
}

SKilLID skill::internal::AbstractStoragePool::newDynamicInstancesSize() const {
    SKilLID r = newObjectsSize();
    for (auto sub : subPools)
        r += sub->newDynamicInstancesSize();

    return r;
}

internal::AbstractStoragePool::~AbstractStoragePool() {
    delete restrictions;

    for(auto f : dataFields)
        delete f;
}
