//
// Created by Timm Felden on 04.11.15.
//

#ifndef SKILL_CPP_COMMON_STRINGPOOL_H
#define SKILL_CPP_COMMON_STRINGPOOL_H

#include <set>
#include <vector>

#include "../api/StringAccess.h"

namespace skill {
    namespace internal {

        /**
         * Implementation of all string handling.
         *
         * @author Timm Felden
         */
        class StringPool : public api::StringAccess {
            // note: this is public, because most internal stuff uses this
        public:
            StringPool();

            /**
   * the set of known strings, including new strings
   *
   * @note having all strings inside of the set instead of just the new ones, we can optimize away some duplicates,
   * while not having any additional cost, because the duplicates would have to be checked upon serialization anyway.
   * Furthermore, we can unify type and field names, thus we do not have to have duplicate names laying around,
   * improving the performance of hash containers and name checks:)
   */
            std::set<std::string> knownStrings;

            /**
             * ID ⇀ (absolute offset, length)
             *
             * will be used if idMap contains a null reference
             *
             * @note there is a fake entry at ID 0
             */
            std::vector<std::pair<long, int>> stringPositions;


            /**
             * get string by ID
             *
             * @todo replace by having
             */
#if(0)
            private[internal]
                var idMap = ArrayBuffer[String](null)
                /**
                 * map used to write strings
                 */

            private
                val serializationIDs = new HashMap[String, Int]
                /**
                 * returns the string, that should be used
                 *
                 * @note if result is null, nothing will happen and null will be returned
                 */
                def add(result : String) {
                    if (result != null)
                        knownStrings.add(result)
                }

                /**
                 * search a string by id it had inside of the read file, may block if the string has not yet been read
                 */
                def get(index : SkillID) : String = {
                        if (index <= 0) null
                        else {
                            var result = idMap(index)
                            if (null == result) {
                                this.synchronized
                                {
                                    // read result
                                    val off = stringPositions(index.toInt)
                                    in.push(off.absoluteOffset)
                                    var chars = in.bytes(off.length)
                                    in.pop
                                    result = new String(chars, "UTF-8")

                                    // ensure that the string is known
                                    knownStrings.add(result)

                                    idMap(index.toInt) = result
                                    result
                                }
                            } else
                                result
                        }
                }

#endif
        };
    }
}

#endif //SKILL_CPP_COMMON_STRINGPOOL_H
