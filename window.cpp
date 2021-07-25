#include "window.h"
#include "stdio.h"
#include "opengl.h"


bool Window::create(const char* name, int width, int height, Uint32 flags){
    Uint32 fl = SDL_WINDOW_OPENGL | flags;
    m_window = SDL_CreateWindow(name,
                             SDL_WINDOWPOS_CENTERED,
                             SDL_WINDOWPOS_CENTERED,
                              width,
                             height,
                             fl);
    if (m_window == nullptr) {
        fprintf(stderr, "SDL Window could not be created!");
        return false;
    }

    SDL_GLContext glContext = SDL_GL_CreateContext(m_window);
    if (glContext == nullptr) {
        fprintf(stderr, "SDL_GL context could not be created!");
        return false;
    }
#ifdef EMSCRIPTEN

#else
    GLenum error = glewInit();
    if (error != GLEW_OK) {
        fprintf(stderr, "Could not initialize glew!");
    }
#endif

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    SDL_GL_SetSwapInterval(0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    swapBuffer();
    return true;
}

void Window::resize(int width, int height){
    if (!m_window) return;
    SDL_SetWindowSize(m_window, width, height);
    SDL_SetWindowPosition(m_window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
}
void Window::hide() const {
     if (!m_window) return;
    SDL_HideWindow(m_window);
}
void Window::show() const {
    if (!m_window) return;
    SDL_ShowWindow(m_window);
}
void Window::swapBuffer() const {
    if (!m_window) return;
    SDL_GL_SwapWindow(m_window);
}
int Window::getWidth(){
    if (!m_window) return 0;
    int w, h;
    SDL_GetWindowSize(m_window, &w, &h);
    return w;
}

int Window::getHeight(){
    if (!m_window) return 0;
    int w, h;
    SDL_GetWindowSize(m_window, &w, &h);
    return h;
}


