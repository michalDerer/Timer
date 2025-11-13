
//#include <iostream>

#include "SDL3/SDL.h"
#include "SDL3/SDL_main.h"
#include "SDL3_image/SDL_image.h"


int main()
{

  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    SDL_Log("SDL_Init failed: %s", SDL_GetError());
    return 1;
  }

  if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) == 0)
  {
    SDL_Log("IMG_Init failed: %s", IMG_GetError());
  }


  SDL_Window* window = SDL_CreateWindow(
    "Timer",
    800,
    600,
    //SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    0
  );

  SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL)


  if (!window || !renderer)
  {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window/renderer %s", SDL_GetError());
    return 1;
  }


  SDL_Texture* texture = IMG_LoadTexture(renderer,"image.png");

  if (!texture)
  {
    SDL_Log("Failed to load image: %s", SDL_GetError());
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

    
    SDL_RenderClear(renderer);
    SDL_RenderTexture(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
    SDL_Delay(16);

  }

  SDL_DestroyTexture(texture);
  SDL)DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  
  IMG_Quit;
  SDL_Quit();

  return 0;
}

