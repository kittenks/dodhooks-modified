/**
 * vim: set ts=4 :
 * ======================================================
 * DODHooks - Valve Globals interface
 * ======================================================
 *
 * GPLv2 License
 */

#ifndef _INCLUDE_SOURCEMOD_EXTENSION_VGLOBALS_H_
#define _INCLUDE_SOURCEMOD_EXTENSION_VGLOBALS_H_

/**
 * @brief Global pointer to the entity list.
 */
extern void *g_pEntList;

/**
 * @brief Global pointer to the objective resource pointer.
 * On Windows this is a pointer to the global variable.
 * On Linux this is the address of the global variable itself.
 */
extern void **g_pObjectiveResource;

/**
 * @brief Initializes valve globals from gamedata signatures.
 * Must be called after g_pGameConf is loaded and after the
 * engine interfaces are available.
 */
void InitializeValveGlobals();

#endif // _INCLUDE_SOURCEMOD_EXTENSION_VGLOBALS_H_
