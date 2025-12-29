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
#include <fstream>
#include <sstream>

// Reads entire text file into a std::string.
// Returns std::nullopt on failure.
bool ReadFileToString(const std::string& path, std::string& contents)
{
    std::ifstream ifs(path, std::ios::in | std::ios::binary);
    if (!ifs)
        return false;

    // Try fast path using file size when available.
    ifs.seekg(0, std::ios::end);
    std::streampos sz = ifs.tellg();
    if (sz > 0) {
        contents.resize(static_cast<size_t>(sz));
        ifs.seekg(0);
        ifs.read(&contents[0], contents.size());
    } else {
        // Fallback (handles empty files and streams where tellg is not usable).
        std::ostringstream ss;
        ss << ifs.rdbuf();
        contents = ss.str();
    }

    return true;
}

// Writes the entire string to a text file.
// Returns true on success, false on failure.
bool WriteStringToFile(const std::string& path, const std::string& contents)
{
    // Use binary to ensure exact byte-for-byte write; for text files this is fine
    // and avoids newline translations on some platforms.
    std::ofstream ofs(path, std::ios::out | std::ios::binary);
    if (!ofs) return false;

    ofs.write(contents.data(), static_cast<std::streamsize>(contents.size()));
    // Ensure all data has been written and no errors occurred.
    return ofs.good();
}

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
void dbgscpHookOnConvertToMSL(bool& modified, bool& saveToFile, bool wasConverted, void* resultInfoPtr, std::string& msl, const std::string& result, const char* debug_name, char*& replace_path_ptr)
{
    const char* msl_str = msl.data();
    const char* result_str = result.c_str();
    DBGSCP_HOOK_VOID("dbgscpHookOnConvertToMSL", modified, saveToFile, wasConverted, resultInfoPtr, msl_str, result_str, debug_name, replace_path_ptr);
}
