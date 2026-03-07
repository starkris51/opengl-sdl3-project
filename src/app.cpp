#define GL_GLEXT_PROTOTYPES
#include "app.h"
#include <GL/gl.h>
#include <GL/glext.h>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <string>

App::App()
{
    SDL_snprintf(title, sizeof(title), "OpenGL SDL3 App");
    init();
}

App::~App()
{
    shutdown();
}

namespace
{
    std::string readTextFile(const char *path)
    {
        std::ifstream file(path);
        if (!file.is_open())
        {
            throw std::runtime_error(std::string("Failed to open shader file: ") + path);
        }

        std::ostringstream ss;
        ss << file.rdbuf();
        return ss.str();
    }
}

void App::init()
{
    SDL_SetLogPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO);

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        throw std::runtime_error(SDL_GetError());
    }
    sdlInitialized = true;

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    window = SDL_CreateWindow(title, 800, 600,
                              SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    if (!window)
    {
        shutdown();
        throw std::runtime_error(SDL_GetError());
    }

    glContext = SDL_GL_CreateContext(window);
    if (!glContext)
    {
        shutdown();
        throw std::runtime_error(SDL_GetError());
    }

    SDL_GL_SetSwapInterval(1);
    SDL_ShowWindow(window);

    glViewport(0, 0, 800, 600);

    const float vertices[] = {
        -0.5f, -0.5f,
        0.5f, -0.5f,
        0.0f, 0.5f};

    const std::string vertexSrcStr = readTextFile("assets/shaders/triangle.vert");
    const std::string fragmentSrcStr = readTextFile("assets/shaders/triangle.frag");
    const char *vertexSrc = vertexSrcStr.c_str();
    const char *fragmentSrc = fragmentSrcStr.c_str();

    // Greenish background color
    glClearColor(0.1f, 0.2f, 0.1f, 1.0f);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSrc, nullptr);
    glCompileShader(vertexShader);

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSrc, nullptr);
    glCompileShader(fragmentShader);

    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    running = true;
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

            SDL_snprintf(title, sizeof(title), "OpenGL SDL3 App - %.1f FPS", fpsValue);
            SDL_SetWindowTitle(window, title);
        }
    }
}

void App::shutdown()
{
    if (glContext)
    {
        SDL_GL_DestroyContext(glContext);
        glContext = nullptr;
    }
    if (window)
    {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    if (VAO)
    {
        glDeleteVertexArrays(1, &VAO);
        VAO = 0;
    }
    if (VBO)
    {
        glDeleteBuffers(1, &VBO);
        VBO = 0;
    }
    if (shaderProgram)
    {
        glDeleteProgram(shaderProgram);
        shaderProgram = 0;
    }
    if (sdlInitialized)
    {
        SDL_Quit();
        sdlInitialized = false;
    }
}

void App::handleEvent(const SDL_Event &e)
{
    if (e.type == SDL_EVENT_QUIT)
        running = false;
}

void App::render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    // --- Render triangle here ---
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}