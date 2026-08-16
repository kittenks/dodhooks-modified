#ifndef _INCLUDE_DODHOOKS_EXTENSION_H_
#define _INCLUDE_DODHOOKS_EXTENSION_H_

#include "smsdk_ext.h"
#include "smsdk_config.h"
#include <sourcehook/sourcehook.h>
#include <sourcehook/sh_vector.h>
#include <vstdlib/random.h>
#include <utlvector.h>

// Forward declarations
class CBaseEntity;
class CBasePlayer;
class CDODPlayer;

// ============================================================
// Detour hook types
// ============================================================

// Voice command detour
typedef void (*VoiceCommandFn)(void* pPlayer, int commandId);
// Class change detour
typedef void (*ClassChangeFn)(void* pPlayer, int classId);
// Helmet pop detour
typedef void (*HelmetPopFn)(void* pPlayer, void* pAttacker, void* pInflictor);
// Spawn detour
typedef void (*SpawnFn)(void* pEntity);
// Think detour
typedef void (*ThinkFn)(void* pEntity);
// SetTeam detour
typedef void (*SetTeamFn)(void* pPlayer, int team);
// FireBullets detour
typedef void (*FireBulletsFn)(void* pPlayer, int a, int b, void* c, void* d);

// ============================================================
// Forward declarations for forwards
// ============================================================
enum
{
    FWD_VOICECOMMAND = 0,
    FWD_CLASSCHANGE,
    FWD_HELMETPOP,
    FWD_PRESPAWN,
    FWD_POSTSPAWN,
    FWD_TELEPORT,
    FWD_CAPCONTROL,
    FWD_BOMBPLANT,
    FWD_BOMBDEFUSE,
    FWD_ROUNDSTART,
    FWD_ROUNDEND,
    FWD_PLAYERDEATH,
    FWD_MAX
};

// ============================================================
// Main extension class
// ============================================================
class DoDHooks : public SDKExtension
{
public:
    DoDHooks();
    ~DoDHooks();

    // SDKExtension overrides
    virtual bool SDK_OnLoad(char* error, size_t maxlength, bool late);
    virtual void SDK_OnUnload();
    virtual void SDK_OnAllLoaded();
    virtual bool QueryRunning(char* error, size_t maxlength);

    // SourceMod API
    virtual const char* GetExtensionName();
    virtual const char* GetExtensionURL();
    virtual const char* GetExtensionTag();
    virtual const char* GetExtensionAuthor();
    virtual const char* GetExtensionVerString();
    virtual const char* GetExtensionDescription();
    virtual const char* GetExtensionLicense();

    // Gamedata
    bool LoadGamedata();
    void UnloadGamedata();

    // Hook management
    bool HookVoiceCommand();
    bool HookClassChange();
    bool HookHelmetPop();
    bool HookSpawn();
    bool HookThink();
    bool HookSetTeam();
    bool HookFireBullets();

    void UnhookAll();

    // Forwards
    IForward* GetForward(int fwdId) { return m_Forwards[fwdId]; }

    // VTable helpers
    void* GetEntityVTable(void* pEntity);
    bool IsValidPlayer(int client);

    // Globals
    void* GetPlayerResource() { return m_pPlayerResource; }
    void SetPlayerResource(void* p) { m_pPlayerResource = p; }

private:
    IForward* m_Forwards[FWD_MAX];
    bool m_HooksActive;
    void* m_pPlayerResource;

    // Detour instances
    VoiceCommandFn m_pOriginalVoiceCmd;
    ClassChangeFn m_pOriginalClassChange;
    HelmetPopFn m_pOriginalHelmetPop;
    SpawnFn m_pOriginalSpawn;
    ThinkFn m_pOriginalThink;
    SetTeamFn m_pOriginalSetTeam;
    FireBulletsFn m_pOriginalFireBullets;
};

extern DoDHooks g_DoDHooks;

// ============================================================
// Detour callback prototypes
// ============================================================
void Detour_VoiceCommand(void* pPlayer, int commandId);
void Detour_ClassChange(void* pPlayer, int classId);
void Detour_HelmetPop(void* pPlayer, void* pAttacker, void* pInflictor);
void Detour_Spawn(void* pEntity);
void Detour_Think(void* pEntity);
void Detour_SetTeam(void* pPlayer, int team);
void Detour_FireBullets(void* pPlayer, int a, int b, void* c, void* d);

// Action return values
#define DODHOOK_ACTION_CONTINUE  0
#define DODHOOK_ACTION_CHANGED   1
#define DODHOOK_ACTION_HANDLED   2
#define DODHOOK_ACTION_STOP      3

#endif // _INCLUDE_DODHOOKS_EXTENSION_H_
