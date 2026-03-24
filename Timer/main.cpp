
//#include <iostream>
#include <cstdio>
#include <filesystem>
#include <string>

#include "SDL3/SDL.h"
#include "SDL3_image/SDL_image.h"

#include "lua.hpp"

#include "core.hpp"



//------------------------------------------------------------------------------------------------------------


const char*     pathExe = NULL;

SDL_Window*     window = NULL;
SDL_FRect       windowRect{};
SDL_Renderer*   renderer = NULL;

const char*     pathPikachu = "pika.png";
SDL_Texture*    texturePikachu = NULL;

lua_State*      L = NULL;
const char*     pathScriptsDir = "scripts";


//------------------------------------------------------------------------------------------------------------


static int initPathToExe()
{
    pathExe = SDL_GetBasePath();

    if (pathExe)
    {
        printf("pathExe: %s\n", pathExe);

        return 0;
    }
    else
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_GetBasePath failed: %s", SDL_GetError());

        return 1;
    }
}


static int createWindow(/*SDL_Window** window, SDL_Renderer** renderer*/)
{
    //SDL_SetHint(SDL_HINT_VIDEO_DRIVER, "x11");

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        //SDL_Log("SDL_Init failed: %s", SDL_GetError());
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_Init failed: %s", SDL_GetError());

        return 1;
    }

    // SDL_Window* window = SDL_CreateWindow("Timer", 800, 600, SDL_WINDOW_RESIZABLE);
    // SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);

    if (!SDL_CreateWindowAndRenderer(
        "Timer",
        800, 600,
        SDL_WINDOW_RESIZABLE,
        &window, &renderer))
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window or renderer or both %s", SDL_GetError());

        return 1;
    }

    // if (!window)
    // {
    //     SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window %s", SDL_GetError());
    // 
    //     return 1;
    // }

    // if (!renderer)
    // {
    //     SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create renderer %s", SDL_GetError());
    // 
    //     return 1;
    // }

    return 0;
}


static int loadAssets(/*const char* pathToExe, SDL_Renderer* renderer, SDL_Texture** texturePikachu*/)
{
    if (pathExe == NULL)
    {
        SDL_Log("pathExe is null");

        return 1;
    }

    printf("pathPikachuFull: %s\n", std::string(pathExe).append(pathPikachu).c_str());
    SDL_Surface* surface = IMG_Load(std::string(pathExe).append(pathPikachu).c_str());
   
    if (!surface)
    {
        SDL_Log("IMG_Load failed: %s", SDL_GetError());

        return 1;
    }

    texturePikachu = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);
    surface = NULL;

    if (!texturePikachu)
    {
        SDL_Log("Failed to create texture: %s", SDL_GetError());

        return 1;
    }

    return 0;
}


static int executeLuaScripts()
{
    //vytvorit lua state

    L = luaL_newstate();
    //luaL_openlibs(L);

    //luaopen_base(L);
    //lua_setglobal(L, "_G");

    luaL_requiref(L, "_G", luaopen_base, 1);
    lua_pop(L, 1);

    luaL_requiref(L, "math", luaopen_math, 1);
    lua_pop(L, 1);

    //naplnit lua state Timer API metodami

    lua_pushcfunction(L, l_sin);
    lua_setglobal(L, "mysin");


    //najst a vykonat lua skripty

    std::string pathScriptsDirFull{pathExe};
    pathScriptsDirFull.append(pathScriptsDir);
    printf("pathScriptsDirFull: %s\n", pathScriptsDirFull.c_str());

    std::filesystem::path p{pathScriptsDirFull};

    if (!std::filesystem::exists(p) || !std::filesystem::is_directory(p))
    {
        printf("pathScriptsDirFull je nevalidna\n");

        return 1;
    }

    for (const auto& entry : std::filesystem::directory_iterator(p))
    {
        if (entry.is_regular_file() && entry.path().extension() == ".lua")
        {
            printf("Executing script: %s\n", entry.path().string().c_str());

            //if (luaL_dostring(L, "lua_Code") != LUA_OK)
            //{
            //   printf("Error: %s\n", lua_tostring(L, -1));
            //   return 1;
            //}

            if (luaL_dofile(L, entry.path().string().c_str()) != LUA_OK)
            {
                printf("Error: %s\n", lua_tostring(L, -1));
                    
                return 1;
            }
        }
    }

    return 0; 
}


static void freeAll()
{
    if (L)
    {
        lua_close(L);
    }

    //nemozem uvolnit pathToExe, lebo sa uvolnuje v SDL_Quit()
    /*if (pathToExe)
    {
        SDL_free((void*)pathToExe);
        pathToExe = nullptr;
    }*/

    if (texturePikachu)
    {
        SDL_DestroyTexture(texturePikachu);
        texturePikachu = nullptr;
    }

    if (renderer)
    {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }

    if (window)
    {
        SDL_DestroyWindow(window);
        window = nullptr;
    }

    SDL_Quit();
}


typedef int (*Method)(void);

static int freeIfNotReturn0(Method method)
//static int freeIfNotReturn0(int(*method)(void))
{
    int result = method();
    if (result != 0)
    {
        freeAll();
    }

    return result;
}


//------------------------------------------------------------------------------------------------------------


int main(int argc, char** argv)
{
    
    //int initPathToExeResult = initPathToExe();
    //if (initPathToExeResult != 0)
    //{
    //    freeAll();
    //    return initPathToExeResult;
    //}
    if (auto result = freeIfNotReturn0(initPathToExe)) return result;
    
    //int createWindowResult = createWindow();
    //if (createWindowResult != 0)
    //{
    //    freeAll();
    //    return createWindowResult;
    //}
    if (auto result = freeIfNotReturn0(createWindow)) return result;

    //int loadAssetsResult = loadAssets();
    //if (loadAssetsResult != 0)
    //{
    //    freeAll();
    //    return loadAssetsResult;
    //}
    if (auto result = freeIfNotReturn0(loadAssets)) return result;

    //int executeLuaScriptsResult = executeLuaScripts();
    //if (executeLuaScriptsResult != 0)
    //{
    //    freeAll();
    //    return executeLuaScriptsResult;
    //}
    if (auto result = freeIfNotReturn0(executeLuaScripts)) return result;


    int winW, winH;
    SDL_GetWindowSize(window, &winW, &winH);
    windowRect.x = 0;
    windowRect.y = 0;
    windowRect.w = static_cast<float>(winW);
    windowRect.h = static_cast<float>(winH);
    
    RectTransform rt;
    rt.set_anchorMinX(0.1f);
    rt.set_anchorMaxX(0.9f);
    rt.set_anchorMinY(0.1f);
    rt.set_anchorMaxY(0.9f);
    rt.set_right(1.f);
    rt.set_bottom(1.f);
    RectTransform rt2;
    rt2.set_anchorMinX(0.1f);
    rt2.set_anchorMaxX(0.7f);
    rt2.set_anchorMinY(0.1f);
    rt2.set_anchorMaxY(0.9f);
    rt2.set_right(1.f);
    rt2.set_bottom(1.f);
    rt.add_child(&rt2);

    //Image img{ &rt, texturePikachu };
    //rt.add_behaviour(&img);

    auto img = rt2.add_behaviour<Image>(texturePikachu);
    img->preserveAspectRation = true;
    img->alignHorizontal = ImageAlignHorizontal::CENTER;
    img->alignVertical = ImageAlignVertical::CENTER;


    {
        //rt.update_rect(windowRect);
        //rt2->update_rect(rt.get_rect());

        std::vector<RectTransform*> transformy{ &rt };

        while (!transformy.empty())
        {
            RectTransform* cur = transformy.front();
            transformy.erase(transformy.begin());

            if (cur->get_parent() == NULL)
            {
                cur->update_rect(windowRect);
            }
            else
            {
                cur->update_rect(cur->get_parent()->get_rect());
            }

            for (int i = 0; i < cur->get_childCount(); i++)
            {
                transformy.push_back(cur->get_child(i));
            }
        }
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
                printf("EVENT_QUIT\n");
            }
            else if (event.type == SDL_EVENT_WINDOW_RESIZED)
            {
                winW = event.window.data1;
                winH = event.window.data2;
                
                printf("EVENT_WINDOW_RESIZED w: %i h: %i\n", winW, winH);

                windowRect.x = 0;
                windowRect.y = 0;
                windowRect.w = static_cast<float>(winW);
                windowRect.h = static_cast<float>(winH);

                {
                    //rt.update_rect(windowRect);
                    //rt2.update_rect(rt.get_rect());

                    std::vector<RectTransform*> transformy{ &rt };

                    while (!transformy.empty())
                    {
                        RectTransform* cur = transformy.front();
                        transformy.erase(transformy.begin());

                        if (cur->get_parent() == NULL)
                        {
                            cur->update_rect(windowRect);
                        }
                        else
                        {
                            cur->update_rect(cur->get_parent()->get_rect());
                        }

                        for (int i = 0; i < cur->get_childCount(); i++)
                        {
                            transformy.push_back(cur->get_child(i));
                        }
                    }
                }
            }
        }

        //clear
        SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
        SDL_RenderClear(renderer);

        {
            //rt.draw(renderer);
            //rt2.draw(renderer);
            
            std::vector<RectTransform*> transformy{&rt};

            while (!transformy.empty())
            {
                RectTransform* cur = transformy.front();
                transformy.erase(transformy.begin());

                cur->draw(renderer);

                auto behaviourImage = cur->get_behaviour<Image>();
                if (behaviourImage != nullptr)
                {
                    //behaviourImage->update(renderer);
                    behaviourImage->update();
                }

                for (int i = 0; i < cur->get_childCount(); i++)
                {
                    transformy.push_back(cur->get_child(i));
                }
            }
        }

        //SDL_RenderTexture(renderer, texture, &textureSrcRect, &textureDstRect);
        //SDL_RenderTextureRotated(renderer, texture, &srcRect, &dstRect, 90.f, NULL, SDL_FLIP_HORIZONTAL);
        //img.update(renderer);


        SDL_RenderPresent(renderer);
        //SDL_Delay(16);
    }

    
    freeAll();


    return 0;
}

