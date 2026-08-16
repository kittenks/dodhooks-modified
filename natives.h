#ifndef _INCLUDE_SOURCEMOD_EXTENSION_NATIVES_H_
#define _INCLUDE_SOURCEMOD_EXTENSION_NATIVES_H_

#include "extension.h"

// Include file for DODHooks natives
// This file is included by extension.cpp and natives.cpp

// Maximum control points
#define MAX_CONTROL_POINTS 8

// Forward declarations for native functions
cell_t Native_GetPlayerClass(IPluginContext *pContext, const cell_t *Params);
cell_t Native_SetPlayerClass(IPluginContext *pContext, const cell_t *Params);
cell_t Native_GetDesiredPlayerClass(IPluginContext *pContext, const cell_t *Params);
cell_t Native_SetDesiredPlayerClass(IPluginContext *pContext, const cell_t *Params);
cell_t Native_PopHelmet(IPluginContext *pContext, const cell_t *Params);
cell_t Native_SetNumControlPoints(IPluginContext *pContext, const cell_t *Params);
cell_t Native_PrecacheCPIcon(IPluginContext *pContext, const cell_t *Params);
cell_t Native_SetCPIcons(IPluginContext *pContext, const cell_t *Params);
cell_t Native_SetCPVisible(IPluginContext *pContext, const cell_t *Params);
cell_t Native_PauseTimer(IPluginContext *pContext, const cell_t *Params);
cell_t Native_ResumeTimer(IPluginContext *pContext, const cell_t *Params);
cell_t Native_SetTimeRemaining(IPluginContext *pContext, const cell_t *Params);
cell_t Native_GetTimeRemaining(IPluginContext *pContext, const cell_t *Params);
cell_t Native_RespawnPlayer(IPluginContext *pContext, const cell_t *Params);
cell_t Native_AddWaveTime(IPluginContext *pContext, const cell_t *Params);
cell_t Native_SetWinningTeam(IPluginContext *pContext, const cell_t *Params);
cell_t Native_SetRoundState(IPluginContext *pContext, const cell_t *Params);
cell_t Native_SetPlayerState(IPluginContext *pContext, const cell_t *Params);
cell_t Native_SetBombTargetState(IPluginContext *pContext, const cell_t *Params);

// Native function table
static const sp_nativeinfo_t g_Natives[] =
{
    {"DOD_GetPlayerClass",         Native_GetPlayerClass},
    {"DOD_SetPlayerClass",         Native_SetPlayerClass},
    {"DOD_GetDesiredPlayerClass",  Native_GetDesiredPlayerClass},
    {"DOD_SetDesiredPlayerClass",  Native_SetDesiredPlayerClass},
    {"DOD_PopHelmet",             Native_PopHelmet},
    {"DOD_SetNumControlPoints",    Native_SetNumControlPoints},
    {"DOD_PrecacheCPIcon",        Native_PrecacheCPIcon},
    {"DOD_SetCPIcons",            Native_SetCPIcons},
    {"DOD_SetCPVisible",          Native_SetCPVisible},
    {"DOD_PauseTimer",            Native_PauseTimer},
    {"DOD_ResumeTimer",           Native_ResumeTimer},
    {"DOD_SetTimeRemaining",       Native_SetTimeRemaining},
    {"DOD_GetTimeRemaining",       Native_GetTimeRemaining},
    {"DOD_RespawnPlayer",         Native_RespawnPlayer},
    {"DOD_AddWaveTime",           Native_AddWaveTime},
    {"DOD_SetWinningTeam",        Native_SetWinningTeam},
    {"DOD_SetRoundState",         Native_SetRoundState},
    {"DOD_SetPlayerState",        Native_SetPlayerState},
    {"DOD_SetBombTargetState",    Native_SetBombTargetState},
    {NULL, NULL},
};

#endif // _INCLUDE_SOURCEMOD_EXTENSION_NATIVES_H_
