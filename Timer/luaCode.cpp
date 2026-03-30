
#include <iostream>

#include "lua.hpp"




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
} player {};


void louaCode()
{

    /*
    for(int i = 0; i < argc; i++)
    {
        std::cout << "argv[" << i << "]: " << argv[i] << "\n";
    }

    std::cout << "filesystem path: " << std::filesystem::current_path() << "\n";

    std::string path(argv[0]);
    // Find last directory separator
    size_t pos = path.find_last_of("\\/");

    std::string directory;
    if (pos != std::string::npos) 
    {
        directory = path.substr(0, pos);
    }
    else {
        directory = "."; // no separator found, use current dir
    }

    std::cout << "Directory: " << directory << std::endl;
    */



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

    /*
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
    */

    lua_close(L);
}