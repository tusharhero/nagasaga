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

typedef struct
{
  SDL_Rect *body;
  int length;
} Snake;

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

  Snake *snake = malloc (sizeof (Snake));
  snake->length = points + 1;
  snake->body = malloc (sizeof (SDL_Rect) * points);
  snake->body[0]
      = (SDL_Rect){ .x = WIDTH / 2, .y = HEIGHT / 2, .w = 10, .h = 10 };

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
              SDL_DestroyRenderer (renderer);
              SDL_DestroyWindow (window);
              SDL_Quit ();
              free (snake->body);
              free (snake);
              return 0;
            case SDL_KEYDOWN:
              {
                switch (event.key.keysym.sym)
                  {
                  case SDLK_d:
                    snake->body[snake->length].x
                        = snake->body[points].x % WIDTH
                          + snake->body[points].w;
                    break;
                  case SDLK_a:
                    snake->body[snake->length].x
                        = 0 < snake->body[points].x
                              ? snake->body[points].x - snake->body[points].w
                              : WIDTH;
                    break;
                  case SDLK_w:
                    snake->body[snake->length].y
                        = 0 < snake->body[points].y
                              ? snake->body[points].y - snake->body[points].h
                              : HEIGHT;
                    break;
                  case SDLK_s:
                    snake->body[snake->length].y
                        = snake->body[points].y % HEIGHT
                          + snake->body[points].h;
                    break;
                  }
                break;
              }
            }
        }

      if (abs (food.x - snake->body[points].x) <= snake->body[points].w
          && abs (food.y - snake->body[points].y) <= snake->body[points].h)
        {
          ++points;
          food.x = (int)(rand () % WIDTH);
          food.y = (int)(rand () % HEIGHT);

          snake->length = points + 1;
          snake->body
              = realloc (snake->body, sizeof (SDL_Rect) * snake->length);
        }

      SDL_SetRenderDrawColor (renderer, 0, 0, 0, 0);
      SDL_RenderClear (renderer);

      SDL_SetRenderDrawColor (renderer, 255, 255, 255, 255);
      for (int i = 0; i < snake->length; ++i)
        {
          SDL_RenderFillRect (renderer, &(snake->body[i]));
        }
      SDL_RenderFillRect (renderer, &food);
      SDL_RenderPresent (renderer);
    }

  return 0;
}
