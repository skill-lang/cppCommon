//
// Created by Timm Felden on 23.11.15.
//

#ifndef SKILL_CPP_COMMON_BUILTINFIELDTYPE_H
#define SKILL_CPP_COMMON_BUILTINFIELDTYPE_H

#include "FieldType.h"
#include "../utils.h"

namespace skill {
    using streams::InStream;
    namespace fieldTypes {

        /**
         * generic implementation of builtin types, where <T> is the base type in C++
         */
        template<typename T, TypeID id>
        class BuiltinFieldType : public FieldType {

        public:
            BuiltinFieldType() : FieldType(typeID) { }
        };

        template<typename T, TypeID id>
        class ConstantFieldType : public BuiltinFieldType<T, id> {

        public:
            ConstantFieldType(T value) : BuiltinFieldType<T, id>(), value(value) { }

            /**
             * value of the constant
             */
            const T value;

            virtual uint64_t offset(api::Box &target) const {
                return 0;
            }

            virtual void write(outstream &out, api::Box &target) const {
                // nothing to do
            }
        };

        class ConstantI8 : public ConstantFieldType<int8_t, 0> {
        public:
            ConstantI8(int8_t value) : ConstantFieldType(value) { }

            virtual api::Box read(streams::MappedInStream &in) const {
                register api::Box r;
                r.i8 = value;
                return r;
            }
        };

        class ConstantI16 : public ConstantFieldType<int16_t, 1> {
        public:

            ConstantI16(int16_t value) : ConstantFieldType(value) { }

            virtual api::Box read(streams::MappedInStream &in) const {
                register api::Box r;
                r.i16 = value;
                return r;
            }
        };

        class ConstantI32 : public ConstantFieldType<int32_t, 2> {
        public:
            ConstantI32(int32_t value) : ConstantFieldType(value) { }

            virtual api::Box read(streams::MappedInStream &in) const {
                register api::Box r;
                r.i32 = value;
                return r;
            }
        };

        class ConstantI64 : public ConstantFieldType<int64_t, 3> {
        public:
            ConstantI64(int64_t value) : ConstantFieldType(value) { }

            virtual api::Box read(streams::MappedInStream &in) const {
                register api::Box r;
                r.i64 = value;
                return r;
            }
        };

        class ConstantV64 : public ConstantFieldType<int64_t, 4> {
        public:
            ConstantV64(int64_t value) : ConstantFieldType(value) { }

            virtual api::Box read(streams::MappedInStream &in) const {
                register api::Box r;
                r.i64 = value;
                return r;
            }
        };

        /**
         * all field types that have no state whatsoever
         */
        template<typename T, TypeID id,
                api::Box Read(InStream &)/*,
                uint64_t Offset(api::Box &target),
                void Write(outstream &out, api::Box &target)*/>
        struct StatelessFieldType : BuiltinFieldType<T, id> {
            StatelessFieldType() : BuiltinFieldType<T, id>() { }

            virtual api::Box read(streams::MappedInStream &in) const {
                return Read(in);
            }
        };

        /**
         * all types with a fixed size serialization
         */
        template<typename T, TypeID id,
                api::Box Read(InStream &),
                uint64_t size>
        struct FixedSizeType : StatelessFieldType<T, id, Read> {

            FixedSizeType() { }

            virtual api::Box read(streams::MappedInStream &in) const {
                return Read(in);
            }

            virtual uint64_t offset(api::Box &target) const {
                return size;
            }

            virtual void write(outstream &out, api::Box &target) const {
                SK_TODO;
            }
        };



        //! TODO annotation

        const FixedSizeType<bool, 6, InStream::boolBox, 1> BoolType;
        const FixedSizeType<int8_t, 7, InStream::i8Box, 1> I8;
        const FixedSizeType<int16_t, 8, InStream::i16Box, 2> I16;
        const FixedSizeType<int32_t, 9, InStream::i32Box, 4> I32;
        const FixedSizeType<int64_t, 10, InStream::i64Box, 8> I64;
        const FixedSizeType<int64_t, 11, InStream::v64Box, 8> V64;
        const FixedSizeType<float, 12, InStream::f32Box, 4> F32;
        const FixedSizeType<double, 13, InStream::f64Box, 8> F64;
    }

}


#endif //SKILL_CPP_COMMON_BUILTINFIELDTYPE_H
