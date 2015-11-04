//
// Created by feldentm on 03.11.15.
//

#ifndef SKILL_CPP_COMMON_INSTREAM_H
#define SKILL_CPP_COMMON_INSTREAM_H

#include <cassert>
#include <cstdint>

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
            virtual ~InStream(){};

            inline uint8_t i8() {
                assert(position < end);
                return *(position++);
            }


            inline bool eof() {
                return position >= end;
            }
        };
    }
}
#endif //SKILL_CPP_COMMON_INSTREAM_H
