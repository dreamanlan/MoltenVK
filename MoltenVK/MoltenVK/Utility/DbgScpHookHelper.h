//
//  DbgScpHookHelper.h
//  MoltenVK
//
//  Created by lan xiang on 2025/10/25.
//  Copyright © 2025 The Brenwill Workshop Ltd. All rights reserved.
//
#pragma once
#include <stdint.h>
#include <iostream>
#include <string>

extern bool ReadFileToString(const std::string& path, std::string& contents);
extern bool WriteStringToFile(const std::string& path, const std::string& contents);

extern void dbgscpHookOnInitBitArray(uint64_t thisAddr, std::size_t data, std::size_t size, std::size_t capacity);
extern void dbgscpHookOnFinalizeBitArray(uint64_t thisAddr);
extern void dbgscpHookOnChangeBitArray(uint64_t thisAddr, std::size_t data, std::size_t size, std::size_t capacity, int tag, const char* func);
extern void dbgscpHookOnNewTextureViewWithPixelFormat(int viewPixFmt, int texPixFmt);
extern void dbgscpHookOnConvertToMSL(bool& modified, bool& saveToFile, bool wasConverted, void* resultInfoPtr, std::string& msl, const std::string& result, const char* debug_name, char*& replace_path_ptr);
