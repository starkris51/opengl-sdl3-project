#pragma once
#include <SDL3/SDL.h>
#include <exception>

class App
{
public:
    bool init();
    void run();
    void shutdown();

private:
    void handleEvent(const SDL_Event &e);
    void render();

    SDL_Window *window = nullptr;
    SDL_GLContext glContext = nullptr;
    bool running = false;
    char title[128];

    Uint64 fpsLastTicks = 0;
    int fpsFrames = 0;
    float fpsValue = 0.0f;
};