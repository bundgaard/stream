#include <iostream>
#include <SDL2/SDL.h>
#include <cmath>
#include <array>
#include <random>

constexpr int FRAME = 30;

int main()
{
    std::random_device rd;
    std::mt19937 rnd(rd());
    std::uniform_real_distribution distribution;



    SDL_Init(SDL_INIT_VIDEO);
    auto window = SDL_CreateWindow("Stream", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480,
                                   SDL_WINDOW_RESIZABLE);
    auto renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED);


    bool quit = false;
    Uint32 frameMs = 1000 / FRAME;
    Uint32 startMs = SDL_GetTicks();
    Uint32 endMs = SDL_GetTicks();

    Uint32 delayMs = 0;
    std::array<SDL_Rect, 10> rects;

    for(int i = 0; i < rects.size(); i++){
        auto& rect = rects[i];
        rect.w = 32;
        rect.h = 32;
        rect.x = 60 + (i * (32 + 5) );
        rect.y = 10 * i * distribution(rnd);
    }

    for(auto & rect : rects) {
        printf("%d\n", rect.x);
    }


    printf("BEG %d, %d %d %d\n", startMs, endMs, delayMs, frameMs);
    SDL_Rect rect = {50, 50, 64, 64};
    while (!quit)
    {


        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_KEYUP:
                    switch (event.key.keysym.sym)
                    {
                        case SDLK_ESCAPE:
                            quit = true;
                            break;
                    }
            }
        }
        startMs = SDL_GetTicks();

        int idx = 0;
        std::for_each(rects.begin(), rects.end(), [&idx](SDL_Rect &rect) {

            rect.y = 100 + ( sin(SDL_GetTicks() * 0.0009 * M_PI_2 - idx) * 64.0);
            idx++;
        });


        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

        std::for_each(rects.begin(), rects.end(), [renderer](SDL_Rect &rect) {
            SDL_RenderFillRect(renderer, &rect);
        });


        SDL_RenderPresent(renderer);

        endMs = SDL_GetTicks() - startMs;
        if(endMs < frameMs) {
            delayMs = frameMs - (endMs - startMs);

            SDL_Delay(frameMs - endMs);
        }

    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();


    return 0;
}
