//
// Created by Timm Felden on 06.11.15.
//

#ifndef SKILL_CPP_COMMON_STRING_H
#define SKILL_CPP_COMMON_STRING_H

#include <string>
#include "../common.h"

namespace skill {
    namespace api {

        /**
         * Managed strings, as used throughout skill files.
         *
         * @note we need our own string implementation for proper resource management
         * @note you can obtain a string from a files string access
         * @note having unique Strings in a file is necessary for our boxing strategy
         *
         * @author Timm Felden
         */
        class String : public std::string {
        private:
            SKilLID id;

            /**
             * users shall not copy strings; managed strings should be unique,
             * so that we can identify them by their pointer and maps over String* behave as
             * intended.
             */
            String(const String &c)
                    : std::string(c), id(c.id) { }

        public:
        };
    }
}


#endif //SKILL_CPP_COMMON_STRING_H
