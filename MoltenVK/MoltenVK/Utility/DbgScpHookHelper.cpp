//
//  DbgScpHookHelper.cpp
//  MoltenVK
//
//  Created by lan xiang on 2025/10/25.
//  Copyright © 2025 The Brenwill Workshop Ltd. All rights reserved.
//

#include "DbgScpHookHelper.h"
#include "DebugScriptEntry.h"

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
