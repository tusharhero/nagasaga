#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <stdio.h>

#define TITLE "NagaSaga"
#define WIDTH 640
#define HEIGHT 480

#define EXIT()                                                                \
  {                                                                           \
    SDL_DestroyRenderer (renderer);                                           \
    SDL_DestroyWindow (window);                                               \
    SDL_Quit ();                                                              \
    return 0;                                                                 \
  }

typedef struct
{
  double x;
  double y;
} Vector;

void
move_rect (SDL_Rect *rect, Vector velocity)
{
  rect->x = rect->x + velocity.x;
  rect->y = rect->y + velocity.y;

  if (rect->x > WIDTH)
    {
      rect->x = rect->x % WIDTH;
    }
  if (rect->x < 0)
    {
      rect->x = WIDTH - rect->w;
    }
  if (rect->y > HEIGHT)
    {
      rect->y = rect->y % HEIGHT;
    }
  if (rect->y < 0)
    {
      rect->y = HEIGHT - rect->h;
    }
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

  int score = 0;

  typedef struct
  {
    SDL_Rect head;
    Vector velocity;
    size_t length;
  } Snake;

  Snake snake = (Snake){
    .head = (SDL_Rect){ .x = WIDTH / 2, .y = HEIGHT / 2, .w = 10, .h = 10 },
    .velocity = (Vector){ .x = 0, .y = 0 },
    .length = score,
  };

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
                    snake.velocity = (Vector){ .x = 1, .y = 0 };
                    break;
                  case SDLK_a:
                    snake.velocity = (Vector){ .x = -1, .y = 0 };
                    break;
                  case SDLK_w:
                    snake.velocity = (Vector){ .x = 0, .y = -1 };
                    break;
                  case SDLK_s:
                    snake.velocity = (Vector){ .x = 0, .y = 1 };
                    break;
                  case SDLK_ESCAPE:
                    EXIT ()
                    break;
                  }
                break;
              }
            }
        }

      move_rect (&snake.head, snake.velocity);

      if (abs (food.x - snake.head.x) <= snake.head.w
          && abs (food.y - snake.head.y) <= snake.head.h)
        {
          food.x = (int)(rand () % WIDTH);
          food.y = (int)(rand () % HEIGHT);

          snake.length = (++score);
        }

      SDL_SetRenderDrawColor (renderer, 0, 0, 0, 0);
      SDL_RenderClear (renderer);

      SDL_SetRenderDrawColor (renderer, 255, 255, 255, 255);

      SDL_RenderFillRect (renderer, &(snake.head));

      SDL_RenderFillRect (renderer, &food);
      SDL_RenderPresent (renderer);
    }

  return 0;
}
