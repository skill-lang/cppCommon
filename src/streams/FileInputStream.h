//
// Created by feldentm on 03.11.15.
//

#ifndef SKILL_CPP_COMMON_FILEINPUTSTREAM_H
#define SKILL_CPP_COMMON_FILEINPUTSTREAM_H

#include <string>

#include "InStream.h"

namespace skill {
    namespace streams {

        class FileInputStream : public InStream {

        private:
            /**
             * the path where this stream was opened from
             */
            const std::string path;

            /**
             * the file object used for communication to the fs
             */
            FILE const *file;


        public:

            /**
             * open the file at the target location
             */
            FileInputStream(const std::string path);

            /**
             * close the stream
             */
            virtual ~FileInputStream();
        };
    }
}

#endif //SKILL_CPP_COMMON_FILEINPUTSTREAM_H
