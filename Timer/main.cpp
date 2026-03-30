
//#include <iostream>
#include <cstdio>
#include <filesystem>
#include <string>

#include "SDL3/SDL.h"
//#include "SDL3_image/SDL_image.h"

#include "lua.hpp"

#include "core.hpp"
#include "context.hpp"



static int initPathExe()
{
    Context::pathExe = SDL_GetBasePath();

    if (Context::pathExe)
    {
        printf("Context::pathExe: %s\n", Context::pathExe);
        return 0;
    }
    else
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_GetBasePath failed: %s", SDL_GetError());
        return 1;
    }
}


static int createWindow()
{
    //SDL_SetHint(SDL_HINT_VIDEO_DRIVER, "x11");

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        //SDL_Log("SDL_Init failed: %s", SDL_GetError());
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_Init failed: %s", SDL_GetError());

        return 1;
    }

    //Context::window = SDL_CreateWindow("Timer", 800, 600, SDL_WINDOW_RESIZABLE);
    //Context::renderer = SDL_CreateRenderer(Context::window, NULL);

    if (!SDL_CreateWindowAndRenderer(
        "Timer",
        800, 600,
        SDL_WINDOW_RESIZABLE,
        &Context::window, &Context::renderer))
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window or renderer or both %s", SDL_GetError());

        return 1;
    }

    //if (!Context::window)
    //{
    //    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window %s", SDL_GetError());
    // 
    //    return 1;
    //}

    //if (!Context::renderer)
    //{
    //    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create renderer %s", SDL_GetError());
    // 
    //    return 1;
    //}

    return 0;
}

static int createLua()
{
    Context::L = luaL_newstate();

    //luaL_openlibs(L);

    //luaopen_base(L);
    //lua_setglobal(L, "_G");

    luaL_requiref(Context::L, "_G", luaopen_base, 1);
    lua_pop(Context::L, 1);

    luaL_requiref(Context::L, "math", luaopen_math, 1);
    lua_pop(Context::L, 1);

    register_API(Context::L);

    return 0;
}

static int executeLua()
{
    std::string pathScriptsFull{Context::pathExe};
    pathScriptsFull.append(Context::pathScripts);
    printf("pathScriptsFull: %s\n", pathScriptsFull.c_str());

    std::filesystem::path p{pathScriptsFull};

    if (!std::filesystem::exists(p) || !std::filesystem::is_directory(p))
    {
        printf("pathScriptsFull je nevalidna\n");
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

            if (luaL_dofile(Context::L, entry.path().string().c_str()) != LUA_OK)
            {
                printf("Error: %s\n", lua_tostring(Context::L, -1));
                    
                return 1;
            }
        }
    }

    return 0; 
}


static void freeAll()
{
    if (Context::L)
    {
        lua_close(Context::L);
        Context::L = nullptr;
    }

    //nemozem uvolnit pathToExe, lebo sa uvolnuje v SDL_Quit()
    //if (Context::pathExe)
    //{
    //    SDL_free((void*)Context::pathExe);
    //    Context::pathExe = nullptr;
    //}

    if (Context::renderer)
    {
        SDL_DestroyRenderer(Context::renderer);
        Context::renderer = nullptr;
    }

    if (Context::window)
    {
        SDL_DestroyWindow(Context::window);
        Context::window = nullptr;
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

    if (auto result = freeIfNotReturn0(initPathExe)) return result;
    if (auto result = freeIfNotReturn0(createWindow)) return result;
    if (auto result = freeIfNotReturn0(createLua)) return result;
    if (auto result = freeIfNotReturn0(executeLua)) return result;

    SDL_FRect windowRect{};
    int winW, winH;
    SDL_GetWindowSize(Context::window, &winW, &winH);
    windowRect.x = 0;
    windowRect.y = 0;
    windowRect.w = static_cast<float>(winW);
    windowRect.h = static_cast<float>(winH);
    

    //Image img{ &rt, texturePikachu };
    //rt.add_behaviour(&img);

    //auto img = rt2.add_behaviour<Image>(texturePikachu);
    //img->preserveAspectRation = true;
    //img->alignHorizontal = ImageAlignHorizontal::CENTER;
    //img->alignVertical = ImageAlignVertical::CENTER;


    {
        std::vector<RectTransform*> transformy{SceneManager::get_activeScene()->get_content()};

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
                    std::vector<RectTransform*> transformy{SceneManager::get_activeScene()->get_content()};

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
        SDL_SetRenderDrawColor(Context::renderer, 20, 20, 20, 255);
        SDL_RenderClear(Context::renderer);

        {
            std::vector<RectTransform*> transformy{SceneManager::get_activeScene()->get_content()};

            while (!transformy.empty())
            {
                RectTransform* cur = transformy.front();
                transformy.erase(transformy.begin());

                cur->draw(Context::renderer);

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


        SDL_RenderPresent(Context::renderer);
        SDL_Delay(16);
    }

    
    freeAll();


    return 0;
}

