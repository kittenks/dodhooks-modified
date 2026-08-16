#include "vglobals.h"
#include "extension.h"

// Global Valve pointers
void *g_pEntList = NULL;
void **g_pObjectiveResource = NULL;

void InitializeValveGlobals()
{
    g_pEntList = g_pGameHelpers->GetGlobalEntityList();

    uint8 *pAddress = NULL;

#if defined PLATFORM_WINDOWS
    int iOffset;

    if (!g_pGameConf->GetMemSig("CreateStandardEntities", (void **)&pAddress) || !pAddress)
    {
        META_CONPRINTF("[DODHooks] Warning: Failed to find CreateStandardEntities signature\n");
        return;
    }

    if (!g_pGameConf->GetOffset("g_pObjectiveResource", &iOffset) || !iOffset)
    {
        META_CONPRINTF("[DODHooks] Warning: Failed to get g_pObjectiveResource offset\n");
        return;
    }

    // On 64-bit Windows, offsets are 8-byte relative offsets
    #if defined PLATFORM_X64
        g_pObjectiveResource = *(void ***)(pAddress + iOffset);
    #else
        g_pObjectiveResource = *reinterpret_cast<void ***>(pAddress + iOffset);
    #endif

#elif defined PLATFORM_LINUX
    if (!g_pGameConf->GetMemSig("g_pObjectiveResource", (void **)&pAddress) || !pAddress)
    {
        META_CONPRINTF("[DODHooks] Warning: Failed to find g_pObjectiveResource signature\n");
        return;
    }

    #if defined PLATFORM_X64
        // On 64-bit Linux, the signature points to the GOT entry
        g_pObjectiveResource = reinterpret_cast<void **>(pAddress);
    #else
        g_pObjectiveResource = reinterpret_cast<void **>(pAddress);
    #endif
#endif

    if (g_pObjectiveResource)
    {
        META_CONPRINTF("[DODHooks] g_pObjectiveResource initialized successfully\n");
    }
    else
    {
        META_CONPRINTF("[DODHooks] Warning: g_pObjectiveResource is NULL after initialization\n");
    }
}
