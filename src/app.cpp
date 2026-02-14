#include "app.h"
#include <GL/gl.h>

bool App::init()
{
    SDL_SetLogPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO);

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    window = SDL_CreateWindow("RaycastFPS", 800, 600,
                              SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    if (!window)
    {
        SDL_Log("SDL_CreateWindow failed: %s", SDL_GetError());
        SDL_Quit();
        return false;
    }

    glContext = SDL_GL_CreateContext(window);
    if (!glContext)
    {
        SDL_Log("SDL_GL_CreateContext failed: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }

    SDL_GL_SetSwapInterval(1);
    SDL_ShowWindow(window);

    running = true;
    return true;
}

void App::run()
{
    fpsLastTicks = SDL_GetTicks();

    while (running)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            handleEvent(e);
        }

        render();
        SDL_GL_SwapWindow(window);

        fpsFrames++;
        Uint64 now = SDL_GetTicks();
        if (now - fpsLastTicks >= 1000)
        {
            fpsValue = (fpsFrames * 1000.0f) / float(now - fpsLastTicks);
            fpsFrames = 0;
            fpsLastTicks = now;

            SDL_snprintf(title, sizeof(title), "RaycastFPS - %.1f FPS", fpsValue);
            SDL_SetWindowTitle(window, title);
        }
    }
}

void App::shutdown()
{
    if (glContext)
        SDL_GL_DestroyContext(glContext);
    if (window)
        SDL_DestroyWindow(window);
    SDL_Quit();
}

void App::handleEvent(const SDL_Event &e)
{
    if (e.type == SDL_EVENT_QUIT)
        running = false;
}

void App::render()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}