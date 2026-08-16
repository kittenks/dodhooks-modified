#ifndef _INCLUDE_SOURCEMOD_EXTENSION_VGLOBALS_H_
#define _INCLUDE_SOURCEMOD_EXTENSION_VGLOBALS_H_

// Valve globals - external declarations
extern void *g_pEntList;
extern void **g_pObjectiveResource;

// Initialize Valve global pointers from gamedata
void InitializeValveGlobals();

#endif // _INCLUDE_SOURCEMOD_EXTENSION_VGLOBALS_H_
