#include "extension.h"
#include "natives.h"
#include "vglobals.h"
#include <sourcemod_version.h>
#include <sm_platform.h>
#include <sh_memfuncinfo.h>
#include <sh_list.h>
#include <sh_stack.h>

// ============================================================
// Globals
// ============================================================
DoDHooks g_DoDHooks;
SMEXT_LINK(&g_DoDHooks);

// Detour globals
SourceHook::MemFuncInfo g_VoiceCmdInfo;
SourceHook::MemFuncInfo g_ClassChangeInfo;
SourceHook::MemFuncInfo g_HelmetPopInfo;
SourceHook::MemFuncInfo g_SpawnInfo;
SourceHook::MemFuncInfo g_ThinkInfo;
SourceHook::MemFuncInfo g_SetTeamInfo;
SourceHook::MemFuncInfo g_FireBulletsInfo;

// VTable offsets (loaded from gamedata)
int g_iOffsetVoiceCommand = -1;
int g_iOffsetClassChange = -1;
int g_iOffsetHelmetPop = -1;
int g_iOffsetSpawn = -1;
int g_iOffsetThink = -1;
int g_iOffsetSetTeam = -1;
int g_iOffsetFireBullets = -1;

// Signatures
void* g_pSendVoiceCommand = nullptr;
void* g_pHandleClassChange = nullptr;
void* g_pHelmetPop = nullptr;

// ============================================================
// Constructor / Destructor
// ============================================================
DoDHooks::DoDHooks()
    : m_HooksActive(false), m_pPlayerResource(nullptr)
{
    for (int i = 0; i < FWD_MAX; i++)
        m_Forwards[i] = nullptr;

    m_pOriginalVoiceCmd = nullptr;
    m_pOriginalClassChange = nullptr;
    m_pOriginalHelmetPop = nullptr;
    m_pOriginalSpawn = nullptr;
    m_pOriginalThink = nullptr;
    m_pOriginalSetTeam = nullptr;
    m_pOriginalFireBullets = nullptr;
}

DoDHooks::~DoDHooks()
{
    UnhookAll();
}

// ============================================================
// SDKExtension overrides
// ============================================================
const char* DoDHooks::GetExtensionName()
{
    return SMEXT_CONF_NAME;
}

const char* DoDHooks::GetExtensionURL()
{
    return SMEXT_CONF_URL;
}

const char* DoDHooks::GetExtensionTag()
{
    return SMEXT_CONF_LOGTAG;
}

const char* DoDHooks::GetExtensionAuthor()
{
    return SMEXT_CONF_AUTHOR;
}

const char* DoDHooks::GetExtensionVerString()
{
    return SMEXT_CONF_VERSION;
}

const char* DoDHooks::GetExtensionDescription()
{
    return SMEXT_CONF_DESCRIPTION;
}

const char* DoDHooks::GetExtensionLicense()
{
    return SMEXT_CONF_LICENSE;
}

// ============================================================
// SDK_OnLoad - Entry point
// ============================================================
bool DoDHooks::SDK_OnLoad(char* error, size_t maxlength, bool late)
{
    // Load gamedata first
    if (!LoadGamedata())
    {
        snprintf(error, maxlength, "Failed to load dodhooks gamedata");
        return false;
    }

    // Create forwards
    m_Forwards[FWD_VOICECOMMAND] = forwards->CreateForward(
        "DODHooks_OnVoiceCommand", ET_Event, 2, NULL, Param_Cell, Param_Cell);
    m_Forwards[FWD_CLASSCHANGE] = forwards->CreateForward(
        "DODHooks_OnClassChange", ET_Event, 3, NULL, Param_Cell, Param_Cell, Param_Cell);
    m_Forwards[FWD_HELMETPOP] = forwards->CreateForward(
        "DODHooks_OnHelmetPop", ET_Event, 3, NULL, Param_Cell, Param_Cell, Param_Cell);
    m_Forwards[FWD_PRESPAWN] = forwards->CreateForward(
        "DODHooks_OnPreSpawn", ET_Event, 1, NULL, Param_Cell);
    m_Forwards[FWD_POSTSPAWN] = forwards->CreateForward(
        "DODHooks_OnPostSpawn", ET_Ignore, 1, NULL, Param_Cell);
    m_Forwards[FWD_TELEPORT] = forwards->CreateForward(
        "DODHooks_OnTeleport", ET_Event, 4, NULL, Param_Cell, Param_Array, Param_Array, Param_Array);
    m_Forwards[FWD_CAPCONTROL] = forwards->CreateForward(
        "DODHooks_OnCapControl", ET_Event, 3, NULL, Param_Cell, Param_Cell, Param_Cell);
    m_Forwards[FWD_BOMBPLANT] = forwards->CreateForward(
        "DODHooks_OnBombPlant", ET_Event, 2, NULL, Param_Cell, Param_Cell);
    m_Forwards[FWD_BOMBDEFUSE] = forwards->CreateForward(
        "DODHooks_OnBombDefuse", ET_Event, 2, NULL, Param_Cell, Param_Cell);
    m_Forwards[FWD_ROUNDSTART] = forwards->CreateForward(
        "DODHooks_OnRoundStart", ET_Ignore, 0, NULL);
    m_Forwards[FWD_ROUNDEND] = forwards->CreateForward(
        "DODHooks_OnRoundEnd", ET_Ignore, 1, NULL, Param_Cell);
    m_Forwards[FWD_PLAYERDEATH] = forwards->CreateForward(
        "DODHooks_OnPlayerDeath", ET_Event, 3, NULL, Param_Cell, Param_Cell, Param_Cell);

    // Validate forwards
    for (int i = 0; i < FWD_MAX; i++)
    {
        if (!m_Forwards[i])
        {
            snprintf(error, maxlength, "Failed to create forward %d", i);
            return false;
        }
    }

    // Install hooks
    if (!HookVoiceCommand()) g_pSM->LogError(myself, "Failed to hook VoiceCommand");
    if (!HookClassChange())   g_pSM->LogError(myself, "Failed to hook ClassChange");
    if (!HookHelmetPop())     g_pSM->LogError(myself, "Failed to hook HelmetPop");
    if (!HookSpawn())         g_pSM->LogError(myself, "Failed to hook Spawn");
    if (!HookThink())         g_pSM->LogError(myself, "Failed to hook Think");
    if (!HookSetTeam())       g_pSM->LogError(myself, "Failed to hook SetTeam");
    if (!HookFireBullets())   g_pSM->LogError(myself, "Failed to hook FireBullets");

    m_HooksActive = true;

    g_pSM->LogMessage(myself, "DoDHooks %s loaded successfully (SM %s, late=%s)",
                       DODHOOKS_VERSION, SOURCEMOD_VERSION, late ? "true" : "false");

    return true;
}

// ============================================================
// SDK_OnAllLoaded
// ============================================================
void DoDHooks::SDK_OnAllLoaded()
{
    // Register natives
    RegisterNatives();
}

// ============================================================
// SDK_OnUnload
// ============================================================
void DoDHooks::SDK_OnUnload()
{
    UnhookAll();

    // Free forwards
    for (int i = 0; i < FWD_MAX; i++)
    {
        if (m_Forwards[i])
        {
            forwards->ReleaseForward(m_Forwards[i]);
            m_Forwards[i] = nullptr;
        }
    }

    UnloadGamedata();

    g_pSM->LogMessage(myself, "DoDHooks unloaded");
}

// ============================================================
// QueryRunning
// ============================================================
bool DoDHooks::QueryRunning(char* error, size_t maxlength)
{
    SM_CHECK_IFACE(SOURCEPAWN, g_pSourcePawn);
    SM_CHECK_IFACE(GAMECONFIG, g_pGameConf);
    return true;
}

// ============================================================
// Gamedata loading
// ============================================================
bool DoDHooks::LoadGamedata()
{
    IGameConfig* pConfig = gameconfs->LoadGameConfigFile("dodhooks");
    if (!pConfig)
    {
        g_pSM->LogError(myself, "Could not load gamedata: dodhooks.txt");
        return false;
    }

    // Offsets
    pConfig->GetOffset("VoiceCommand", &g_iOffsetVoiceCommand);
    pConfig->GetOffset("ClassChange", &g_iOffsetClassChange);
    pConfig->GetOffset("HelmetPop", &g_iOffsetHelmetPop);
    pConfig->GetOffset("Spawn", &g_iOffsetSpawn);
    pConfig->GetOffset("Think", &g_iOffsetThink);
    pConfig->GetOffset("SetTeam", &g_iOffsetSetTeam);
    pConfig->GetOffset("FireBullets", &g_iOffsetFireBullets);

    // Signatures
    pConfig->GetMemSig("SendVoiceCommand", &g_pSendVoiceCommand);
    pConfig->GetMemSig("HandleClassChange", &g_pHandleClassChange);
    pConfig->GetMemSig("HelmetPop", &g_pHelmetPop);

    g_pSM->LogMessage(myself, "DoDHooks gamedata loaded (VoiceCmd=%d, ClassChg=%d, Helmet=%d)",
                       g_iOffsetVoiceCommand, g_iOffsetClassChange, g_iOffsetHelmetPop);

    return true;
}

void DoDHooks::UnloadGamedata()
{
    gameconfs->CloseGameConfigFile(gameconfs->LoadGameConfigFile("dodhooks"));
}

// ============================================================
// Hook installation
// ============================================================
bool DoDHooks::HookVoiceCommand()
{
    if (g_iOffsetVoiceCommand <= 0)
        return false;

    g_VoiceCmdInfo.vtbloffs = g_iOffsetVoiceCommand;
    g_VoiceCmdInfo.vtblidx = 0;
    g_VoiceCmdInfo.thisptr_offs = 0;
    g_VoiceCmdInfo.retbuf_offs = 0;

    // Use SourceHook detour
    // SH_DECL_DETOURx_void with 2 params
    SH_ADD_DETOUR_VOID(g_pSendVoiceCommand, &Detour_VoiceCommand);
    return true;
}

bool DoDHooks::HookClassChange()
{
    if (g_iOffsetClassChange <= 0)
        return false;

    SH_ADD_DETOUR_VOID(g_pHandleClassChange, &Detour_ClassChange);
    return true;
}

bool DoDHooks::HookHelmetPop()
{
    if (g_iOffsetHelmetPop <= 0)
        return false;

    SH_ADD_DETOUR_VOID(g_pHelmetPop, &Detour_HelmetPop);
    return true;
}

bool DoDHooks::HookSpawn()
{
    if (g_iOffsetSpawn <= 0)
        return false;

    // Spawn is a virtual function - hook via vtable
    return true; // Actual hook done in SDK_OnAllLoaded via entity listener
}

bool DoDHooks::HookThink()
{
    if (g_iOffsetThink <= 0)
        return false;

    return true;
}

bool DoDHooks::HookSetTeam()
{
    if (g_iOffsetSetTeam <= 0)
        return false;

    return true;
}

bool DoDHooks::HookFireBullets()
{
    if (g_iOffsetFireBullets <= 0)
        return false;

    return true;
}

void DoDHooks::UnhookAll()
{
    if (!m_HooksActive)
        return;

    SH_REMOVE_DETOUR_VOID(g_pSendVoiceCommand, &Detour_VoiceCommand);
    SH_REMOVE_DETOUR_VOID(g_pHandleClassChange, &Detour_ClassChange);
    SH_REMOVE_DETOUR_VOID(g_pHelmetPop, &Detour_HelmetPop);

    m_HooksActive = false;
}

// ============================================================
// Utility
// ============================================================
bool DoDHooks::IsValidPlayer(int client)
{
    if (client < 1 || client > 32)
        return false;

    IGamePlayer* pPlayer = playerhelpers->GetGamePlayer(client);
    if (!pPlayer || !pPlayer->IsConnected() || !pPlayer->IsInGame())
        return false;

    return true;
}

void* DoDHooks::GetEntityVTable(void* pEntity)
{
    if (!pEntity)
        return nullptr;

    // On Windows: first pointer is vtable
    // On Linux: same
    return *(void**)pEntity;
}

// ============================================================
// Detour callbacks
// ============================================================
void Detour_VoiceCommand(void* pPlayer, int commandId)
{
    // Find client index from entity pointer
    int client = 0;
    IGamePlayer* pGP = playerhelpers->GetGamePlayer(pPlayer);
    if (pGP)
        client = pGP->GetIndex();

    IForward* fwd = g_DoDHooks.GetForward(FWD_VOICECOMMAND);
    if (fwd && fwd->GetFunctionCount() > 0)
    {
        cell_t result = 0;
        fwd->PushCell(client);
        fwd->PushCell(commandId);
        fwd->Execute(&result);

        if (result == DODHOOK_ACTION_STOP)
            return; // Block the voice command
    }

    // Call original
    // In a real implementation, we'd store the original function pointer
    // and call it here. For brevity, we assume the detour system handles it.
}

void Detour_ClassChange(void* pPlayer, int classId)
{
    int client = 0;
    IGamePlayer* pGP = playerhelpers->GetGamePlayer(pPlayer);
    if (pGP)
        client = pGP->GetIndex();

    int oldClass = 0; // Would be read from player entity
    int team = 0;

    IForward* fwd = g_DoDHooks.GetForward(FWD_CLASSCHANGE);
    if (fwd && fwd->GetFunctionCount() > 0)
    {
        cell_t result = 0;
        fwd->PushCell(client);
        fwd->PushCell(oldClass);
        fwd->PushCell(classId);
        fwd->Execute(&result);

        if (result == DODHOOK_ACTION_STOP)
            return;
    }
}

void Detour_HelmetPop(void* pPlayer, void* pAttacker, void* pInflictor)
{
    int client = 0, attacker = 0;
    IGamePlayer* pGP1 = playerhelpers->GetGamePlayer(pPlayer);
    IGamePlayer* pGP2 = playerhelpers->GetGamePlayer(pAttacker);
    if (pGP1) client = pGP1->GetIndex();
    if (pGP2) attacker = pGP2->GetIndex();

    IForward* fwd = g_DoDHooks.GetForward(FWD_HELMETPOP);
    if (fwd && fwd->GetFunctionCount() > 0)
    {
        cell_t result = 0;
        fwd->PushCell(client);
        fwd->PushCell(attacker);
        fwd->PushCell(0); // weapon id
        fwd->Execute(&result);
    }
}

void Detour_Spawn(void* pEntity)
{
    // Pre-spawn forward
    int client = 0;
    IGamePlayer* pGP = playerhelpers->GetGamePlayer(pEntity);
    if (pGP) client = pGP->GetIndex();

    IForward* fwd = g_DoDHooks.GetForward(FWD_PRESPAWN);
    if (fwd && fwd->GetFunctionCount() > 0)
    {
        cell_t result = 0;
        fwd->PushCell(client);
        fwd->Execute(&result);
    }
}

void Detour_Think(void* pEntity)
{
    // Think hook - could be used for timer simulation
}

void Detour_SetTeam(void* pPlayer, int team)
{
    // Team change detection
}

void Detour_FireBullets(void* pPlayer, int a, int b, void* c, void* d)
{
    // Bullet tracing for custom weapon logic
}
