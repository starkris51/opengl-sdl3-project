#include "app.h"

int main()
{
    try
    {
        App app;
        if (!app.init())
        {
            return -1;
        }

        app.run();
        app.shutdown();
    }
    catch (const std::exception &e)
    {
        SDL_Log("Unhandled exception: %s", e.what());
        return -1;
    }
    catch (...)
    {
        SDL_Log("Unhandled unknown exception");
        return -1;
    }
    return 0;
}