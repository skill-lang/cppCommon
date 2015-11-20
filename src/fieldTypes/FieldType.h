//
// Created by Timm Felden on 20.11.15.
//

#ifndef SKILL_CPP_COMMON_FIELDTYPE_H
#define SKILL_CPP_COMMON_FIELDTYPE_H

#include "../common.h"

namespace skill {
    /**
     * this namespace contains field type information used by skill
     */
    namespace fieldTypes {

        /**
         * Field types as used in reflective access.
         *  @author Timm Felden
         *  @note field types have a mental type T that represents the base type of the field
         */
        class FieldType {

        public:
            FieldType(const TypeID typeID) : typeID(typeID) { }

            //! the id of this type
            const TypeID typeID;
        };
    }
}


#endif //SKILL_CPP_COMMON_FIELDTYPE_H
