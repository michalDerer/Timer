#include "context.hpp"



const char* Context::pathExe = "NULL";

SDL_Window* Context::window = NULL;
SDL_Renderer* Context::renderer = NULL;

lua_State* Context::L = NULL;

const char* Context::pathScripts = "scripts";