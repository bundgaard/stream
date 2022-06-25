#include <iostream>

#include <cmath>
#include <array>
#include <random>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

constexpr int FRAME = 30;

static void scc(int errorCode)
{
    if (errorCode < 0)
    {
        fprintf(stderr, "failed with %s\n", SDL_GetError());
        exit(1);
    }
}

static void *scp(void *ptr)
{
    if (ptr == nullptr)
    {
        fprintf(stderr, "failed with %s\n", SDL_GetError());
        exit(1);
    }
    return ptr;
}
static int currentPos = 0;

static void samples(void *data, Uint8 *stream, int len)
{
    currentPos += len;
    printf("Len=%d, Current %d\n",len, currentPos / len);
}


struct Snake {
    int x;
    int y;
    int w;
    int h;
    int len;
};


int main()
{
    std::random_device rd;
    std::mt19937 rnd(rd());
    std::uniform_real_distribution distribution;

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    scc(Mix_Init(MIX_INIT_MOD));
    scc(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 128));


    Mix_SetPostMix(samples, nullptr);
    auto *music = (Mix_Music *) scp(Mix_LoadMUS("/home/dbundgaard/code/stream/shadows_in_dark.mod"));
    auto window = SDL_CreateWindow("Stream", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                   640, 480,
                                   SDL_WINDOW_RESIZABLE);
    auto renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED);


    bool quit = false;
    Uint32 frameMs = 1000 / FRAME;
    Uint32 startMs = SDL_GetTicks();
    Uint32 endMs = SDL_GetTicks();

    Uint32 delayMs = 0;
    std::array<SDL_Rect, 10> rects{};

    for (int i = 0; i < rects.size(); i++)
    {
        auto &rect = rects[i];
        rect.w = 32;
        rect.h = 32;
        rect.x = 60 + (i * (32 + 5));
        rect.y = 10 * i * distribution(rnd);
    }

    Snake s{};
    s.w = 64;
    s.h = 64;
    s.x = 10;
    s.y = 10;

    SDL_Rect rect = {50, 50, 64, 64};
    Mix_PlayMusic(music, 1);

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
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym)
                    {
                        case SDLK_UP:
                            s.y--;
                            break;
                        case SDLK_DOWN:
                            s.y++;
                            break;
                        case SDLK_LEFT:
                            s.x--;
                            break;
                        case SDLK_RIGHT:
                            s.x++;
                            break;
                    }
                    break;
            }
        }
        startMs = SDL_GetTicks();



        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

        SDL_Rect snakeRect{s.x, s.y, s.w, s.h};
        SDL_RenderFillRect(renderer, &snakeRect);

        SDL_RenderPresent(renderer);

        endMs = SDL_GetTicks() - startMs;
        if (endMs < frameMs)
        {
            delayMs = frameMs - (endMs - startMs);

            SDL_Delay(frameMs - endMs);
        }

    }

    Mix_HaltMusic();
    Mix_FreeMusic(music);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    Mix_Quit();
    SDL_Quit();


    return 0;
}
