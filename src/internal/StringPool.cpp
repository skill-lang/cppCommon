//
// Created by Timm Felden on 04.11.15.
//

#include "StringPool.h"

skill::internal::StringPool::StringPool() {
    // ensure existence of fake entry
    stringPositions.push_back(std::pair<long,int>(-1L, -1));
}
