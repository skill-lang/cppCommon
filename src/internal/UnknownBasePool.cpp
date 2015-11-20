//
// Created by Timm Felden on 20.11.15.
//

#include "UnknownBasePool.h"
#include "LazyField.h"

using namespace skill;
using namespace internal;

FieldDeclaration *UnknownBasePool::addField(TypeID id, FieldType *type,
                                            api::String name) {
    return new internal::LazyField(type, name);
}
