#pragma once

#include <memory>

#include "lua.hpp"

#include "core.hpp"



struct LuaTexture
{
    std::shared_ptr<SDL_Texture> texture;
};

int LuaTexture_new(lua_State* L);
int LuaTexture_gc(lua_State* L);

void register_LuaTexture(lua_State* L);

//------------------------------------------------------------------------------------------------

struct LuaImage
{
    Image* image = nullptr;
};

//int LuaImage_new(lua_State* L);
int LuaImage_gc(lua_State* L);

void register_LuaImage(lua_State* L);

//------------------------------------------------------------------------------------------------

struct LuaRectTransform
{
    RectTransform rectTransform;

    LuaRectTransform();
    LuaRectTransform(RectTransform* parent);
};

int LuaRectTransform_new(lua_State* L);
int LuaRectTransform_gc(lua_State* L);
int LuaRectTransform_set_parent(lua_State* L);
int LuaRectTransform_set_values(lua_State* L);
int LuaRectTransform_add_behaviour(lua_State* L);

void register_LuaRectTransform(lua_State* L);

//------------------------------------------------------------------------------------------------

void register_API(lua_State* L);


