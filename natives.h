#ifndef _INCLUDE_DODHOOKS_NATIVES_H_
#define _INCLUDE_DODHOOKS_NATIVES_H_

#include "extension.h"

// Register all natives with SourcePawn
void RegisterNatives();

// ============================================================
// Native function declarations (19 natives)
// ============================================================

// Player class natives
cell_t Native_GetPlayerClass(IPluginContext* pContext, const cell_t* params);
cell_t Native_SetPlayerClass(IPluginContext* pContext, const cell_t* params);
cell_t Native_GetPlayerTeam(IPluginContext* pContext, const cell_t* params);
cell_t Native_SetPlayerTeam(IPluginContext* pContext, const cell_t* params);

// Weapon natives
cell_t Native_GetPlayerWeapon(IPluginContext* pContext, const cell_t* params);
cell_t Native_GivePlayerWeapon(IPluginContext* pContext, const cell_t* params);
cell_t Native_RemovePlayerWeapon(IPluginContext* pContext, const cell_t* params);

// Control point natives
cell_t Native_GetCapIndex(IPluginContext* pContext, const cell_t* params);
cell_t Native_GetCapOwner(IPluginContext* pContext, const cell_t* params);
cell_t Native_SetCapOwner(IPluginContext* pContext, const cell_t* params);
cell_t Native_GetCapProgress(IPluginContext* pContext, const cell_t* params);

// Bomb target natives
cell_t Native_GetBombTarget(IPluginContext* pContext, const cell_t* params);
cell_t Native_IsBombPlanted(IPluginContext* pContext, const cell_t* params);
cell_t Native_GetBombTimer(IPluginContext* pContext, const cell_t* params);

// Timer natives
cell_t Native_GetRoundTime(IPluginContext* pContext, const cell_t* params);
cell_t Native_SetRoundTime(IPluginContext* pContext, const cell_t* params);

// Misc natives
cell_t Native_IsPlayerSpawned(IPluginContext* pContext, const cell_t* params);
cell_t Native_GetPlatform(IPluginContext* pContext, const cell_t* params);

#endif // _INCLUDE_DODHOOKS_NATIVES_H_
