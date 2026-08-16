/**
 * DoDHooks - Day of Defeat: Source
 * SourcePawn Include File
 * Version 2.0.0
 */

#if defined _dodhooks_included
 #endinput
#endif
#define _dodhooks_included

// ============================================================
// Version
// ============================================================
#define DODHOOKS_VERSION_MAJOR  2
#define DODHOOKS_VERSION_MINOR  0
#define DODHOOKS_VERSION_BUILD  0

// ============================================================
// Action values (return from forward)
// ============================================================
#define DODHOOK_ACTION_CONTINUE  0
#define DODHOOK_ACTION_CHANGED  1
#define DODHOOK_ACTION_HANDLED  2
#define DODHOOK_ACTION_STOP     3

// ============================================================
// Player classes
// ============================================================
#define DODCLASS_RIFLEMAN   0
#define DODCLASS_ASSAULT    1
#define DODCLASS_SUPPORT    2
#define DODCLASS_SNIPER     3
#define DODCLASS_MG         4
#define DODCLASS_ROCKET     5

// ============================================================
// Teams
// ============================================================
#define DODTEAM_SPECTATOR  1
#define DODTEAM_ALLIES     2
#define DODTEAM_AXIS       3

// ============================================================
// Voice commands
// ============================================================
#define DODVOICE_MEDIC     0
#define DODVOICE_AMMO      1
#define DODVOICE_GOGOGO    2
#define DODVOICE_FALLBACK  3
#define DODVOICE_THANKS    4
#define DODVOICE_YES       5
#define DODVOICE_NO        6

// ============================================================
// Weapon IDs
// ============================================================
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

// ============================================================
// Platform constants
// ============================================================
#define DODHOOK_PLATFORM_WIN32    0
#define DODHOOK_PLATFORM_LINUX32  1
#define DODHOOK_PLATFORM_WIN64    2
#define DODHOOK_PLATFORM_LINUX64  3

// ============================================================
// Forwards (callbacks)
// ============================================================

/**
 * Called when a player uses a voice command.
 *
 * @param client    Client index
 * @param commandId Voice command ID
 * @return         Action to take
 */
forward Action DODHooks_OnVoiceCommand(int client, int commandId);

/**
 * Called when a player changes class.
 *
 * @param client    Client index
 * @param oldClass  Previous class
 * @param newClass  New class
 * @return         Action to take
 */
forward Action DODHooks_OnClassChange(int client, int oldClass, int newClass);

/**
 * Called when a player's helmet pops off.
 *
 * @param client    Client index
 * @param attacker  Attacker index
 * @param weaponId  Weapon that caused the pop
 * @return         Action to take
 */
forward Action DODHooks_OnHelmetPop(int client, int attacker, int weaponId);

/**
 * Called before a player spawns.
 *
 * @param client    Client index
 * @return         Action to take
 */
forward Action DODHooks_OnPreSpawn(int client);

/**
 * Called after a player spawns.
 *
 * @param client    Client index
 */
forward void DODHooks_OnPostSpawn(int client);

/**
 * Called when a player teleports.
 *
 * @param client    Client index
 * @param origin    New position
 * @param angles    New angles
 * @param velocity  New velocity
 * @return         Action to take
 */
forward Action DODHooks_OnTeleport(int client, float origin[3], float angles[3], float velocity[3]);

/**
 * Called when a control point changes control.
 *
 * @param cpIndex   Control point index
 * @param oldTeam   Previous owning team
 * @param newTeam   New owning team
 * @return         Action to take
 */
forward Action DODHooks_OnCapControl(int cpIndex, int oldTeam, int newTeam);

/**
 * Called when a bomb is planted.
 *
 * @param client    Planter index
 * @param target    Bomb target entity
 * @return         Action to take
 */
forward Action DODHooks_OnBombPlant(int client, int target);

/**
 * Called when a bomb is defused.
 *
 * @param client    Defuser index
 * @param target    Bomb target entity
 * @return         Action to take
 */
forward Action DODHooks_OnBombDefuse(int client, int target);

/**
 * Called when a round starts.
 */
forward void DODHooks_OnRoundStart();

/**
 * Called when a round ends.
 *
 * @param winner    Winning team
 */
forward void DODHooks_OnRoundEnd(int winner);

/**
 * Called when a player dies.
 *
 * @param victim    Victim index
 * @param attacker  Attacker index
 * @param weaponId  Weapon used
 * @return         Action to take
 */
forward Action DODHooks_OnPlayerDeath(int victim, int attacker, int weaponId);

// ============================================================
// Natives
// ============================================================

/**
 * Get a player's current class.
 *
 * @param client    Client index
 * @return         Class ID or -1 on error
 */
native int DODHooks_GetPlayerClass(int client);

/**
 * Set a player's class.
 *
 * @param client    Client index
 * @param classId   Class ID
 * @return         true on success
 */
native bool DODHooks_SetPlayerClass(int client, int classId);

/**
 * Get a player's team.
 *
 * @param client    Client index
 * @return         Team ID
 */
native int DODHooks_GetPlayerTeam(int client);

/**
 * Set a player's team.
 *
 * @param client    Client index
 * @param team      Team ID
 * @return         true on success
 */
native bool DODHooks_SetPlayerTeam(int client, int team);

/**
 * Get a player's active weapon.
 *
 * @param client    Client index
 * @return         Weapon ID or -1
 */
native int DODHooks_GetPlayerWeapon(int client);

/**
 * Give a weapon to a player.
 *
 * @param client    Client index
 * @param weaponId  Weapon ID
 * @return         true on success
 */
native bool DODHooks_GivePlayerWeapon(int client, int weaponId);

/**
 * Remove a weapon from a player.
 *
 * @param client    Client index
 * @param weaponId  Weapon ID
 * @return         true on success
 */
native bool DODHooks_RemovePlayerWeapon(int client, int weaponId);

/**
 * Get control point index from entity.
 *
 * @param entity    Entity index
 * @return         CP index or -1
 */
native int DODHooks_GetCapIndex(int entity);

/**
 * Get control point owner.
 *
 * @param cpIndex   Control point index
 * @return         Team ID
 */
native int DODHooks_GetCapOwner(int cpIndex);

/**
 * Set control point owner.
 *
 * @param cpIndex   Control point index
 * @param team      Team ID
 * @return         true on success
 */
native bool DODHooks_SetCapOwner(int cpIndex, int team);

/**
 * Get capture progress (0-100).
 *
 * @param cpIndex   Control point index
 * @return         Progress percentage
 */
native int DODHooks_GetCapProgress(int cpIndex);

/**
 * Get the active bomb target entity.
 *
 * @return         Entity index or -1
 */
native int DODHooks_GetBombTarget();

/**
 * Check if a bomb is planted.
 *
 * @return         true if planted
 */
native bool DODHooks_IsBombPlanted();

/**
 * Get bomb timer in seconds.
 *
 * @return         Seconds remaining
 */
native int DODHooks_GetBombTimer();

/**
 * Get current round time in seconds.
 *
 * @return         Seconds
 */
native int DODHooks_GetRoundTime();

/**
 * Set round time.
 *
 * @param seconds   New round time
 * @return         true on success
 */
native bool DODHooks_SetRoundTime(int seconds);

/**
 * Check if a player is spawned (alive and in-game).
 *
 * @param client    Client index
 * @return         true if spawned
 */
native bool DODHooks_IsPlayerSpawned(int client);

/**
 * Get the current platform.
 *
 * @return         Platform constant
 */
native int DODHooks_GetPlatform();
