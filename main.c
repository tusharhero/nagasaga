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

#define EXIT()                                                                \
  {                                                                           \
    SDL_DestroyRenderer (renderer);                                           \
    SDL_DestroyWindow (window);                                               \
    SDL_Quit ();                                                              \
    return 0;                                                                 \
  }

typedef struct
{
  int x;
  int y;
} Vector;

typedef struct
{
  int allocation_size;
  int length;
  Vector *vectors;
} VectorArray;

typedef struct
{
  SDL_Rect head;
  Vector direction;
  VectorArray turning_points;
  size_t length;
} Snake;

void
set_turning_point (Snake *snake)
{
  if (snake->turning_points.length <= snake->turning_points.allocation_size)
    {
      snake->turning_points.allocation_size *= 2;
      snake->turning_points.vectors
          = realloc (snake->turning_points.vectors,
                     snake->turning_points.allocation_size * sizeof (Vector));
    }
  snake->turning_points.vectors[snake->turning_points.length++]
      = (Vector){ .x = snake->head.x, .y = snake->head.y };
}

void
bound_rect (SDL_Rect *rect)
{

  if (rect->x > WIDTH)
    {
      rect->x = rect->x % WIDTH;
    }
  if (rect->x < 0)
    {
      rect->x = WIDTH + rect->x;
    }
  if (rect->y > HEIGHT)
    {
      rect->y = rect->y % HEIGHT;
    }
  if (rect->y < 0)
    {
      rect->y = HEIGHT + rect->y;
    }
}

void
move_rect (SDL_Rect *rect, Vector direction)
{
  rect->x = rect->x + direction.x;
  rect->y = rect->y + direction.y;

  bound_rect (rect);
}

void
render_snake (SDL_Renderer *renderer, Snake snake)
{
  SDL_RenderFillRect (renderer, &(snake.head));
  SDL_Rect *temp_rect;
  for (size_t i = 0; i <= snake.length; ++i)
    {
      temp_rect = &((SDL_Rect){
          .x = snake.head.x - i * snake.direction.x * snake.head.w,
          .y = snake.head.y - i * snake.direction.y * snake.head.h,
          .w = snake.head.w,
          .h = snake.head.h,
      });
      bound_rect (temp_rect);
      SDL_RenderFillRect (renderer, temp_rect);
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

  Snake snake = (Snake){ .head = (SDL_Rect){ .x = WIDTH / 2,
                                             .y = HEIGHT / 2,
                                             .w = 10,
                                             .h = 10 },
                         .direction = (Vector){ .x = 0, .y = 0 },
                         .length = score,
                         .turning_points = (VectorArray){
                             .allocation_size = 1,
                             .length = 0,
                             .vectors
                             = malloc (sizeof (Vector)
                                       * snake.turning_points.allocation_size),
                         } };

  SDL_Rect food = (SDL_Rect){
    .x = (int)(rand () % WIDTH),
    .y = (int)(rand () % HEIGHT),
    .w = 5,
    .h = 5,
  };

  int frame = 0;
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
                    snake.direction = (Vector){ .x = +1 };
                    set_turning_point (&snake);
                    break;
                  case SDLK_a:
                    snake.direction = (Vector){ .x = -1 };
                    set_turning_point (&snake);
                    break;
                  case SDLK_w:
                    snake.direction = (Vector){ .y = -1 };
                    set_turning_point (&snake);
                    break;
                  case SDLK_s:
                    snake.direction = (Vector){ .y = +1 };
                    set_turning_point (&snake);
                    break;
                  case SDLK_ESCAPE:
                    EXIT ()
                    break;
                  }
                break;
              }
            }
        }

      if (!(frame % 20))
        {
          move_rect (&snake.head, snake.direction);
        }

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

      render_snake (renderer, snake);

      SDL_RenderFillRect (renderer, &food);
      SDL_RenderPresent (renderer);

      ++frame;
    }

  return 0;
}
