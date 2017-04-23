#include <stdio.h>
#include <stdlib.h>
#include "SDL2/SDL.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int FRAME_RATE = 60;

typedef struct {
    SDL_Window* window;
    SDL_Surface* surface;
    const char* error;
} Game;

Game init() {
    SDL_Window* window = NULL;
    SDL_Surface* surface = NULL;
    const char* error = NULL;
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        const char *sdlError = SDL_GetError();
        printf("SDL could not initialize! SDL_Error: %s\n", sdlError);
        error = sdlError;
    } else {
        window = SDL_CreateWindow(
                "SDL Tutorial",
                SDL_WINDOWPOS_UNDEFINED,
                SDL_WINDOWPOS_UNDEFINED,
                SCREEN_WIDTH,
                SCREEN_HEIGHT,
                SDL_WINDOW_SHOWN
        );
        if (window == NULL) {
            const char *sdlError = SDL_GetError();
            printf("Window could not be created! SDL_Error: %s\n", sdlError);
            error = sdlError;
        } else {
            surface = SDL_GetWindowSurface(window);
        }
    }
    Game game;
    game.window = window;
    game.surface = surface;
    game.error = error;
    return game;
}

void closeGame (Game game, int surfaceCount, ...)
{
    SDL_DestroyWindow(game.window);
    va_list ap;
    int i;
    va_start (ap, surfaceCount);
    for (i = 0; i < surfaceCount; i++) {
        SDL_Surface* surf = va_arg (ap, SDL_Surface*);
        SDL_FreeSurface(surf);
    }
    va_end (ap);
    SDL_Quit();
}

int main(int argc, char *args[]) {
    Game game = init();
    if (game.error != NULL) {
        printf( "Unable to initialize game %s", game.error);
        return 1;
    }

    SDL_Surface* helloWorld = SDL_LoadBMP( "assets/hello_world.bmp" );

    // set to 1 when window close button is pressed
    int close_requested = 0;

    // animation loop
    while (!close_requested) {
        // process events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                close_requested = 1;
            }
        }
        if (helloWorld != NULL) {
            SDL_BlitSurface(helloWorld, NULL, game.surface, NULL );
        } else {
            printf( "Unable to load image %s! SDL Error: %s\n", "assets/hello_world.bmp", SDL_GetError() );
            return 2;
        }
        SDL_UpdateWindowSurface(game.window);
        // wait 1/60th of a second
        SDL_Delay(1000/60);
    }

    closeGame(game, 1, helloWorld);

    return 0;
}