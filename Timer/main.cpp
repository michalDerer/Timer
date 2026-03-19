
#include <iostream>
#include <filesystem>
#include <string>

#include "SDL3/SDL.h"
#include "SDL3_image/SDL_image.h"

#include "rectTransform.hpp"



//------------------------------------------------------------------------------------------------------------


const char* pathToExe = NULL;


//------------------------------------------------------------------------------------------------------------


void calculate_rect(float texAsp, float winW, float winH, SDL_FRect& rect)
{
    if (winH * texAsp <= winW)
    {
        rect.w = winH * texAsp;
        rect.h = winH;
        rect.x = (winW / 2.0f) - (rect.w / 2.0f);
        rect.y = 0;
    }
    else
    {
        rect.w = winW;
        rect.h = winW / texAsp;
        rect.x = 0;
        rect.y = (winH / 2.0f) - (rect.h / 2.0f);
    }
}


void init_root_path(int& argc, char**& argv)
{
    std::cout << "argc: " << argc << "\n";
    
    for (int i = 0; i < argc; i++)
    {
        std::cout << "argv["<< i <<"]: " << argv[i] << "\n";
    }

    std::cout << "std::filesystem::current_path: " << std::filesystem::current_path() << "\n";

    pathToExe = SDL_GetBasePath();
    std::cout << "SDL_GetBasePath: " << pathToExe << "\n";

}


//------------------------------------------------------------------------------------------------------------


int main(int argc, char** argv)
{
    
    //SDL_SetHint(SDL_HINT_VIDEO_DRIVER, "x11");

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return 1;
    }

    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    // SDL_Window* window = SDL_CreateWindow(
    //     "Timer",
    //     800, 600,
    //     SDL_WINDOW_RESIZABLE);

    // SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);

    if(!SDL_CreateWindowAndRenderer(
        "Timer",
        800, 600,
        SDL_WINDOW_RESIZABLE,
        &window, &renderer))
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window or renderer or both %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // if (!window)
    // {
    //     SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window %s", SDL_GetError());
    //     return 1;
    // }

    // if (!renderer)
    // {
    //     SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create renderer %s", SDL_GetError());
    //     return 1;
    // }

    init_root_path(argc, argv);

    SDL_Surface* surface = IMG_Load( std::string(pathToExe).append("/pika.png").c_str() );

    if (!surface)
    {
        SDL_Log("IMG_Load failed: %s", SDL_GetError());
        SDL_free((void*)pathToExe);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);
    surface = NULL;

    if (!texture)
    {
        SDL_Log("Failed to create texture: %s", SDL_GetError());
        SDL_free((void*)pathToExe);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    float texW, texH;
    SDL_GetTextureSize(texture, &texW, &texH);
    
    int winW, winH;
    SDL_GetWindowSize(window, &winW, &winH);

    SDL_FRect windowRect {.x = 0, .y = 0, .w = static_cast<float>(winW), .h = static_cast<float>(winH)};

    SDL_FRect textureSrcRect;
    textureSrcRect.x = 0.0f;
    textureSrcRect.y = 0.0f;
    textureSrcRect.w = texW;
    textureSrcRect.h = texH;
    float texAsp = texW / texH;

    SDL_FRect textureDstRect;
    calculate_rect(texAsp, winW, winH, textureDstRect);

    RectTransform rt;
    rt.set_anchorMinX(0.1f);
    rt.set_anchorMaxX(0.9f);
    rt.set_anchorMinY(0.1f);
    rt.set_anchorMaxY(0.9f);
    rt.set_right(1.f);
    rt.set_bottom(1.f);
    RectTransform* rt2 = rt.create_child();
    rt2->set_anchorMinX(0.2f);
    rt2->set_anchorMaxX(0.8f);
    rt2->set_anchorMinY(0.2f);
    rt2->set_anchorMaxY(0.8f);
    rt2->set_right(1.f);
    rt2->set_bottom(1.f);
    
    bool done = false;  

    while(!done)
    {
        SDL_Event event;

        while(SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                done = true;
                std::cout << "EVENT_QUIT\n";
            }
            else if (event.type == SDL_EVENT_WINDOW_RESIZED)
            {
                winW = event.window.data1;
                winH = event.window.data2;
                
                std::cout << "EVENT_WINDOW_RESIZED w: " << winW << "h: " << winH << "\n";

                windowRect.x = 0;
                windowRect.y = 0;
                windowRect.w = static_cast<float>(winW);
                windowRect.h = static_cast<float>(winH);

                calculate_rect(texAsp, winW, winH, textureDstRect);
            }
        }

        //clear
        SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
        SDL_RenderClear(renderer);


        {
            //rt.update_rect(windowRect);
            //rt2->update_rect(rt.get_rect());

            std::vector<RectTransform*> transformy{&rt};

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

                for (int i = 0; i < cur->get_child_count(); i++)
                {
                    transformy.push_back(cur->get_child(i));
                }
            }
        }

        {
            //rt.draw(renderer);
            //rt2->draw(renderer);
            
            std::vector<RectTransform*> transformy{&rt};

            while (!transformy.empty())
            {
                RectTransform* cur = transformy.front();
                transformy.erase(transformy.begin());

                cur->draw(renderer);

                for (int i = 0; i < cur->get_child_count(); i++)
                {
                    transformy.push_back(cur->get_child(i));
                }
            }
        }

        SDL_RenderTexture(renderer, texture, &textureSrcRect, &textureDstRect);
        // SDL_RenderTextureRotated(renderer, texture, &srcRect, &dstRect, 90.f, NULL, SDL_FLIP_HORIZONTAL);
        
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }


    SDL_free((void*)pathToExe);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}

