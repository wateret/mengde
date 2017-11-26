#include "lua_game.h"
#include "lua/lua_script.h"
#include "game.h"
#include "cmd.h"

static Game* lua_get_game_object(lua_State* L) {
  lua_getglobal(L, LUA_GAME_OBJ_NAME);
  Game* game = static_cast<Game*>(lua_touserdata(L, -1));
  lua_pop(L, 1);
  return game;
}

static Vec2D GetVec2DFromLua(LuaScript* lua) {
  vector<int> vec = lua->GetVector<int>();
  lua->PopStack(1); // FIXME GetVector does not pop a element.
  return {vec[0], vec[1]};
}

#define LUA_IMPL(cname) \
  int Game_##cname(lua_State* L)

LUA_IMPL(GenerateUnit) {
  Game* game = lua_get_game_object(L);
  LuaScript lua(L);
  Vec2D pos = GetVec2DFromLua(&lua);
  Unit::Side side = (Unit::Side)lua.Get<int>();
  string id = lua.Get<string>();
  int unit_no = game->GenerateUnit(id, side, pos);
  lua.PushToStack(unit_no);
  return 1;
}

LUA_IMPL(UnitPutItemsOn) {
  Game* game = lua_get_game_object(L);
  LuaScript lua(L);
  string aid_s = lua.Get<string>();
  string armor_s = lua.Get<string>();
  string weapon_s = lua.Get<string>();
  int no = lua.Get<int>();

  Unit* unit = game->GetUnit(no);
  if (weapon_s != "nil") {
    Item* weapon = game->GetItem(weapon_s);
    unit->PutWeaponOn(weapon);
  }
  if (armor_s != "nil") {
    Item* armor = game->GetItem(armor_s);
    unit->PutWeaponOn(armor);
  }
  if (aid_s != "nil") {
    Item* aid = game->GetItem(aid_s);
    unit->PutAidOn(aid);
  }
  return 0;
}

LUA_IMPL(GetNumEnemiesAlive) {
  Game* game = lua_get_game_object(L);
  uint32_t ret = game->GetNumEnemiesAlive();
  lua_pushnumber(L, ret);
  return 1;
}

LUA_IMPL(GetNumOwnsAlive) {
  Game* game = lua_get_game_object(L);
  uint32_t ret = game->GetNumOwnsAlive();
  lua_pushnumber(L, ret);
  return 1;
}

LUA_IMPL(PushCmdMove) {
  Game* game = lua_get_game_object(L);
  LuaScript lua(L);
  Vec2D pos = GetVec2DFromLua(&lua);
  int unit_id = lua.Get<int>();
  Unit* unit = game->GetUnit(unit_id);
  game->PushCmd(unique_ptr<CmdMove>(new CmdMove(unit, pos)));
  return 0;
}

LUA_IMPL(PushCmdSpeak) {
  Game* game = lua_get_game_object(L);
  LuaScript lua(L);
  string words = lua.Get<string>();
  int unit_id = lua.Get<int>();
  Unit* unit = game->GetUnit(unit_id);
  game->PushCmd(unique_ptr<CmdSpeak>(new CmdSpeak(unit, words)));
  return 0;
}

#undef LUA_IMPL
