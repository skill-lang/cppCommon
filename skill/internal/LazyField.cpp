//
// Created by Timm Felden on 20.11.15.
//

#include "LazyField.h"
#include "AbstractStoragePool.h"

using namespace skill;
using namespace internal;

api::Box LazyField::getR(const api::Object *i) {
    if (-1 == i->id)
        return newData[i];

    ensureIsLoaded();
    return data[i];
}

void LazyField::setR(api::Object *i, api::Box v) {
    if (-1 == i->id)
        newData[i] = v;

    ensureIsLoaded();
    data[i] = v;
}

void LazyField::read(const streams::MappedInStream *in, const Chunk *target) {
    (*parts)[target] = new skill::streams::MappedInStream(in, target->begin, target->end);
}

void LazyField::ensureIsLoaded() {
    if (!isLoaded())
        load();
}

void LazyField::load() {
    for (const auto &e : *parts) {
        const auto &target = e.first;
        auto &part = e.second;
        skill::streams::MappedInStream &in = *part;

        try {
            if (target->isSimple()) {
                for (::skill::SKilLID i = 1 + ((::skill::internal::SimpleChunk *) target)->bpo,
                             high = i + target->count; i != high; i++)
                    data[owner->getAsAnnotation(i)] = type->read(in);
            } else {
                //case bci : BulkChunk ⇒
                for (int i = 0; i < ((::skill::internal::BulkChunk *) target)->blockCount; i++) {
                    const auto &b = owner->blocks[i];
                    for (::skill::SKilLID i = 1 + b.bpo, end = i + b.dynamicCount; i != end; i++)
                        data[owner->getAsAnnotation(i)] = type->read(in);
                }
            }
        } catch (::skill::SkillException e) {
            throw ParseException(
                    in.getPosition(),
                    part->getPosition() + target->begin,
                    part->getPosition() + target->end, e.message);
        } catch (...) {
            throw ParseException(
                    in.getPosition(),
                    part->getPosition() + target->begin,
                    part->getPosition() + target->end, "unexpected foreign exception");
        }

        if (!in.eof())
            throw ParseException(
                    in.getPosition(),
                    part->getPosition() + target->begin,
                    part->getPosition() + target->end, "did not consume all bytes");
        delete part;
    }
    delete parts;
    parts = nullptr;
}

bool LazyField::check() const {
    const_cast<LazyField *>(this)->ensureIsLoaded();
    return DistributedField::check();
}

LazyField::~LazyField() {
    if (parts) {
        for (const auto &p : *parts)
            delete p.second;
        delete parts;
    }
}
