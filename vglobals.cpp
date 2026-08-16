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
        META_CONPRINTF("[DoDHooks] Warning: Failed to find CreateStandardEntities signature\n");
        return;
    }

    if (!g_pGameConf->GetOffset("g_pObjectiveResource", &iOffset) || !iOffset)
    {
        META_CONPRINTF("[DoDHooks] Warning: Failed to get g_pObjectiveResource offset\n");
        return;
    }

    // On 64-bit Windows, the instruction is RIP-relative
    // Pattern: mov [mem], rax  (48 89 04 25 <disp32>)
    // We need to find the displacement and compute the absolute address
    #if defined PLATFORM_X64
        // Skip the mov opcode to find the 32-bit displacement
        // The pattern is: 48 89 04 25 xx xx xx xx
        // disp32 is at offset 4 from pAddress
        int32 disp = *(int32 *)(pAddress + 4);
        uintptr_t next_inst = (uintptr_t)(pAddress + 8);
        g_pObjectiveResource = *(void ***)(next_inst + disp);
    #else
        g_pObjectiveResource = *(void ***)(pAddress + iOffset);
    #endif

#elif defined PLATFORM_LINUX
    if (!g_pGameConf->GetMemSig("g_pObjectiveResource", (void **)&pAddress) || !pAddress)
    {
        META_CONPRINTF("[DoDHooks] Warning: Failed to find g_pObjectiveResource signature\n");
        return;
    }

    // On Linux, the symbol directly points to the global pointer variable
    g_pObjectiveResource = (void **)pAddress;
#endif

    if (g_pObjectiveResource)
    {
        META_CONPRINTF("[DoDHooks] g_pObjectiveResource initialized successfully (value: %p)\n",
                       *g_pObjectiveResource);
    }
    else
    {
        META_CONPRINTF("[DoDHooks] Warning: g_pObjectiveResource is NULL after initialization\n");
    }
}
