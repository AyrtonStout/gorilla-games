
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <unistd.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <thread>
#include "game_cursor.h"
#include "direction.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

using namespace std;
// This is some hacky dev-environment nonsense
#ifndef __EMSCRIPTEN__
const string pathPrefix = "../"; // NOLINT(cert-err58-cpp)
#endif
#ifdef __EMSCRIPTEN__
const string pathPrefix = "./";
#endif


struct context
{
    SDL_Renderer *renderer;

    /**
     * Rectangle that the owl texture will be rendered into
     */
    SDL_Rect dest;
    SDL_Texture *owl_tex;
    GameCursor game_cursor;
    bool loop_forever = true;
};

/**
 * Loads the image located at 'fileName' and copies it to the
 * renderer 'renderer'
 */
int testImage(context *ctx, const string &fileName) {
  SDL_Surface *image = IMG_Load(fileName.c_str());
  if (!image)
  {
     printf("IMG_Load: %s\n", IMG_GetError());
     return 0;
  }
  int result = image->w;

  // Apparently emcc is unhappy without this cast, but gcc is cool with it
  ctx->dest = (struct SDL_Rect) {.x = 200, .y = 100, .w = 200, .h = 200};
  ctx->owl_tex = SDL_CreateTextureFromSurface(ctx->renderer, image);
  ctx->dest.w = image->w;
  ctx->dest.h = image->h;

  SDL_RenderCopy(ctx->renderer, ctx->owl_tex, nullptr, &ctx->dest);

  SDL_FreeSurface (image);

  return result;
}

void process_keypress(SDL_Event event, GameCursor *cursor) {
    switch (event.key.keysym.sym)
        {
            case SDLK_UP:
                cursor->move(UP); break;
            case SDLK_DOWN:
                cursor->move(DOWN); break;
            case SDLK_LEFT:
                cursor->move(LEFT); break;
            case SDLK_RIGHT:
                cursor->move(RIGHT); break;
            default:
                break;
        }
}

void process_input(context *ctx) {

    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_KEYDOWN:
                process_keypress(event, &ctx->game_cursor); break;
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_CLOSE) {
                    ctx->loop_forever = false;
                }
        }
    }
}

void loop_fn(void* arg) {
    auto *ctx = (context*) arg;

    process_input(ctx);

    ctx->dest.x = 200 + ctx->game_cursor.get_x();
    ctx->dest.y = 100 + ctx->game_cursor.get_y();

    SDL_RenderClear(ctx->renderer);
    SDL_RenderCopy(ctx->renderer, ctx->owl_tex, nullptr, &ctx->dest);
    SDL_RenderPresent(ctx->renderer);

#ifdef __EMSCRIPTEN__
    if (!ctx->loop_forever) {
        emscripten_cancel_main_loop();
    }
#endif
}

int main()
{
  SDL_Init(SDL_INIT_VIDEO);

  SDL_Window *window;

  struct context ctx;
  ctx.dest.x = 200;
  ctx.dest.y = 100;

  SDL_CreateWindowAndRenderer(600, 400, 0, &window, &ctx.renderer);

  // Set up a white background
  SDL_SetRenderDrawColor(ctx.renderer, 255, 255, 255, 255);
  SDL_RenderClear(ctx.renderer);

  ctx.game_cursor = GameCursor();

  // Load and copy the test image to the renderer
  testImage(&ctx, pathPrefix + "assets/images/owl.png");

  // Show what is in the renderer
  SDL_RenderPresent(ctx.renderer);

  printf("you should see an image.\n");

#ifdef __EMSCRIPTEN__
  // If using emscripten, have our main loop be browser friendly
  emscripten_set_main_loop_arg(loop_fn, &ctx, -1, 1);
  return 0;
#endif

#ifndef __EMSCRIPTEN__
  // Otherwise, do a janky normal loop
  while (ctx.loop_forever) {
      loop_fn(&ctx);
      std::this_thread::sleep_for(std::chrono::milliseconds(16));
  }
#endif

  return 0;
}

