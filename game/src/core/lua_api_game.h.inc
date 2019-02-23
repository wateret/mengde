#ifndef MACRO_LUA_GAME
#error  Define MACRO_LUA_GAME before including this file.
#endif

MACRO_LUA_GAME(AppointHero,          appoint_hero)
MACRO_LUA_GAME(GenerateOwnUnit,      generate_own_unit)
MACRO_LUA_GAME(GenerateUnit,         generate_unit)
MACRO_LUA_GAME(ObtainEquipment,      obtain_equipment)

// Game object retrieval
MACRO_LUA_GAME(GetNumEnemiesAlive,   get_num_enemies_alive)
MACRO_LUA_GAME(GetNumOwnsAlive,      get_num_owns_alive)
MACRO_LUA_GAME(GetUnitInfo,          get_unit_info)
MACRO_LUA_GAME(GetUnitOnPosition,    get_unit_on_position)
MACRO_LUA_GAME(GetTerrainOnPosition, get_terrain_on_position)

// Pushing commands
MACRO_LUA_GAME(CmdMove,              cmd_move)
MACRO_LUA_GAME(CmdSpeak,             cmd_speak)
MACRO_LUA_GAME(CmdGainExp,           cmd_gain_exp)
MACRO_LUA_GAME(CmdKill,              cmd_kill)

// Callback registration
MACRO_LUA_GAME(SetOnDeploy,          set_on_deploy)
MACRO_LUA_GAME(SetOnBegin,           set_on_begin)
MACRO_LUA_GAME(SetOnVictory,         set_on_victory)
MACRO_LUA_GAME(SetOnDefeat,          set_on_defeat)
MACRO_LUA_GAME(SetEndCondition,      set_end_condition)
MACRO_LUA_GAME(RegisterEvent,        register_event)
MACRO_LUA_GAME(UnregisterEvent,      unregister_event)

// AI
MACRO_LUA_GAME(SetAIMode,            set_ai_mode)
