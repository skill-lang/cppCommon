//
// Created by Timm Felden on 28.12.15.
//

#ifndef SKILL_CPP_COMMON_DYNAMIC_DATA_ITERATOR_H
#define SKILL_CPP_COMMON_DYNAMIC_DATA_ITERATOR_H

#include <iterator>


namespace skill {
    namespace internal {
        template<class T, class B>
        class StoragePool;
    }
    using internal::StoragePool;

    namespace iterators {
        /**
         * Iterates efficiently over dynamic instances of a pool.
         *
         * First phase will iterate over all blocks of the pool.
         * The second phase will iterate over all dynamic instances of the pool.
         *
         * @author Timm Felden
         */
        template<class T, class B>
        class DynamicDataIterator :
                public std::iterator<std::input_iterator_tag, T> {

            //! current type
            TypeHierarchyIterator ts;

            SKilLID secondIndex;
            const SKilLID lastBlock;
            SKilLID index;
            SKilLID end;

        public:
            DynamicDataIterator(const StoragePool<T, B> *p)
                    : ts(p), secondIndex(0), lastBlock(p->blocks.size()),
                      index(0), end(0) {

                while (index == end && secondIndex < lastBlock) {
                    const auto &b = p->blocks[secondIndex];
                    index = b.bpo;
                    end = index + b.dynamicCount;
                    secondIndex++;
                }
                // mode switch, if there is no other block
                if (index == end && secondIndex == lastBlock) {
                    secondIndex++;
                    while (ts.hasNext()) {
                        const StoragePool<T, B> *t = (const StoragePool<T, B> *) ts.next();
                        if (t->newObjects.size() != 0) {
                            index = 0;
                            end = t->newObjects.size();
                            break;
                        }
                    }
                }
            }

            DynamicDataIterator(const DynamicDataIterator<T, B> &iter)
                    : ts(iter.ts), secondIndex(iter.secondIndex), lastBlock(iter.lastBlock),
                      index(iter.index), end(iter.end) { }

            DynamicDataIterator &operator++() {
                if (secondIndex <= lastBlock) {
                    index++;
                    if (index == end) {
                        const StoragePool<T, B> &p = (const StoragePool<T, B> &) *ts;
                        while (index == end && secondIndex < lastBlock) {
                            const auto &b = p.blocks[secondIndex];
                            index = b.bpo;
                            end = index + b.dynamicCount;
                            secondIndex++;
                        }
                        // mode switch, if there is no other block
                        if (index == end && secondIndex == lastBlock) {
                            secondIndex++;
                            while (ts.hasNext()) {
                                const StoragePool<T, B> *t = (const StoragePool<T, B> *) ts.next();
                                if (t->newObjects.size() != 0) {
                                    index = 0;
                                    end = t->newObjects.size();
                                    break;
                                }
                            }
                        }
                    }
                } else {
                    index++;
                    if (index == end) {
                        while (ts.hasNext()) {
                            const StoragePool<T, B> *t = (const StoragePool<T, B> *) ts.next();
                            if (t->newObjects.size() != 0) {
                                index = 0;
                                end = t->newObjects.size();
                                break;
                            }
                        }
                    }
                }
                return *this;
            }

            DynamicDataIterator operator++(int) {
                DynamicDataIterator tmp(*this);
                operator++();
                return tmp;
            }

            //! move to next position and return current element
            T *next() {
                if (secondIndex <= lastBlock) {
                    const StoragePool<T, B> &p = (const StoragePool<T, B> &) *ts;
                    // @note increment happens before access, because we shifted data by 1
                    index++;
                    T *r = p.data[index];
                    if (index == end) {
                        while (index == end && secondIndex < lastBlock) {
                            const auto &b = p.blocks[secondIndex];
                            index = b.bpo;
                            end = index + b.dynamicCount;
                            secondIndex++;
                        }
                        // mode switch, if there is no other block
                        if (index == end && secondIndex == lastBlock) {
                            secondIndex++;
                            while (ts.hasNext()) {
                                const StoragePool<T, B> *t = (const StoragePool<T, B> *) ts.next();
                                if (t->newObjects.size() != 0) {
                                    index = 0;
                                    end = t->newObjects.size();
                                    break;
                                }
                            }
                        }
                    }
                    return r;
                } else {
                    const StoragePool<T, B> &p = (const StoragePool<T, B> &) *ts;
                    T *r = p.newObjects[index];
                    index++;
                    if (index == end) {
                        while (ts.hasNext()) {
                            const StoragePool<T, B> *t = (const StoragePool<T, B> *) ts.next();
                            if (t->newObjects.size() != 0) {
                                index = 0;
                                end = t->newObjects.size();
                                break;
                            }
                        }
                    }
                    return r;
                }
            }

            //! @return true, iff another element can be returned
            bool hasNext() const {
                return index != end;
            }

            bool operator==(const DynamicDataIterator<T, B> &iter) const {
                return ts == iter.ts &&
                       secondIndex == iter.secondIndex &&
                       lastBlock == iter.lastBlock &&
                       index == iter.index &&
                       end == iter.end;
            }

            bool operator!=(const DynamicDataIterator<T, B> &rhs) const {
                return !(this->operator==(rhs));
            }

            T &operator*() const {
                const StoragePool<T, B> &p = (const StoragePool<T, B> &) *ts;
                // @note increment happens before access, because we shifted data by 1
                return *(index < end ? p.data[index + 1] : p.newObjects[secondIndex]);
            }

            T &operator->() const {
                const StoragePool<T, B> &p = (const StoragePool<T, B> &) *ts;
                return *(index < end ? p.data[index + 1] : p.newObjects[secondIndex]);
            }
        };
    }
}

#endif //SKILL_CPP_COMMON_DYNAMIC_DATA_ITERATOR_H
