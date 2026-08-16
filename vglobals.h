#ifndef _INCLUDE_DODHOOKS_VGLOBALS_H_
#define _INCLUDE_DODHOOKS_VGLOBALS_H_

#include "extension.h"

// ============================================================
// VGlobals - Valve global variable access
// Handles 32-bit and 64-bit pointer dereferencing
// ============================================================

// Platform detection
#if defined(PLATFORM_WINDOWS)
    #define IS_WINDOWS 1
    #if defined(PLATFORM_X64)
        #define IS_64BIT 1
    #else
        #define IS_64BIT 0
    #endif
#elif defined(PLATFORM_LINUX)
    #define IS_WINDOWS 0
    #if defined(PLATFORM_X64)
        #define IS_64BIT 1
    #else
        #define IS_64BIT 0
    #endif
#else
    #define IS_WINDOWS 0
    #define IS_64BIT 0
#endif

// ============================================================
// Global variable accessors
// ============================================================

// Get global entity list
void* GetEntityList();

// Get global max entities
int GetMaxEntities();

// Get player resource entity
void* GetPlayerResource();

// Get global server time
float GetServerTime();

// Get CGlobalVars pointer
void* GetGlobalVars();

// ============================================================
// Pointer utility functions (handle 32/64 bit differences)
// ============================================================

// Read a pointer from an address (handles 32/64 bit)
inline void* ReadPointer(void* addr)
{
    if (!addr)
        return nullptr;

    if (IS_64BIT)
        return *(void**)addr;
    else
        return *(void**)addr;
}

// Read a 32-bit integer from an address
inline int ReadInt32(void* addr)
{
    if (!addr)
        return 0;
    return *(int*)addr;
}

// Read a float from an address
inline float ReadFloat(void* addr)
{
    if (!addr)
        return 0.0f;
    return *(float*)addr;
}

// Write a pointer to an address
inline void WritePointer(void* addr, void* value)
{
    if (!addr)
        return;
    *(void**)addr = value;
}

// ============================================================
// VTable helpers
// ============================================================

// Get vtable from entity pointer
inline void** GetVTable(void* pEntity)
{
    if (!pEntity)
        return nullptr;
    return *(void***)pEntity;
}

// Call virtual function by index
inline void* GetVirtualFunc(void* pEntity, int index)
{
    void** vtable = GetVTable(pEntity);
    if (!vtable || index < 0)
        return nullptr;
    return vtable[index];
}

// ============================================================
// Signature scanning helpers
// ============================================================

// Find pattern in memory range
void* FindPattern(const char* pattern, const char* mask, void* start, size_t length);

// Get module base address
void* GetModuleBase(const char* moduleName);

// ============================================================
// Globals
// ============================================================
extern void* g_pEntityList;
extern int   g_iMaxEntities;
extern void* g_pGlobalVars;

#endif // _INCLUDE_DODHOOKS_VGLOBALS_H_
