using namespace std;

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <unistd.h>
#include <stdlib.h>
#include <string>

#ifndef STANDALONE
#include <emscripten.h>
#endif

#ifdef STANDALONE
string pathPrefix = "../"; // NOLINT(cert-err58-cpp)
#endif
#ifndef STANDALONE
string pathPrefix = "./";
#endif


/**
 * Loads the image located at 'fileName' and copies it to the
 * renderer 'renderer'
 */
int testImage(SDL_Renderer* renderer, const string &fileName)
{
  SDL_Surface *image = IMG_Load(fileName.c_str());
  if (!image)
  {
     printf("IMG_Load: %s\n", IMG_GetError());
     return 0;
  }
  int result = image->w;

  /**
   * position and size that you wish the image to be copied
   * to on the renderer:
   */
  SDL_Rect dest = {.x = 200, .y = 100, .w = 200, .h = 200};

  SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, image);

  SDL_RenderCopy (renderer, tex, nullptr, &dest);

  /**
   * Now that the image data is in the renderer, we can free the memory
   * used by the texture and the image surface
   */
  SDL_DestroyTexture (tex);

  SDL_FreeSurface (image);

  return result;
}

int main()
{
  SDL_Init(SDL_INIT_VIDEO);

  SDL_Window *window;
  SDL_Renderer *renderer;

  SDL_CreateWindowAndRenderer(600, 400, 0, &window, &renderer);

  int result = 0;

  /**
   * Set up a white background
   */
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderClear(renderer);

  /**
   * Load and copy the test image to the renderer
   */
  result |= testImage(renderer, pathPrefix + "assets/images/owl.png");

  /**
   * Show what is in the renderer
   */
  SDL_RenderPresent(renderer);

  printf("you should see an image.\n");

//  while (true) {
//
//  }

  return 0;
}

