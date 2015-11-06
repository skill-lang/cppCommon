//
// Created by feldentm on 03.11.15.
//

#ifndef SKILL_CPP_COMMON_INSTREAM_H
#define SKILL_CPP_COMMON_INSTREAM_H

#include <string>
#include <cassert>
#include <cstdint>
#include <stdint-gcc.h>

namespace skill {
    namespace streams {

        /**
        * Abstract input streams.
        *
        * @author Timm Felden
        */
        class InStream {
        protected:

            /**
             * base pointer of the stream.
             * We keep the base pointer, because it is required for unmap and sane error reporting.
             */
            const void *base;

            /**
             * position inside of the stream
             */
            uint8_t *position;
            /**
             * end pointer of the stream. The stream is done, if position reached end.
             */
            const void *end;

        public:

            /**
             * Proper destruction happens in child destructors
             */
            virtual ~InStream() { };

            inline uint8_t i8() {
                assert(position < end);
                return *(position++);
            }

            inline uint16_t i16() {
                assert(position + 1 < end);
                register uint16_t r = *(position++) << 8;
                r |= *(position++);
                return r;
            }

            inline uint32_t i32() {
                assert(position + 3 < end);
                register uint32_t r = *(position++) << 24;
                r |= *(position++) << 16;
                r |= *(position++) << 8;
                r |= *(position++);
                return r;
            }

            inline uint64_t i64() {
                assert(position + 7 < end);
                register uint64_t r = ((uint64_t) *(position++)) << 56;
                r |= ((uint64_t) *(position++)) << 48;
                r |= ((uint64_t) *(position++)) << 40;
                r |= ((uint64_t) *(position++)) << 32;
                r |= *(position++) << 24;
                r |= *(position++) << 16;
                r |= *(position++) << 8;
                r |= *(position++);
                return r;
            }

            inline uint64_t v64() {
                register uint64_t r, rval;

                if (0 != ((rval = i8()) & 0x80)) {
                    rval = (rval & 0x7f) | (((r = i8()) & 0x7f) << 7);

                    if (0 != (r & 0x80)) {
                        rval |= ((r = i8()) & 0x7f) << 14;

                        if (0 != (r & 0x80)) {
                            rval |= ((r = i8()) & 0x7f) << 21;

                            if (0 != (r & 0x80)) {
                                rval |= ((uint64_t) (r = i8()) & 0x7f) << 28;

                                if (0 != (r & 0x80)) {
                                    rval |= ((uint64_t) (r = i8()) & 0x7f) << 35;

                                    if (0 != (r & 0x80)) {
                                        rval |= ((uint64_t) (r = i8()) & 0x7f) << 42;

                                        if (0 != (r & 0x80)) {
                                            rval |= ((uint64_t) (r = i8()) & 0x7f) << 49;

                                            if (0 != (r & 0x80)) {
                                                rval |= (((uint64_t) i8()) << 56);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                return rval;
            }

            inline float f32() {
                register union {
                    uint32_t i;
                    float f;
                } result;
                result.i = i32();
                return result.f;
            }

            inline double f64() {
                register union {
                    uint64_t i;
                    double f;
                } result;
                result.i = i64();
                return result.f;
            }

            bool boolean() {
                return *(position++) != 0;
            }

            /**
             * create a string from the stream
             * @note the caller owns the string!
             */
            std::string *string(int length) {
                assert(position + length <= end);
                auto rval = new std::string((const char *) position, length);
                position = position + length;
                return rval;
            }

            /**
             * the position of this stream inside of its bounds
             */
            long getPosition() const {
                return (long) position - (long) base;
            }

            inline bool eof() const {
                return position >= end;
            }
        };
    }
}
#endif //SKILL_CPP_COMMON_INSTREAM_H
