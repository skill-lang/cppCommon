//
// Created by Timm Felden on 03.11.15.
//

#ifndef SKILL_CPP_COMMON_SKILLEXCEPTION_H
#define SKILL_CPP_COMMON_SKILLEXCEPTION_H

#include <string>
#include <sstream>
#include "../streams/InStream.h"
#include "../streams/FileInputStream.h"

namespace skill {

/**
 * top level class of everything, that is thrown by SKilL implementations
 */
    class SkillException {
    public:
        /**
         * the message related to the issue
         *
         * @note skill will not recover from exceptions internally
         */
        const std::string message;

        SkillException(std::string message);

        /**
         * creates a parse exception
         */
        static SkillException ParseException(streams::FileInputStream *stream, int blockCount, std::string msg) {
            std::stringstream message;
            message << "ParseException in file" << stream->getPath() << "\n Position" << stream->getPosition()
            << "\n reason: " << msg << std::endl;
            return SkillException(message.str());
        }

        static SkillException ParseException(streams::InStream *stream, int blockCount, std::string msg) {
            std::stringstream message;
            message << "ParseException in mapped stream.\n Position" << stream->getPosition() << "\n reason: "
            << msg << std::endl;
            return SkillException(message.str());
        }
    };

}
#endif //SKILL_CPP_COMMON_SKILLEXCEPTION_H
