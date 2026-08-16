#include "natives.h"
#include <sourcemod.h>
#include <sm_platform.h>
#include <IPlayerHelpers.h>
#include <iplayerinfo.h>

// ============================================================
// Native registration table
// ============================================================
const sp_nativeinfo_t g_Natives[] =
{
    {"DODHooks_GetPlayerClass",     Native_GetPlayerClass},
    {"DODHooks_SetPlayerClass",     Native_SetPlayerClass},
    {"DODHooks_GetPlayerTeam",      Native_GetPlayerTeam},
    {"DODHooks_SetPlayerTeam",      Native_SetPlayerTeam},
    {"DODHooks_GetPlayerWeapon",    Native_GetPlayerWeapon},
    {"DODHooks_GivePlayerWeapon",   Native_GivePlayerWeapon},
    {"DODHooks_RemovePlayerWeapon", Native_RemovePlayerWeapon},
    {"DODHooks_GetCapIndex",        Native_GetCapIndex},
    {"DODHooks_GetCapOwner",        Native_GetCapOwner},
    {"DODHooks_SetCapOwner",        Native_SetCapOwner},
    {"DODHooks_GetCapProgress",     Native_GetCapProgress},
    {"DODHooks_GetBombTarget",      Native_GetBombTarget},
    {"DODHooks_IsBombPlanted",      Native_IsBombPlanted},
    {"DODHooks_GetBombTimer",       Native_GetBombTimer},
    {"DODHooks_GetRoundTime",       Native_GetRoundTime},
    {"DODHooks_SetRoundTime",       Native_SetRoundTime},
    {"DODHooks_IsPlayerSpawned",    Native_IsPlayerSpawned},
    {"DODHooks_GetPlatform",        Native_GetPlatform},
    {nullptr, nullptr}
};

void RegisterNatives()
{
    g_pSourcePawn->AddNatives(g_Natives);
}

// ============================================================
// Helper: get CBaseEntity pointer from client index
// ============================================================
void* GetEntityPtr(int client)
{
    IGamePlayer* pPlayer = playerhelpers->GetGamePlayer(client);
    if (!pPlayer || !pPlayer->IsInGame())
        return nullptr;

    edict_t* pEdict = pPlayer->GetEdict();
    if (!pEdict)
        return nullptr;

    return pEdict->GetUnknown();
}

// ============================================================
// Player class natives
// ============================================================
cell_t Native_GetPlayerClass(IPluginContext* pContext, const cell_t* params)
{
    int client = params[1];

    if (client < 1 || client > 32)
    {
        pContext->ThrowNativeError("Invalid client index %d", client);
        return -1;
    }

    IGamePlayer* pPlayer = playerhelpers->GetGamePlayer(client);
    if (!pPlayer || !pPlayer->IsInGame())
        return -1;

    IPlayerInfo* pInfo = pPlayer->GetPlayerInfo();
    if (!pInfo)
        return -1;

    // DoD:S stores class in m_iPlayerClass (offset varies by version)
    // We read it via the player info interface
    return pInfo->GetClassId();
}

cell_t Native_SetPlayerClass(IPluginContext* pContext, const cell_t* params)
{
    int client = params[1];
    int classId = params[2];

    if (client < 1 || client > 32)
    {
        pContext->ThrowNativeError("Invalid client index %d", client);
        return 0;
    }

    if (classId < 0 || classId > 5)
    {
        pContext->ThrowNativeError("Invalid class id %d (must be 0-5)", classId);
        return 0;
    }

    void* pEntity = GetEntityPtr(client);
    if (!pEntity)
        return 0;

    // In DoD:S, class change is handled by the server via the
    // "dod_changeclass" command or directly via the ClassChange detour
    // We trigger the change through the console command
    char cmd[64];
    snprintf(cmd, sizeof(cmd), "dod_changeclass %d", classId);

    // Use engine->ServerCommand or execute on client
    IGamePlayer* pPlayer = playerhelpers->GetGamePlayer(client);
    if (pPlayer)
    {
        // Send the command to the client
        engine->ClientCommand(pPlayer->GetEdict(), "dod_changeclass %d", classId);
        return 1;
    }

    return 0;
}

cell_t Native_GetPlayerTeam(IPluginContext* pContext, const cell_t* params)
{
    int client = params[1];

    if (client < 1 || client > 32)
    {
        pContext->ThrowNativeError("Invalid client index %d", client);
        return 0;
    }

    IGamePlayer* pPlayer = playerhelpers->GetGamePlayer(client);
    if (!pPlayer || !pPlayer->IsInGame())
        return 0;

    return pPlayer->GetTeamIndex();
}

cell_t Native_SetPlayerTeam(IPluginContext* pContext, const cell_t* params)
{
    int client = params[1];
    int team = params[2];

    if (client < 1 || client > 32)
    {
        pContext->ThrowNativeError("Invalid client index %d", client);
        return 0;
    }

    if (team < 1 || team > 3)
    {
        pContext->ThrowNativeError("Invalid team %d (must be 1-3)", team);
        return 0;
    }

    IGamePlayer* pPlayer = playerhelpers->GetGamePlayer(client);
    if (!pPlayer)
        return 0;

    // Change team via engine
    engine->ChangePlayerTeam(pPlayer->GetEdict(), team);
    return 1;
}

// ============================================================
// Weapon natives
// ============================================================
cell_t Native_GetPlayerWeapon(IPluginContext* pContext, const cell_t* params)
{
    int client = params[1];

    if (client < 1 || client > 32)
        return -1;

    IGamePlayer* pPlayer = playerhelpers->GetGamePlayer(client);
    if (!pPlayer || !pPlayer->IsInGame())
        return -1;

    IPlayerInfo* pInfo = pPlayer->GetPlayerInfo();
    if (!pInfo)
        return -1;

    // Get active weapon ID
    return pInfo->GetWeaponId();
}

cell_t Native_GivePlayerWeapon(IPluginContext* pContext, const cell_t* params)
{
    int client = params[1];
    int weaponId = params[2];

    if (client < 1 || client > 32)
        return 0;

    IGamePlayer* pPlayer = playerhelpers->GetGamePlayer(client);
    if (!pPlayer || !pPlayer->IsInGame())
        return 0;

    // Give weapon via command
    const char* weaponNames[] = {
        "weapon_garand", "weapon_kar98", "weapon_bar",
        "weapon_mp40", "weapon_thompson", "weapon_stg44",
        "weapon_springfield", "weapon_30cal", "weapon_mg42",
        "weapon_bazooka", "weapon_panzerfaust",
        "weapon_p38", "weapon_c96", "weapon_m1carbine"
    };

    if (weaponId < 0 || weaponId >= (int)(sizeof(weaponNames)/sizeof(weaponNames[0])))
        return 0;

    engine->ClientCommand(pPlayer->GetEdict(), "give %s", weaponNames[weaponId]);
    return 1;
}

cell_t Native_RemovePlayerWeapon(IPluginContext* pContext, const cell_t* params)
{
    int client = params[1];
    int weaponId = params[2];

    // Stub: actual implementation would find weapon entity and remove
    return 1;
}

// ============================================================
// Control point natives
// ============================================================
cell_t Native_GetCapIndex(IPluginContext* pContext, const cell_t* params)
{
    int entity = params[1];
    // Stub: read m_iPointIndex from CDoDObjectiveResource
    return 0;
}

cell_t Native_GetCapOwner(IPluginContext* pContext, const cell_t* params)
{
    int capIndex = params[1];
    // Stub: read team ownership from objective resource
    return 0;
}

cell_t Native_SetCapOwner(IPluginContext* pContext, const cell_t* params)
{
    int capIndex = params[1];
    int team = params[2];
    // Stub: set team ownership
    return 0;
}

cell_t Native_GetCapProgress(IPluginContext* pContext, const cell_t* params)
{
    int capIndex = params[1];
    // Stub: return capture progress 0-100
    return 0;
}

// ============================================================
// Bomb target natives
// ============================================================
cell_t Native_GetBombTarget(IPluginContext* pContext, const cell_t* params)
{
    // Stub: find active bomb target entity
    return 0;
}

cell_t Native_IsBombPlanted(IPluginContext* pContext, const cell_t* params)
{
    // Stub: check bomb planted state
    return 0;
}

cell_t Native_GetBombTimer(IPluginContext* pContext, const cell_t* params)
{
    // Stub: return bomb timer in seconds
    return 0;
}

// ============================================================
// Timer natives
// ============================================================
cell_t Native_GetRoundTime(IPluginContext* pContext, const cell_t* params)
{
    // Stub: read round timer
    return 0;
}

cell_t Native_SetRoundTime(IPluginContext* pContext, const cell_t* params)
{
    int seconds = params[1];
    // Stub: set round timer
    return 0;
}

// ============================================================
// Misc natives
// ============================================================
cell_t Native_IsPlayerSpawned(IPluginContext* pContext, const cell_t* params)
{
    int client = params[1];

    if (client < 1 || client > 32)
        return 0;

    IGamePlayer* pPlayer = playerhelpers->GetGamePlayer(client);
    if (!pPlayer || !pPlayer->IsInGame())
        return 0;

    // Check if player is alive and spawned
    IPlayerInfo* pInfo = pPlayer->GetPlayerInfo();
    if (!pInfo)
        return 0;

    return pInfo->IsDead() ? 0 : 1;
}

cell_t Native_GetPlatform(IPluginContext* pContext, const cell_t* params)
{
#if defined(PLATFORM_WINDOWS)
    #if defined(PLATFORM_X64)
        return 2; // Windows 64-bit
    #else
        return 0; // Windows 32-bit
    #endif
#elif defined(PLATFORM_LINUX)
    #if defined(PLATFORM_X64)
        return 3; // Linux 64-bit
    #else
        return 1; // Linux 32-bit
    #endif
#else
    return -1; // Unknown
#endif
}
