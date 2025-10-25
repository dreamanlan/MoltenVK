//
//  DbgScpHookHelper.h
//  MoltenVK
//
//  Created by lan xiang on 2025/10/25.
//  Copyright © 2025 The Brenwill Workshop Ltd. All rights reserved.
//
#pragma once

#include "DbgScpHook.h"

extern void dbgscpHookOnInitBitArray(uint64_t thisAddr, std::size_t data, std::size_t size, std::size_t capacity);
extern void dbgscpHookOnFinalizeBitArray(uint64_t thisAddr);
extern void dbgscpHookOnChangeBitArray(uint64_t thisAddr, std::size_t data, std::size_t size, std::size_t capacity, int tag, const char* func);
