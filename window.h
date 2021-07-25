#ifndef WINDOW_H
#define WINDOW_H
#include <SDL/SDL.h>
class Window
{
public:
    bool create(const char* name, int width, int height, Uint32 flags);
    void resize(int width, int height);

    void hide() const;
    void show() const;
    void swapBuffer() const;

    int getWidth();
    int getHeight();
private:
    SDL_Window* m_window = nullptr;
};

#endif // WINDOW_H
