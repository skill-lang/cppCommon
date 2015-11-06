//
// Created by Timm Felden on 04.11.15.
//

#ifndef SKILL_CPP_COMMON_FILEPARSER_H_H
#define SKILL_CPP_COMMON_FILEPARSER_H_H

#include <set>
#include <vector>
#include <map>

#include "../streams/FileInputStream.h"
#include "AbstractStoragePool.h"
#include "../api/SkillFile.h"
#include "../common.h"

//! TODO replace by actual implementation
typedef int TypeRestriction;
typedef int AnnotationType;
typedef int StringPool;

namespace skill {
    using namespace streams;
    using namespace api;
    namespace internal {

        template<
                //!create a new pool in the target type system
                AbstractStoragePool *newPool(TypeID typeID,
                                             const std::string *name,
                                             AbstractStoragePool *superPool,
                                             std::set<TypeRestriction> *restrictions),

                //! create a new state in the target type system
                SkillFile *makeState(FileInputStream *in,
                                     WriteMode mode,
                                     StringPool *String,
                                     AnnotationType *Annotation,
                                     std::vector<AbstractStoragePool *> *types,
                                     std::map<std::string, AbstractStoragePool *> *typesByName,
                                     std::vector<MappedInStream *> &dataList)
        >

        SkillFile *parseFile(FileInputStream *in, WriteMode mode) {
            // ERROR DETECTION
            int blockCounter = 0;
            std::vector<std::string *> seenTypes;

            // PARSE STATE
            StringPool *String = new StringPool;
            std::vector<AbstractStoragePool *> *types = new std::vector<AbstractStoragePool *>();
            std::map<std::string, AbstractStoragePool *> *typesByName = new std::map<std::string, AbstractStoragePool *>();
            AnnotationType *Annotation = new AnnotationType;
            std::vector<MappedInStream *> dataList;

            // process stream
            while (!in->eof()) {

                // string block
                try {
                    const int count = (int) in->v64();

                    if (0 != count) {
                        int last = 0, offset = 0;
                        const long position = in->getPosition() + 4 * count;
                        for (int i = count; i != 0; i--) {
                            offset = in->i32();
//                            String.stringPositions.push_back(new StringPosition(position + last, offset - last));
 //                           String.idMap.push_back(null);
                            last = offset;
                        }
                        in->jump(in->getPosition() + last);
                    }

                } catch (SkillException e) {
                    throw SkillException::ParseException(in, blockCounter, "corrupted string block", e);
                }
#if 0
                // type block
                try {
                    var typeCount = in.v64.toInt

                    // this barrier is strictly increasing inside of each block and reset to 0 at the beginning of each block
                    var
                    blockIDBarrier :
                    Int = 0;

                    // reset counters and queues
                    seenTypes.clear
                    val resizeQueue = new ArrayBuffer[StoragePool[_, _]](typeCount)

                    // number of fields to expect for that type in this block
                    val localFields = new ArrayBuffer[LFEntry](typeCount)

                    // parse type definitions
                    while (typeCount != 0) {
                        typeCount -= 1
                        val name = String.get(in.v64.toInt)

                        // check null name
                        if (null == name)
                            throw ParseException(in, blockCounter, "Corrupted file, nullptr in typename")

                        // check duplicate types
                        if (seenTypes.contains(name))
                            throw ParseException(in, blockCounter, s
                        "Duplicate definition of type $name")

                        seenTypes.add(name)

                        val count = in.v64.toInt
                        val definition = typesByName.get(name).getOrElse {

                            // type restrictions
                            var restrictionCount = in.v64.toInt
                            val rest = new HashSet[restrictions.TypeRestriction]
                            rest.sizeHint(restrictionCount)
                            while (restrictionCount != 0) {
                                restrictionCount -= 1

                                rest += ((in.v64.toInt : @switch) match {
                                        case 0 ⇒ restrictions.Unique
                                        case 1 ⇒ restrictions.Singleton
                                        case 2 ⇒ restrictions.Monotone
                                        case 3 ⇒ restrictions.Abstract
                                        case 5 ⇒ restrictions.DefaultTypeRestriction(in.v64.toInt)

                                        case i ⇒ throw new ParseException(in, blockCounter,
                                        s"Found unknown field restriction $i. Please regenerate your binding, if possible.", null)
                                })
                            }

                            // super
                            val superID = in.v64.toInt
                            val
                            superPool =
                            if (0 == superID)
                                null
                                    else
                            {
                                if (superID > types.size)
                                    throw ParseException(in, blockCounter, s
                                """Type $name refers to an ill-formed super type.
                                found:
                                $superID
                                current
                                number
                                of
                                types:
                                ${types.size}
                                """)
                                else {
                                    val r = types(superID - 1)
                                            assert(r != null)
                                    r
                                }
                            }

                            // allocate pool
                            val r = newPool(types.size + 32, name, superPool, rest)
                            types.append(r)
                            typesByName.put(name, r)
                            r
                        }

                        if (blockIDBarrier < definition.typeID)
                            blockIDBarrier = definition.typeID;
                        else
                            throw new ParseException(in, blockCounter,
                                                     s
                        "Found unordered type block. Type $name has id ${definition.typeID}, barrier was $blockIDBarrier.");

                        // in contrast to prior implementation, bpo is the position inside of data, even if there are no actual
                        // instances. We need this behavior, because that way we can cheaply calculate the number of static instances
                        val
                        lbpo = definition.basePool.cachedSize + (if (null == definition.superPool) {
                            0
                        } else if (0 != count)
                            in.v64().toInt
                        else
                            definition.superPool.blocks.last.bpo)

                        // ensure that bpo is in fact inside of the parents block
                        if (null != definition.superPool) {
                            val b = definition.superPool.blocks.last
                            if (lbpo < b.bpo || b.bpo + b.dynamicCount < b.bpo)
                                throw new ParseException(in, blockCounter,
                                                         s
                            "Found broken bpo: $lbpo not in [${b.bpo}; ${b.bpo + b.dynamicCount}[");
                        }

                        // static count and cached size are updated in the resize phase
                        // @note we assume that all dynamic instance are static instances as well, until we know for sure
                        definition.blocks.append(new Block(blockCounter, lbpo, count, count))
                        definition.staticDataInstnaces += count

                        resizeQueue.append(definition)
                        localFields.append(new LFEntry(definition, in.v64.toInt))
                    }

                    // resize pools, i.e. update cachedSize and staticCount
                    locally {
                            val ps = resizeQueue.iterator
                            while (ps.hasNext) {
                                val p = ps.next
                                val b = p.blocks.last
                                p.cachedSize += b.dynamicCount

                                if (0 != b.dynamicCount) {
                                    // calculate static count of our parent
                                    val
                                    parent :
                                    StoragePool[_, _] = p.superPool
                                    if (null != parent) {
                                        val sb = parent.blocks.last
                                        // assumed static instances, minus what static instances would be, if p were the first sub pool.
                                        val delta = sb.staticCount - (b.bpo - sb.bpo);
                                        // if positive, then we have to subtract it from the assumed static count (local and global)
                                        if (delta > 0) {
                                            sb.staticCount -= delta
                                            parent.staticDataInstnaces -= delta
                                        }
                                    }
                                }
                            }
                    }

                    // track offset information, so that we can create the block maps and jump to the next block directly after
                    // parsing field information
                    var dataEnd = 0L

                    // parse fields
                    val es = localFields.iterator
                    while (es.hasNext) {
                        val e = es.next()
                        val p = e.pool
                        var legalFieldIDBarrier = 1 + p.dataFields.size
                        val block = p.blocks.last
                        var localFieldCount = e.count
                        while (0 != localFieldCount) {
                            localFieldCount -= 1
                            val id = in.v64.toInt
                            if (id <= 0 || legalFieldIDBarrier < id)
                                throw ParseException(in, blockCounter, s
                            "Found an illegal field ID: $id")

                            var
                            endOffset :
                            Long = 0
                            if (id == legalFieldIDBarrier) {
                                // new field
                                legalFieldIDBarrier += 1
                                val fieldName = String.get(in.v64.toInt)
                                if (null == fieldName)
                                    throw ParseException(in, blockCounter, s
                                "Field ${p.name}#$id has a nullptr as name.")

                                val t = parseFieldType(in, types, String, Annotation, blockCounter)

                                // parse field restrictions
                                var fieldRestrictionCount = in.v64.toInt
                                val rest = new HashSet[restrictions.FieldRestriction]
                                rest.sizeHint(fieldRestrictionCount)
                                while (fieldRestrictionCount != 0) {
                                    fieldRestrictionCount -= 1

                                    rest += ((in.v64.toInt : @switch) match {
                                            case 0 ⇒ restrictions.NonNull.theNonNull
                                            case 1 ⇒ restrictions.DefaultRestriction(t.read(in))
                                            case 3 ⇒ t match {
                                                case I8  ⇒ restrictions.Range(in.i8, in.i8)
                                                case I16 ⇒ restrictions.Range(in.i16, in.i16)
                                                case I32 ⇒ restrictions.Range(in.i32, in.i32)
                                                case I64 ⇒ restrictions.Range(in.i64, in.i64)
                                                case V64 ⇒ restrictions.Range(in.v64, in.v64)
                                                case F32 ⇒ restrictions.Range(in.f32, in.f32)
                                                case F64 ⇒ restrictions.Range(in.f64, in.f64)
                                                case t   ⇒ throw new ParseException(in, blockCounter, s
                                                "Type $t can not be range restricted!", null)
                                            }
                                            case 5 ⇒ restrictions.Coding(String.get(in.v64.toInt))
                                            case 7 ⇒ restrictions.ConstantLengthPointer
                                            case 9 ⇒ restrictions.OneOf((0 until in.v64.toInt).map(i ⇒
                                            parseFieldType(in, types, String, Annotation, blockCounter) match {
                                                case t :
                                                    StoragePool[_, _] ⇒ t.getInstanceClass
                                                case t ⇒ throw new ParseException(in, blockCounter,
                                                                                  s
                                                "Found a one of restrictions that tries to restrict to non user type $t.", null)
                                            }).toArray
                                            )
                                            case i ⇒ throw new ParseException(in, blockCounter,
                                            s"Found unknown field restriction $i. Please regenerate your binding, if possible.", null)
                                    })
                                }
                                endOffset = in.v64

                                val f = p.addField(id, t, fieldName, rest)
                                f.addChunk(new BulkChunk(dataEnd, endOffset, p.cachedSize, p.blocks.size))
                            } else {
                                // known field
                                endOffset = in.v64
                                p.dataFields(id - 1).addChunk(
                                        new SimpleChunk(dataEnd, endOffset, block.dynamicCount, block.bpo))
                            }
                            dataEnd = endOffset
                        }
                    }

                    // jump over data and continue in the next block
                    dataList.append(in.jumpAndMap(dataEnd.toInt))
                } catch {
                    case e :
                        SkillException ⇒ throw e
                    case e :
                        Exception      ⇒ throw ParseException(in, blockCounter, "unexpected foreign exception", e)
                }

                blockCounter += 1
                seenTypes.clear()
            }
#endif
                // note there still isn't a single instance
                return makeState(in, mode, String, Annotation, types, typesByName, dataList);
            }
        }
    }
}

#endif //SKILL_CPP_COMMON_FILEPARSER_H_H
