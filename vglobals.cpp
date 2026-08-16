/**
 * vim: set ts=4 :
 * ======================================================
 * DODHooks - Valve Globals implementation
 * ======================================================
 *
 * GPLv2 License
 */

#include "vglobals.h"
#include "extension.h"

void *g_pEntList = NULL;
void **g_pObjectiveResource = NULL;

void InitializeValveGlobals()
{
    g_pEntList = g_pGameHelpers->GetGlobalEntityList();

    uint8 *pAddress = NULL;

#ifdef PLATFORM_WINDOWS
    int iOffset;

    if (!g_pGameConf->GetMemSig("CreateStandardEntities", (void **)&pAddress) || !pAddress)
    {
        META_CONPRINTF("DODHooks: Warning - Failed to find CreateStandardEntities signature\n");
        return;
    }

    if (!g_pGameConf->GetOffset("g_pObjectiveResource", &iOffset) || !iOffset)
    {
        META_CONPRINTF("DODHooks: Warning - Failed to find g_pObjectiveResource offset\n");
        return;
    }

    /* On 64-bit Windows, the offset in gamedata is relative to the instruction.
     * The signature search lands us at the function start; the offset tells us
     * where the global pointer is stored. */
    g_pObjectiveResource = *reinterpret_cast<void ***>(pAddress + iOffset);

#elif defined PLATFORM_LINUX
    if (!g_pGameConf->GetMemSig("g_pObjectiveResource", (void **)&pAddress) || !pAddress)
    {
        META_CONPRINTF("DODHooks: Warning - Failed to find g_pObjectiveResource signature\n");
        return;
    }

    /* On Linux, the symbol directly gives us the address of the global. */
    g_pObjectiveResource = reinterpret_cast<void **>(pAddress);
#endif

    if (g_pObjectiveResource)
    {
        META_CONPRINTF("DODHooks: g_pObjectiveResource initialized successfully\n");
    }
    else
    {
        META_CONPRINTF("DODHooks: Warning - g_pObjectiveResource is NULL after init\n");
    }
}
