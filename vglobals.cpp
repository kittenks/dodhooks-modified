#include "vglobals.h"
#include <sourcemod_version.h>
#include <ISmmPlugin.h>
#include <sh_memfuncinfo.h>
#include <sh_string.h>
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ============================================================
// Globals
// ============================================================
void* g_pEntityList = nullptr;
int   g_iMaxEntities = 0;
void* g_pGlobalVars = nullptr;

// ============================================================
// Get module base address
// ============================================================
void* GetModuleBase(const char* moduleName)
{
#if IS_WINDOWS
    HMODULE hModule = GetModuleHandleA(moduleName);
    return (void*)hModule;
#else
    // Linux: read from /proc/self/maps
    FILE* fp = fopen("/proc/self/maps", "r");
    if (!fp)
        return nullptr;

    char line[512];
    void* base = nullptr;
    char perm[8];
    unsigned long long start, end;
    char path[256];

    while (fgets(line, sizeof(line), fp))
    {
        // Format: start-end perms offset dev inode path
        int n = sscanf(line, "%llx-%llx %s %*s %*s %*d %s", &start, &end, perm, path);
        if (n >= 4)
        {
            // Check if path ends with moduleName
            size_t plen = strlen(path);
            size_t mlen = strlen(moduleName);
            if (plen >= mlen && strcmp(path + plen - mlen, moduleName) == 0)
            {
                base = (void*)start;
                break;
            }
        }
    }
    fclose(fp);
    return base;
#endif
}

// ============================================================
// Find pattern in memory
// ============================================================
void* FindPattern(const char* pattern, const char* mask, void* start, size_t length)
{
    if (!start || length == 0)
        return nullptr;

    unsigned char* pStart = (unsigned char*)start;
    size_t patLen = strlen(mask);

    for (size_t i = 0; i <= length - patLen; i++)
    {
        bool found = true;
        for (size_t j = 0; j < patLen; j++)
        {
            if (mask[j] == 'x' && pStart[i + j] != (unsigned char)pattern[j])
            {
                found = false;
                break;
            }
        }
        if (found)
            return (void*)(pStart + i);
    }
    return nullptr;
}

// ============================================================
// GetEntityList - Read CBaseEntity list from globals
// ============================================================
void* GetEntityList()
{
    if (g_pEntityList)
        return g_pEntityList;

    // The entity list is typically at a fixed offset in g_pGlobalVars
    // g_pGlobalVars->pEntityList is at offset 0x28 (64-bit) or 0x14 (32-bit)
    if (g_pGlobalVars)
    {
#if IS_64BIT
        g_pEntityList = ReadPointer((char*)g_pGlobalVars + 0x28);
#else
        g_pEntityList = ReadPointer((char*)g_pGlobalVars + 0x14);
#endif
    }

    return g_pEntityList;
}

// ============================================================
// GetMaxEntities
// ============================================================
int GetMaxEntities()
{
    if (g_iMaxEntities > 0)
        return g_iMaxEntities;

    // Default for Source engine
    g_iMaxEntities = 2048;

    // Try to read from globals
    if (g_pGlobalVars)
    {
#if IS_64BIT
        int* pMax = (int*)((char*)g_pGlobalVars + 0x30);
#else
        int* pMax = (int*)((char*)g_pGlobalVars + 0x18);
#endif
        if (pMax && *pMax > 0 && *pMax < 8192)
            g_iMaxEntities = *pMax;
    }

    return g_iMaxEntities;
}

// ============================================================
// GetPlayerResource
// ============================================================
void* GetPlayerResource()
{
    // Player resource entity is typically at a known offset
    // or found by searching for CDoDPlayerResource
    if (g_DoDHooks.GetPlayerResource())
        return g_DoDHooks.GetPlayerResource();

    return nullptr;
}

// ============================================================
// GetServerTime
// ============================================================
float GetServerTime()
{
    if (g_pGlobalVars)
    {
#if IS_64BIT
        return ReadFloat((char*)g_pGlobalVars + 0x40);
#else
        return ReadFloat((char*)g_pGlobalVars + 0x20);
#endif
    }
    return 0.0f;
}

// ============================================================
// GetGlobalVars
// ============================================================
void* GetGlobalVars()
{
    return g_pGlobalVars;
}

// ============================================================
// Windows x64 specific: RIP-relative address resolution
// ============================================================
#if IS_WINDOWS && IS_64BIT
// Resolve RIP-relative displacement to absolute address
void* ResolveRIPRelative(void* instructionAddr, int displacementOffset, int displacementSize)
{
    if (!instructionAddr)
        return nullptr;

    // RIP-relative: target = next_instruction + displacement
    unsigned char* pInstr = (unsigned char*)instructionAddr;
    int disp = 0;

    if (displacementSize == 4)
    {
        disp = *(int*)(pInstr + displacementOffset);
    }
    else if (displacementSize == 1)
    {
        disp = *(signed char*)(pInstr + displacementOffset);
    }

    // Next instruction address
    void* nextInstr = (char*)instructionAddr + displacementOffset + displacementSize;
    return (char*)nextInstr + disp;
}
#endif

// ============================================================
// Linux x64 specific: GOT pointer resolution
// ============================================================
#if !IS_WINDOWS && IS_64BIT
// On Linux x64, global variables are accessed via the GOT
// The instruction sequence is typically:
//   mov rax, [rip+offset]  (load GOT entry)
//   mov rax, [rax]         (dereference)
void* ResolveGOTPointer(void* movInstr, int dispOffset)
{
    if (!movInstr)
        return nullptr;

    // Read displacement
    int disp = *(int*)((char*)movInstr + dispOffset);

    // GOT entry = next_instruction + displacement
    void** gotEntry = (void**)((char*)movInstr + dispOffset + 4 + disp);

    // Dereference GOT entry to get actual address
    return *gotEntry;
}
#endif
