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

#define EXIT()                                                                \
  {                                                                           \
    free (snake);                                                             \
    SDL_DestroyRenderer (renderer);                                           \
    SDL_DestroyWindow (window);                                               \
    SDL_Quit ();                                                              \
    return 0;                                                                 \
  }

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

  int points = 0;

  int allocated_max_length = 128;
  SDL_Rect *snake = malloc (sizeof (SDL_Rect) * (allocated_max_length));
  snake[0] = (SDL_Rect){ .x = WIDTH / 2, .y = HEIGHT / 2, .w = 10, .h = 10 };

  SDL_Rect food = (SDL_Rect){
    .x = (int)(rand () % WIDTH), .y = (int)(rand () % HEIGHT), .w = 5, .h = 5
  };

  for (;;)
    {
      SDL_Event event;
      while (SDL_PollEvent (&event))
        {
          switch (event.type)
            {
            case SDL_QUIT:
              EXIT ()
            case SDL_KEYDOWN:
              {
                switch (event.key.keysym.sym)
                  {
                  case SDLK_d:
                    for (int i = 0; i <= points; ++i)
                      snake[i].x = snake[i].x % WIDTH + snake[i].w
                                   + (points - i) * snake[i].w;
                    break;
                  case SDLK_a:
                    for (int i = 0; i <= points; ++i)
                      snake[i].x
                          = (0 < snake[i].x ? snake[i].x - snake[i].w : WIDTH)
                            + (points - i) * snake[i].w;
                    break;
                  case SDLK_w:
                    for (int i = 0; i <= points; ++i)
                      snake[i].y
                          = (0 < snake[i].y ? snake[i].y - snake[i].h : HEIGHT)
                            + (points - i) * snake[i].h;
                    break;
                  case SDLK_s:
                    for (int i = 0; i <= points; ++i)
                      snake[i].y = snake[i].y % HEIGHT + snake[i].h
                                   + (points - i) * snake[i].h;
                    break;
                  case SDLK_ESCAPE:
                    EXIT ()
                    break;
                  }
                break;
              }
            }
        }

      if (abs (food.x - snake[points].x) <= snake[points].w
          && abs (food.y - snake[points].y) <= snake[points].h)
        {
          ++points;
          food.x = (int)(rand () % WIDTH);
          food.y = (int)(rand () % HEIGHT);

          if (points == allocated_max_length)
            {
              allocated_max_length *= 2;
              snake = realloc (snake,
                               sizeof (SDL_Rect) * (allocated_max_length));
            }
          snake[points] = (SDL_Rect){ .x = snake[points - 1].x,
                                      .y = snake[points - 1].y,
                                      .w = 10,
                                      .h = 10 };
        }

      SDL_SetRenderDrawColor (renderer, 0, 0, 0, 0);
      SDL_RenderClear (renderer);

      SDL_SetRenderDrawColor (renderer, 255, 255, 255, 255);

      for (int i = 0; i < points + 1; ++i)
        SDL_RenderFillRect (renderer, &(snake[i]));

      SDL_RenderFillRect (renderer, &food);
      SDL_RenderPresent (renderer);
    }

  return 0;
}
