#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

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

  SDL_Rect snake
      = (SDL_Rect){ .x = WIDTH / 2, .y = HEIGHT / 2, .w = 10, .h = 10 };

  SDL_Rect food = (SDL_Rect){ .x = (int)(rand () % WIDTH),
                              .y = (int)(rand () % HEIGHT),
                              .w = 5,
                              .h = 5 };

  int points = 0;

  for (;;)
    {
      SDL_Event event;
      while (SDL_PollEvent (&event))
        {
          switch (event.type)
            {
            case SDL_QUIT:
              SDL_DestroyRenderer (renderer);
              SDL_DestroyWindow (window);
              SDL_Quit ();
              return 0;
            case SDL_KEYDOWN:
              {
                switch (event.key.keysym.sym)
                  {
                  case SDLK_d:
                    snake.x = snake.x % WIDTH + snake.w;
                    break;
                  case SDLK_a:
                    snake.x = 0 < snake.x ? snake.x - snake.w : WIDTH;
                    break;
                  case SDLK_w:
                    snake.y = 0 < snake.y ? snake.y - snake.h : HEIGHT;
                    break;
                  case SDLK_s:
                    snake.y = snake.y % HEIGHT + snake.h;
                    break;
                  }
                break;
              }
            }
        }

      if (abs (food.x - snake.x) <= snake.w &&
          abs(food.y - snake.y) <= snake.h )
        {
	  ++points; 
	  food.x = (int)(rand () % WIDTH);
	  food.y = (int)(rand () % HEIGHT);
        }

      SDL_SetRenderDrawColor (renderer, 0, 0, 0, 0);
      SDL_RenderClear (renderer);

      SDL_SetRenderDrawColor (renderer, 255, 255, 255, 255);
      SDL_RenderFillRect (renderer, &snake);
      SDL_RenderFillRect (renderer, &food);
      SDL_RenderPresent (renderer);
    }

  return 0;
}
