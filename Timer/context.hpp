#pragma once

#include "SDL3/SDL.h"

#include "lua.hpp"



/// <summary>
/// 
/// </summary>
class Context
{

public:

    static const char*     pathExe;

    static SDL_Window*     window;
    static SDL_Renderer*   renderer;

    static lua_State*      L;

    static const char*     pathScripts;

public:

    Context()                               = delete;

    Context(const Context&)                 = delete;   //Copying disabled
    Context& operator=(const Context&)      = delete;

    Context(Context&&) noexcept             = delete;    //Moving disabled
    Context& operator=(Context&&) noexcept  = delete;

    ~Context() = delete;
};