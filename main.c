#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <stdio.h>
#include <stdlib.h>

#define TITLE "NagaSaga"
#define WIDTH 640
#define HEIGHT 480

typedef struct
{
  double x;
  double y;
} Vector;

typedef struct
{
  size_t allocation_size;
  size_t length;
  Vector *vectors;
} VectorArray;

typedef struct
{
  VectorArray body;
  Vector direction;
} Snake;

int
exit_program (SDL_Renderer *renderer, SDL_Window *window, Snake snake)
{
  free (snake.body.vectors);
  SDL_DestroyRenderer (renderer);
  SDL_DestroyWindow (window);
  SDL_Quit ();
  return 0;
}

Vector
bound_pos (Vector pos)
{
  Vector bounded_pos = pos;
  if (pos.x > WIDTH)
    {
      bounded_pos.x = (int)pos.x % WIDTH;
    }
  if (pos.x < 0)
    {
      bounded_pos.x = WIDTH + pos.x;
    }
  if (pos.y > HEIGHT)
    {
      bounded_pos.y = (int)pos.y % HEIGHT;
    }
  if (pos.y < 0)
    {
      bounded_pos.y = HEIGHT + pos.y;
    }
  return bounded_pos;
}

Vector
move_pos (Vector pos, Vector direction, Vector step_size, double delta)
{
  return (Vector){
    pos.x + direction.x * step_size.x * delta,
    pos.y + direction.y * step_size.y * delta,
  };
}

Vector
get_snake_head (Snake snake)
{
  return snake.body.vectors[0];
}

SDL_Rect *
get_rect (Vector position, Vector size)
{
  return &(SDL_Rect){
    .x = position.x,
    .y = position.y,
    .w = size.x,
    .h = size.y,
  };
}

void
render_snake (SDL_Renderer *renderer, Snake snake, Vector blocksize)
{
  for (size_t i = 0; i < snake.body.length; ++i)
    {
      SDL_RenderFillRect (renderer,
                          get_rect (snake.body.vectors[i], blocksize));
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
  Vector snake_block_size = { .x = 10, .y = 10 };

  Snake snake = (Snake){
    .direction = (Vector){ .x = 0, .y = 0 },
    .body = (VectorArray){ .allocation_size = 128,
                           .length = score + 1,
                           .vectors = malloc (sizeof (Vector)
                                              * snake.body.allocation_size) }
  };

  snake.body.vectors[0] = (Vector){
    .x = WIDTH / 2,
    .y = HEIGHT / 2,
  };

  SDL_Rect food = (SDL_Rect){
    .x = (int)(rand () % WIDTH),
    .y = (int)(rand () % HEIGHT),
    .w = 5,
    .h = 5,
  };

  double last_tick_time = 0;
  double tick_time = SDL_GetTicks ();
  double delta = 0;
  int frame = 0;
  for (;;)
    {
      tick_time = SDL_GetTicks64 ();
      delta = tick_time - last_tick_time;
      last_tick_time = tick_time;

      SDL_Event event;
      while (SDL_PollEvent (&event))
        {
          switch (event.type)
            {
            case SDL_QUIT:
              return exit_program (renderer, window, snake);
              break;
            case SDL_KEYDOWN:
              {
                switch (event.key.keysym.sym)
                  {
                  case SDLK_d:
                    snake.direction = (Vector){ .x = +1 };
                    break;
                  case SDLK_a:
                    snake.direction = (Vector){ .x = -1 };
                    break;
                  case SDLK_w:
                    snake.direction = (Vector){ .y = -1 };
                    break;
                  case SDLK_s:
                    snake.direction = (Vector){ .y = +1 };
                    break;
                  case SDLK_ESCAPE:
                    return exit_program (renderer, window, snake);
                    break;
                  }
                break;
              }
            }
        }

      Vector penul_pos, anti_penul_pos;
      for (size_t i = 0; i <= snake.body.length; ++i)
        {
          penul_pos = snake.body.vectors[i];
          if (i == 0)
            {
              snake.body.vectors[i] = bound_pos (
                  move_pos (penul_pos, snake.direction,
                            (Vector){ .x = 10, .y = 10 }, delta / 1000));
            }
          else
            {
              snake.body.vectors[i] = anti_penul_pos;
            }
          anti_penul_pos = penul_pos;
        }

      if (abs (food.x - get_snake_head (snake).x) <= snake_block_size.x
          && abs (food.y - get_snake_head (snake).y) <= snake_block_size.y)
        {
          food.x = (int)(rand () % WIDTH);
          food.y = (int)(rand () % HEIGHT);

          snake.body.length = 1 + (++score);
        }

      SDL_SetRenderDrawColor (renderer, 0, 0, 0, 0);
      SDL_RenderClear (renderer);

      SDL_SetRenderDrawColor (renderer, 255, 255, 255, 255);

      render_snake (renderer, snake, snake_block_size);

      SDL_RenderFillRect (renderer, &food);
      SDL_RenderPresent (renderer);

      ++frame;
    }

  return 0;
}
