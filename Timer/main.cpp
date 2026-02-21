
#include <iostream>
#include <filesystem>

#include "SDL3/SDL.h"
// #include "SDL3/SDL_main.h"
#include "SDL3_image/SDL_image.h"

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}



bool CheckLua(lua_State* L, int r)
{
    if (r != LUA_OK)
    {
        std::string errormsg = lua_tostring(L, -1);
        std::cout << errormsg << "\n";
        return false;
    }
    return true;
}

struct Player
{
    std::string PlayerTitle;
    std::string PlayerName;
    std::string PlayerFamily;
    int PlayerLevel;
} player;




//------------------------------------------------------------------------------------------------------------

int main(int argc, char** argv)
{

  for(int i = 0; i < argc; i++)
    {
        std::cout << argv[i] << "\n";
    }

    std::cout << "filesystem path: " << std::filesystem::current_path() << "\n";

    std::string path(argv[0]);
    // Find last directory separator
    size_t pos = path.find_last_of("\\/");

    std::string directory;
    if (pos != std::string::npos) {
        directory = path.substr(0, pos);
    }
    else {
        directory = "."; // no separator found, use current dir
    }

    std::cout << "Directory: " << directory << std::endl;





    std::string cmd = "a = 7  + 11 + math.sin(23.7)";

    lua_State* L = luaL_newstate();
    luaL_openlibs(L);


    /*
    if (CheckLua(L, luaL_dostring(L, cmd.c_str())))
    {
        lua_getglobal(L, "a");
        if (lua_isnumber(L, -1))
        {
            float a_in_cpp = (float)lua_tonumber(L, -1);
            std::cout << "a is: " << a_in_cpp << "\n";
        }
    }

    cmd = "a = a +100";

    if (CheckLua(L, luaL_dostring(L, cmd.c_str())))
    {
        lua_getglobal(L, "a");
        if (lua_isnumber(L, -1))
        {
            float a_in_cpp = (float)lua_tonumber(L, -1);
            std::cout << "a is: " << a_in_cpp << "\n";
        }
    }
    */

    /*if (CheckLua(L, luaL_dofile(L, "Script.lua")))
    {
        lua_getglobal(L, "PlayerTitle");
        if (lua_isstring(L, -1))
        {
            player.PlayerTitle = (std::string)lua_tostring(L, -1);
            std::cout << "PlayerTitle is: " << player.PlayerTitle << "\n";
        }
    }*/

    /*
    if (CheckLua(L, luaL_dofile(L, "Script.lua")))
    {
        lua_getglobal(L, "player");
        if (lua_istable(L, -1))
        {

            lua_pushstring(L, "PlayerTitle");
            lua_gettable(L, -2); //popne stack na pozicii -1 a pusne value popnuteho kluca
            player.PlayerTitle = lua_tostring(L, -1);
            lua_pop(L, 1); //nakoniec value na vrchu staku popneme, aby bola na vrchu tabulka

            lua_pushstring(L, "PlayerName");
            lua_gettable(L, -2);
            player.PlayerName = lua_tostring(L, -1);
            lua_pop(L, 1);

            lua_pushstring(L, "PlayerFamily");
            lua_gettable(L, -2);
            player.PlayerFamily = lua_tostring(L, -1);
            lua_pop(L, 1);

            lua_pushstring(L, "PlayerLevel");
            lua_gettable(L, -2);
            player.PlayerLevel = lua_tonumber(L, -1);
            lua_pop(L, 1);

            std::cout << player.PlayerFamily << " " << player.PlayerLevel << " " << player.PlayerName << " " << player.PlayerTitle << "\n";
        }
    }
    */

    if (CheckLua(L, luaL_dofile(L, (directory + "/Script.lua").c_str() )))
    //if (CheckLua(L, luaL_dofile(L, "Script.lua")))
    {
        lua_getglobal(L, "AddStuff");
        if (lua_isfunction(L, -1))
        {
            lua_pushnumber(L, 3.5f);
            lua_pushnumber(L, 7.1f);

            if (CheckLua(L, lua_pcall(L, 2, 1, 0)))
            {
                std::cout << "executed result: " << lua_tonumber(L, -1) << "\n";
            }
        }
    }


  //---------------------------------------------------------------------------------------------------------------
  
  
if (!SDL_Init(SDL_INIT_VIDEO) < 0)
{
    SDL_Log("SDL_Init failed: %s", SDL_GetError());
    return 1;
}

//v 3.5 netreba init SDL_image
// if (IMG_Init(IMG_INIT_PNG) == 0)
// {
//     SDL_Log("IMG_Init failed: %s", SDL_GetError());
//     return 1;
// }

SDL_Window* window = SDL_CreateWindow(
    "Timer",
    800,
    600,
    SDL_WINDOW_RESIZABLE
);

if (!window)
{
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window %s", SDL_GetError());
    return 1;
}

SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);


if (!renderer)
{
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create renderer %s", SDL_GetError());
    return 1;
}

SDL_Surface* surface = IMG_Load( (directory + "/pika.png").c_str() );

if (!surface)
{
    SDL_Log("IMG_Load failed: %s", SDL_GetError());
    return 1;
}

SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
SDL_DestroySurface(surface);
surface = NULL;

if (!texture)
{
   SDL_Log("Failed to create texture: %s", SDL_GetError());
   return 1;
}


bool done = false;
while(!done)
{
    SDL_Event event;

    while(SDL_PollEvent(&event))
    {
        if (event.type == SDL_EVENT_QUIT)
        {
            done = true;
        }
    }


    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
    SDL_RenderClear(renderer);
    SDL_RenderTexture(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
    SDL_Delay(16);
}

SDL_DestroyTexture(texture);
SDL_DestroyRenderer(renderer);
SDL_DestroyWindow(window);

SDL_Quit();



//--------------------------------------------------------------------------

//SDL_SetHint(SDL_HINT_VIDEO_DRIVER, "x11");
/*
if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "SDL3 Window Example",
        800, 600,
        0 //SDL_WINDOW_RESIZABLE
    );

    if (!window) {
        printf("SDL_CreateWindow failed: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        printf("SDL_CreateRenderer failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    int running = 1;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = 0;
            }
        }

        SDL_SetRenderDrawColor(renderer, 128, 30, 30, 255);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
*/



  return 0;
}

