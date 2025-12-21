//
//  DbgScpHookHelper.cpp
//  MoltenVK
//
//  Created by lan xiang on 2025/10/25.
//  Copyright © 2025 The Brenwill Workshop Ltd. All rights reserved.
//

#include "DbgScpHookHelper.h"
#include "DebugScriptEntry.h"
#include "DbgScpHook.h"

void dbgscpInit()
{
    const char* s_DbgScpPath = "/Users/dreaman/Documents/GitHub/myuzu/tools/dbg_scp_compiler/moltenvk";
    LoadDbgScp(s_DbgScpPath, s_DbgScpPath);
}

void dbgscpHookOnInitBitArray(uint64_t thisAddr, std::size_t data, std::size_t size, std::size_t capacity)
{
    DBGSCP_HOOK_VOID("dbgscpHookOnInitBitArray", thisAddr, data, size, capacity);
}
void dbgscpHookOnFinalizeBitArray(uint64_t thisAddr)
{
    DBGSCP_HOOK_VOID("dbgscpHookOnFinalizeBitArray", thisAddr);
}
void dbgscpHookOnChangeBitArray(uint64_t thisAddr, std::size_t data, std::size_t size, std::size_t capacity, int tag, const char* func)
{
    DBGSCP_HOOK_VOID("dbgscpHookOnChangeBitArray", thisAddr, data, size, capacity, tag, func);
}
void dbgscpHookOnNewTextureViewWithPixelFormat(int viewPixFmt, int texPixFmt)
{
    DBGSCP_HOOK_VOID("dbgscpHookOnNewTextureViewWithPixelFormat", viewPixFmt, texPixFmt);
}
void dbgscpHookOnConvertToMSL(bool& modified, bool wasConverted, void* resultInfoPtr, std::string& msl, const std::string& result, const char* spirv, size_t spirv_size, const char* debug_name)
{
    char* msl_str = msl.data();
    const char* result_str = result.c_str();
    DBGSCP_HOOK_VOID("dbgscpHookOnConvertToMSL", modified, wasConverted, resultInfoPtr, msl_str, result_str, spirv, spirv_size, debug_name);
    if (modified && msl_str != msl.data()) {
        msl = msl_str;
    }
}
