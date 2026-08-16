/**
 * DoDHooks - Day of Defeat: Source Extension
 * Version 2.0.0 - Compatible with SourceMod 1.12/1.13
 * 
 * smsdk_config.h - SDK configuration
 */

#ifndef _INCLUDE_DODHOOKS_CONFIG_H_
#define _INCLUDE_DODHOOKS_CONFIG_H_

#define DODHOOKS_VERSION "2.0.0"
#define DODHOOKS_BUILD    "2026-08-16"

// SM 1.12 SDK version
#define SMEXT_CONF_SDK_VERSION   "1.12.x"
#define SMEXT_CONF_MIN_APILEVEL 0x0210  // API 2.1+

// Extension info
#define SMEXT_CONF_NAME         "DoDHooks"
#define SMEXT_CONF_DESCRIPTION  "Day of Defeat: Source server hooks and natives"
#define SMEXT_CONF_AUTHOR       "AlliedModders / DodHooks Contributors"
#define SMEXT_CONF_VERSION      DODHOOKS_VERSION
#define SMEXT_CONF_URL          "https://github.com/kittenks/dodhooks-modified"
#define SMEXT_CONF_LOGTAG       "DODHOOKS"
#define SMEXT_CONF_LICENSE      "GPLv3"

// Game: Day of Defeat: Source
#define SMEXT_CONF_GAME          "dod"
#define SMEXT_CONF_GAMEDIR      "dod"

// Features
#define DODHOOKS_ENABLE_VOICECMD     1
#define DODHOOKS_ENABLE_CLASSCHANGE  1
#define DODHOOKS_ENABLE_HELMPOP      1
#define DODHOOKS_ENABLE_SPAWN       1
#define DODHOOKS_ENABLE_CAPCONTROL   1
#define DODHOOKS_ENABLE_BOMBTARGET   1
#define DODHOOKS_ENABLE_TIMER        1

// Team enums (DoD:S)
#define DODTEAM_SPECTATOR  1
#define DODTEAM_ALLIES     2
#define DODTEAM_AXIS       3

// Player class enums (DoD:S)
#define DODCLASS_RIFLEMAN   0
#define DODCLASS_ASSAULT    1
#define DODCLASS_SUPPORT    2
#define DODCLASS_SNIPER     3
#define DODCLASS_MG         4
#define DODCLASS_ROCKET     5

// Weapon IDs
#define DODWEP_GARAND        0
#define DODWEP_KAR            1
#define DODWEP_BAR            2
#define DODWEP_MP40           3
#define DODWEP_THOMPSON       4
#define DODWEP_STG44          5
#define DODWEP_SPRINGFIELD    6
#define DODWEP_SCOUT         10
#define DODWEP_30CAL         11
#define DODWEP_MG42          12
#define DODWEP_BAZOOKA       13
#define DODWEP_PANZER        14
#define DODWEP_PISTOL        15
#define DODWEP_GREN_FRAG     16
#define DODWEP_GREN_STICK    17
#define DODWEP_GREN_SMOKE    18
#define DODWEP_KNIFE         19
#define DODWEP_SPADE         20

#endif // _INCLUDE_DODHOOKS_CONFIG_H_
