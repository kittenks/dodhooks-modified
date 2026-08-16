#ifndef _INCLUDE_SOURCEMOD_EXTENSION_PROPER_H_
#define _INCLUDE_SOURCEMOD_EXTENSION_PROPER_H_

#include "smsdk_ext.h"
#include "sm_datatable.h"
#include "compat_wrappers.h"

// CDetour is provided by the bintools extension
// We declare the interface we need
class CDetour
{
public:
    virtual ~CDetour() {}
    virtual bool EnableDetour() = 0;
    virtual bool DisableDetour() = 0;
    virtual void Destroy() = 0;
    virtual bool IsEnabled() = 0;
};

// Detour declaration macros (AMBuild 2.2 API)
#define DETOUR_DECL_MEMBER0(name, rettype) \
    rettype detour_##name()

#define DETOUR_DECL_MEMBER1(name, rettype, t1, n1) \
    rettype detour_##name(t1 n1)

#define DETOUR_DECL_MEMBER2(name, rettype, t1, n1, t2, n2) \
    rettype detour_##name(t1 n1, t2 n2)

#define DETOUR_MEMBER_CALL(name) detour_##name

#define DETOUR_CREATE_MEMBER(detour, name, gamedata) \
    CDetourManager::CreateDetour(g_pGameConf, (void*)&detour_##name, gamedata)

#include "vglobals.h"
#include "natives.h"

// Macro definitions for easier access
#define g_pGameHelpers gamehelpers
#define g_pEngine engine
#define g_pGameConfs gameconfs

// External declarations
extern IGameConfig *g_pGameConf;
extern CGlobalVars *g_pGlobals;
extern IBinTools *g_pBinTools;
extern ISDKTools *g_pSDKTools;
extern IServerGameEnts *g_pGameEnts;
extern CSharedEdictChangeInfo *g_pSharedChangeInfo;
extern INetworkStringTableContainer *netstringtables;

extern uint32 g_iOffset_PlayerClass;
extern uint32 g_iOffset_DesiredPlayerClass;
extern uint32 g_iOffset_NumControlPoints;
extern uint32 g_iOffset_AlliesIcons;
extern uint32 g_iOffset_AxisIcons;
extern uint32 g_iOffset_NeutralIcons;
extern uint32 g_iOffset_TimerCapIcons;
extern uint32 g_iOffset_BombedIcons;
extern uint32 g_iOffset_CPIsVisible;
extern uint32 g_iOffset_TimerPaused;
extern uint32 g_iOffset_TimeRemaining;
extern uint32 g_iOffset_TimerEndTime;

extern void *g_pEntList;
extern void **g_pObjectiveResource;

#define DOD_MAXPLAYERS 33

// Detour helper macros
#define CREATE_DETOUR(detour, name, gamedata) \
    do { \
        detour = DETOUR_CREATE_MEMBER(detour, name, gamedata); \
        if (detour != NULL) \
        { \
            detour->EnableDetour(); \
        } \
        else \
        { \
            if (szConfigError[0]) \
            { \
                snprintf(error, maxlength, "Fatal Error: Unable to load detour - %s: %s", gamedata, szConfigError); \
            } \
            else \
            { \
                snprintf(error, maxlength, "Fatal Error: Unable to load detour - %s", gamedata); \
            } \
            return false; \
        } \
    } while (0)

#define REMOVE_DETOUR(detour) \
    do { \
        if (detour != NULL) \
        { \
            detour->Destroy(); \
            detour = NULL; \
        } \
    } while (0)

/**
 * @brief Helper to get send prop offset
 */
inline uint32 GetSendPropOffset(const char *szNetClass, const char *szPropName)
{
    sm_sendprop_info_t SendPropInfo;

    if (!g_pGameHelpers->FindSendPropInfo(szNetClass, szPropName, &SendPropInfo))
    {
        META_CONPRINTF("Fatal Error: Unable to get offset: %s::%s!\n", szNetClass, szPropName);
        return -1;
    }

    return SendPropInfo.actual_offset;
}

/**
 * @brief Main extension class
 */
class CDODHooks : public SDKExtension
{
public:
    void OnSetCommandClient(int client);

    virtual bool SDK_OnLoad(char *error, size_t maxlength, bool late) override;
    virtual void SDK_OnUnload() override;
    virtual void SDK_OnAllLoaded() override;
    virtual bool QueryRunning(char *error, size_t maxlength) override;
    virtual void NotifyInterfaceDrop(SMInterface *pInterface) override;

#if defined SMEXT_CONF_METAMOD
    virtual bool SDK_OnMetamodLoad(SourceMM::ISmmAPI *ismm, char *error, size_t maxlen, bool late) override;
    virtual bool SDK_OnMetamodUnload(char *error, size_t maxlength) override;
    virtual bool SDK_OnMetamodPauseChange(bool paused, char *error, size_t maxlength) override;
#endif
};

#endif // _INCLUDE_SOURCEMOD_EXTENSION_PROPER_H_
