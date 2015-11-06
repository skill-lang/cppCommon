//
// Created by Timm Felden on 03.11.15.
//

#ifndef SKILL_CPP_COMMON_SKILLEXCEPTION_H
#define SKILL_CPP_COMMON_SKILLEXCEPTION_H

#include <string>
#include "../streams/InStream.h"

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
        static SkillException ParseException(streams::InStream* stream, int blockCount, std::string msg, SkillException& cause){

        }
    };
    
}
#endif //SKILL_CPP_COMMON_SKILLEXCEPTION_H
