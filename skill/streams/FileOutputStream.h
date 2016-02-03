//
// Created by Timm Felden on 03.02.16.
//

#ifndef SKILL_CPP_COMMON_FILEOUTPUTSTREAM_H
#define SKILL_CPP_COMMON_FILEOUTPUTSTREAM_H

#include <string>
#include <assert.h>
#include "Stream.h"
#include "../api/String.h"

namespace skill {
    namespace streams {

        class MappedOutStream;

        /**
         * File out streams manages file; uses a buffer for its write operations.
         * Can create a map of correct size for mapped streams.
         *
         * @author Timm Felden
         */
        class FileOutputStream : public Stream {

            /**
             * the path where this stream was opened from
             */
            const std::string path;

            /**
             * the file object used for communication to the fs
             */
            FILE *const file;

            //! number of bytes written to the file; used for truncation on close
            size_t bytesWriten;

            /**
             * flush the buffer
             */
            void flush();

            inline void require(size_t i) {
                if (!has(i))
                    flush();
            }

            //! size of the backing buffer
            static constexpr size_t BUFFER_SIZE = 4096;
            //! the backing buffer (allocate plain to get rid of one pointer deref)
            uint8_t buffer[BUFFER_SIZE];

        public:

            /**
             * open the file at the target location
             *
             * @param append set to true, if content shall be appended to the file
             */
            FileOutputStream(const std::string &path, bool append);

            /**
             * close the stream
             */
            virtual ~FileOutputStream();

            /**
             * total file size
             */
            size_t fileSize() const noexcept {
                return bytesWriten;
            }

            const std::string &filePath() const noexcept {
                return path;
            }

            /**
            * Maps from current position until offset.
            *
            * @return a buffer that has exactly offset many bytes remaining
            */
            MappedOutStream *jumpAndMap(long offset);

            /**
             * Delete a mapping.
             * This method has to be called for the result of jump and map eventually.
             * At that point, no clone of the argument stream must be in use.
             * @note do NOT call this for clones!
             * @note the argument map will be deleted!
             * @note clones must be deleted by the cloner
             */
            void unmap(MappedOutStream *map);

            inline void i8(int8_t v) {
                require(1);
                *(position++) = (uint8_t) v;
            }

            inline void i16(int16_t v) {
                require(2);
                *(position++) = (uint8_t) (v >> 8);
                *(position++) = (uint8_t) v;
            }

            inline void i32(int32_t v) {
                require(4);
                *(position++) = (uint8_t) (v >> 24);
                *(position++) = (uint8_t) (v >> 16);
                *(position++) = (uint8_t) (v >> 8);
                *(position++) = (uint8_t) v;
            }

            inline void i64(int64_t v) {
                require(8);
                *(position++) = (uint8_t) (v >> 56);
                *(position++) = (uint8_t) (v >> 48);
                *(position++) = (uint8_t) (v >> 40);
                *(position++) = (uint8_t) (v >> 32);
                *(position++) = (uint8_t) (v >> 24);
                *(position++) = (uint8_t) (v >> 16);
                *(position++) = (uint8_t) (v >> 8);
                *(position++) = (uint8_t) v;
            }

            inline void v64(int64_t v) {
                require(9);

                if (0L == (v & 0xFFFFFFFFFFFFFF80L)) {
                    *(position++) = (uint8_t) (v);
                } else {
                    *(position++) = (uint8_t) ((0x80L | v));
                    if (0L == (v & 0xFFFFFFFFFFFFC000L)) {
                        *(position++) = (uint8_t) ((v >> 7));
                    } else {
                        *(position++) = (uint8_t) ((0x80L | v >> 7));
                        if (0L == (v & 0xFFFFFFFFFFE00000L)) {
                            *(position++) = (uint8_t) ((v >> 14));
                        } else {
                            *(position++) = (uint8_t) ((0x80L | v >> 14));
                            if (0L == (v & 0xFFFFFFFFF0000000L)) {
                                *(position++) = (uint8_t) ((v >> 21));
                            } else {
                                *(position++) = (uint8_t) ((0x80L | v >> 21));
                                if (0L == (v & 0xFFFFFFF800000000L)) {
                                    *(position++) = (uint8_t) ((v >> 28));
                                } else {
                                    *(position++) = (uint8_t) ((0x80L | v >> 28));
                                    if (0L == (v & 0xFFFFFC0000000000L)) {
                                        *(position++) = (uint8_t) ((v >> 35));
                                    } else {
                                        *(position++) = (uint8_t) ((0x80L | v >> 35));
                                        if (0L == (v & 0xFFFE000000000000L)) {
                                            *(position++) = (uint8_t) ((v >> 42));
                                        } else {
                                            *(position++) = (uint8_t) ((0x80L | v >> 42));
                                            if (0L == (v & 0xFF00000000000000L)) {
                                                *(position++) = (uint8_t) ((v >> 49));
                                            } else {
                                                *(position++) = (uint8_t) ((0x80L | v >> 49));
                                                *(position++) = (uint8_t) ((v >> 56));
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            inline void put(const api::string_t *const s) {
                const auto size = s->size();
                if (size >= BUFFER_SIZE) {
                    flush();
                    fwrite(s->c_str(), 1, size, file);
                    bytesWriten += size;
                } else {
                    require(size);
                    for (uint8_t c : *s)
                        *(position++) = c;
                }
            }
        };
    }
}

#endif //SKILL_CPP_COMMON_FILEOUTPUTSTREAM_H
