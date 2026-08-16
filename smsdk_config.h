/**
 * DoDHooks - SourceMod Extension Configuration
 * Updated for SourceMod 1.12 / 1.13
 * Supports both 32-bit (x86) and 64-bit (x86_64)
 */

#ifndef _INCLUDE_SOURCEMOD_EXTENSION_CONFIG_H_
#define _INCLUDE_SOURCEMOD_EXTENSION_CONFIG_H_

/**
 * @file smsdk_config.h
 * @brief Contains macros for configuring extension build options
 */

// Version info
#define SMEXT_CONF_VERSION      "2.0.0"
#define SMEXT_CONF_AUTHOR       "Joakim Andersson 'Andersso', ChesterSmitty, DNA-styx, Apfelwurm"
#define SMEXT_CONF_NAME         "DoDHooks"
#define SMEXT_CONF_DESCRIPTION  "SourceMod extension with Detours & Natives for Day of Defeat: Source"
#define SMEXT_CONF_URL          "https://github.com/DODSLeague/dodhooks"
#define SMEXT_CONF_LOGTAG       "DODHOOKS"
#define SMEXT_CONF_LICENSE      "GPL-2.0"

// Flags
#define SMEXT_CONF_METAMOD              // Requires Metamod:Source
#define SMEXT_CONF_DEBUGLOG             // Enable debug logging

// Required extensions
#define SMEXT_CONF_REQUIRE_BINTOOLS
#define SMEXT_CONF_REQUIRE_SDKTOOLS

// SDK compatibility
#define SMEXT_CONF_VERSIONS            "1.12.x", "1.13.x"

// Maximum number of control points in DoD:S
#define MAX_CONTROL_POINTS              8

// Player class definitions
#define PlayerClass_Random              -2
#define PlayerClass_None                0
#define PlayerClass_Rifleman            1
#define PlayerClass_Assault             2
#define PlayerClass_Support             3
#define PlayerClass_Sniper              4
#define PlayerClass_Machinegunner        5
#define PlayerClass_Rocket              6
#define PlayerClass_Size                7

// Team definitions
#define Team_Unassigned                0
#define Team_Spectator                 1
#define Team_Allies                    2
#define Team_Axis                      3

// Round state definitions (DODRoundState)
#define RoundState_Init                 0
#define RoundState_Preround             1
#define RoundState_Round_Going          2
#define RoundState_Allies_Win           3
#define RoundState_Axis_Win             4
#define RoundState_Draw                 5
#define RoundState_Bonus_Round          6

// Player state definitions (DODPlayerState)
#define PlayerState_Active              0
#define PlayerState_Dying               1
#define PlayerState_Dead                2
#define PlayerState_Observer            3

// Bomb target state definitions (BombTargetState)
#define BombTargetState_Active          0
#define BombTargetState_AlliesPlanted   1
#define BombTargetState_AxisPlanted     2
#define BombTargetState_Disabled        3
#define BombTargetState_Exploded        4

#endif // _INCLUDE_SOURCEMOD_EXTENSION_CONFIG_H_
