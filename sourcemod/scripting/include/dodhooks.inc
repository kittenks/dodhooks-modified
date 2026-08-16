/**
 * DoDHooks - SourcePawn Include File
 * Updated for SourceMod 1.12 / 1.13
 * 
 * Native and detour functions for Day of Defeat: Source
 */

#if defined _dodhooks_included
 #endinput
#endif
#define _dodhooks_included

// ============================================================================
// Enumerations
// ============================================================================

// Player classes
enum DODPlayerClass
{
    DODClass_Random = -2,
    DODClass_None = 0,
    DODClass_Rifleman = 1,
    DODClass_Assault = 2,
    DODClass_Support = 3,
    DODClass_Sniper = 4,
    DODClass_Machinegunner = 5,
    DODClass_Rocket = 6
};

// Teams
enum DODTeam
{
    DODTeam_Unassigned = 0,
    DODTeam_Spectator = 1,
    DODTeam_Allies = 2,
    DODTeam_Axis = 3
};

// Round states
enum DODRoundState
{
    DODRound_Init = 0,
    DODRound_Preround = 1,
    DODRound_RoundGoing = 2,
    DODRound_AlliesWin = 3,
    DODRound_AxisWin = 4,
    DODRound_Draw = 5,
    DODRound_BonusRound = 6
};

// Player states
enum DODPlayerState
{
    DODPlayer_Active = 0,
    DODPlayer_Dying = 1,
    DODPlayer_Dead = 2,
    DODPlayer_Observer = 3
};

// Bomb target states
enum DODBombTargetState
{
    DODBomb_Active = 0,
    DODBomb_AlliesPlanted = 1,
    DODBomb_AxisPlanted = 2,
    DODBomb_Disabled = 3,
    DODBomb_Exploded = 4
};

// ============================================================================
// Natives
// ============================================================================

/**
 * Gets a player's current class.
 *
 * @param client    Client index.
 * @return          Current DODPlayerClass value.
 * @error           Invalid client index.
 */
native int DOD_GetPlayerClass(int client);

/**
 * Sets a player's current class.
 *
 * @param client    Client index.
 * @param class     DODPlayerClass value to set.
 * @return          True on success, false otherwise.
 * @error           Invalid client or class.
 */
native bool DOD_SetPlayerClass(int client, int class);

/**
 * Gets a player's desired class (class they want to spawn as).
 *
 * @param client    Client index.
 * @return          Desired DODPlayerClass value.
 * @error           Invalid client index.
 */
native int DOD_GetDesiredPlayerClass(int client);

/**
 * Sets a player's desired class.
 *
 * @param client    Client index.
 * @param class     DODPlayerClass value to set.
 * @return          True on success, false otherwise.
 * @error           Invalid client or class.
 */
native bool DOD_SetDesiredPlayerClass(int client, int class);

/**
 * Forces a player to pop their helmet.
 *
 * @param client        Client index.
 * @param vecVelocity   Velocity vector (3D array).
 * @param vecOrigin     Origin vector (3D array).
 * @return              True on success.
 * @error               Invalid client index.
 */
native bool DOD_PopHelmet(int client, float vecVelocity[3], float vecOrigin[3]);

/**
 * Sets the number of control points.
 *
 * @param numCPs    Number of control points (0-8).
 * @return          True on success.
 */
native bool DOD_SetNumControlPoints(int numCPs);

/**
 * Precaches a control point icon material.
 *
 * @param materialName  Material name to precache.
 * @return              Material index, or 0 on failure.
 */
native int DOD_PrecacheCPIcon(const char[] materialName);

/**
 * Sets the icons for a control point.
 *
 * @param cpIndex           Control point index.
 * @param alliesIcon        Allied icon material index (0 = no change).
 * @param axisIcon          Axis icon material index (0 = no change).
 * @param neutralIcon       Neutral icon material index (0 = no change).
 * @param timerCapIcon      Timer capture icon material index (0 = no change).
 * @param bombedIcon        Bombed icon material index (0 = no change).
 * @return                  True on success.
 */
native bool DOD_SetCPIcons(int cpIndex, int alliesIcon, int axisIcon, int neutralIcon, int timerCapIcon, int bombedIcon);

/**
 * Sets a control point's visibility.
 *
 * @param cpIndex   Control point index.
 * @param visible   True to show, false to hide.
 * @return          True on success.
 */
native bool DOD_SetCPVisible(int cpIndex, bool visible);

/**
 * Pauses a round timer.
 *
 * @param timerEnt  Entity index of the dod_round_timer.
 * @return          True on success.
 * @error           Invalid timer entity.
 */
native bool DOD_PauseTimer(int timerEnt);

/**
 * Resumes a round timer.
 *
 * @param timerEnt  Entity index of the dod_round_timer.
 * @return          True on success.
 * @error           Invalid timer entity.
 */
native bool DOD_ResumeTimer(int timerEnt);

/**
 * Sets the remaining time on a round timer.
 *
 * @param timerEnt          Entity index of the dod_round_timer.
 * @param timeRemaining     Time remaining in seconds.
 * @return                  True on success.
 * @error                   Invalid timer entity.
 */
native bool DOD_SetTimeRemaining(int timerEnt, float timeRemaining);

/**
 * Gets the remaining time on a round timer.
 *
 * @param timerEnt  Entity index of the dod_round_timer.
 * @return          Time remaining in seconds.
 * @error           Invalid timer entity.
 */
native float DOD_GetTimeRemaining(int timerEnt);

/**
 * Respawns a player.
 *
 * @param client            Client index.
 * @param forceRespawn     If true, respawn even if not dead.
 * @return                 True on success.
 * @error                  Invalid client.
 */
native bool DOD_RespawnPlayer(int client, bool forceRespawn = true);

/**
 * Adds wave time to a team.
 *
 * @param team     DODTeam value.
 * @param delay    Additional wave time in seconds.
 * @return         True on success.
 */
native bool DOD_AddWaveTime(int team, float delay);

/**
 * Sets the winning team.
 *
 * @param team     DODTeam value (must be DODTeam_Allies or DODTeam_Axis).
 * @return         True on success.
 */
native void DOD_SetWinningTeam(int team);

/**
 * Sets the round state.
 *
 * @param state    DODRoundState value.
 * @return         True on success.
 */
native bool DOD_SetRoundState(int state);

/**
 * Sets a player's state.
 *
 * @param client   Client index.
 * @param state    DODPlayerState value.
 * @return         True on success.
 */
native bool DOD_SetPlayerState(int client, int state);

/**
 * Sets a bomb target's state.
 *
 * @param entity   Entity index of the dod_bomb_target.
 * @param state    DODBombTargetState value.
 * @return         True on success.
 */
native bool DOD_SetBombTargetState(int entity, int state);

// ============================================================================
// Forwards (Detours)
// ============================================================================

/**
 * Called when a player uses a voice command.
 *
 * @param client        Client index.
 * @param voiceCommand Voice command number (modified by reference).
 * @return              Plugin_Continue to allow, Plugin_Handled to block.
 */
forward Action OnVoiceCommand(int client, int &voiceCommand);

/**
 * Called when a player joins a class.
 *
 * @param client   Client index.
 * @param class    DODPlayerClass (modified by reference).
 * @return         Plugin_Continue to allow, Plugin_Handled to block.
 */
forward Action OnJoinClass(int client, int &class);

/**
 * Called when a player's helmet is popped off.
 *
 * @param client    Client index.
 * @param velocity  Helmet velocity vector (modified by reference).
 * @param origin    Helmet origin vector (modified by reference).
 * @return          Plugin_Continue to allow, Plugin_Handled to block.
 */
forward Action OnPopHelmet(int client, float velocity[3], float origin[3]);

/**
 * Called when a player is about to respawn.
 *
 * @param client   Client index.
 * @return         Plugin_Continue to allow, Plugin_Handled to block.
 */
forward Action OnPlayerRespawn(int client);

/**
 * Called when wave time is being added.
 *
 * @param team     Team index.
 * @param delay    Wave delay in seconds (modified by reference).
 * @return         Plugin_Continue to allow, Plugin_Handled to block.
 */
forward Action OnAddWaveTime(int team, float &delay);

/**
 * Called when the winning team is being set.
 *
 * @param team     Winning team index.
 * @return         Plugin_Continue to allow, Plugin_Handled to block.
 */
forward Action OnSetWinningTeam(int team);

/**
 * Called when the round state changes.
 *
 * @param state    DODRoundState (modified by reference).
 * @return         Plugin_Continue to allow, Plugin_Handled to block.
 */
forward Action OnEnterRoundState(int &state);

/**
 * Called when a player's state changes.
 *
 * @param client   Client index.
 * @param state    DODPlayerState (modified by reference).
 * @return         Plugin_Continue to allow, Plugin_Handled to block.
 */
forward Action OnEnterPlayerState(int client, int &state);

/**
 * Called when a bomb target's state changes.
 *
 * @param entity   Bomb target entity index.
 * @param state    DODBombTargetState (modified by reference).
 * @return         Plugin_Continue to allow, Plugin_Handled to block.
 */
forward Action OnEnterBombTargetState(int entity, int &state);

// ============================================================================
// Utility functions
// ============================================================================

/**
 * Checks if the DODHooks extension is available.
 *
 * @return  True if available, false otherwise.
 */
stock bool DODHooks_IsAvailable()
{
    return GetFeatureStatus(FeatureType_Native, "DOD_GetPlayerClass") == FeatureStatus_Available;
}

/**
 * Gets the platform and architecture the extension was built for.
 *
 * @return  String containing platform info (e.g. "Linux x86_64", "Windows x86").
 */
native char[] DODHooks_GetPlatform();
