#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <threads.h>

#define TITLE "NagaSaga"
#define WIDTH 640
#define HEIGHT 480

int
main (void)
{
  if (SDL_Init (SDL_INIT_VIDEO) != 0)
    {
      printf ("SDL_Init Error: %s\n", SDL_GetError ());
      return 1;
    }

  SDL_Window *window = SDL_CreateWindow (TITLE, SDL_WINDOWPOS_UNDEFINED,
                                         SDL_WINDOWPOS_UNDEFINED, WIDTH,
                                         HEIGHT, SDL_WINDOW_RESIZABLE);
  if (window == NULL)
    {
      printf ("SDL_CreateWindow Error: %s\n", SDL_GetError ());
      return 1;
    }

  SDL_Renderer *renderer = SDL_CreateRenderer (window, -1, 0);

  SDL_SetRenderDrawColor (renderer, 255, 255, 255, 255);

  for (;;)
    {
      SDL_Event event;
      while (SDL_PollEvent (&event))
        {
          if (event.type == SDL_QUIT)
            {
              SDL_DestroyRenderer (renderer);
              SDL_DestroyWindow (window);
              SDL_Quit ();
              return 0;
            }
        }

      SDL_RenderPresent (renderer);
    }

  return 0;
}
