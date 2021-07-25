#ifndef ZTIME
#define ZTIME
#include <SDL\SDL.h>
class Timer {
public:
    Timer(){}
    Timer(float t) {
        m_tick = (Uint32)(t * 1000);
    }
    void start() {
        if (!m_started) {
            m_started = true;
            m_tick = SDL_GetTicks();
            m_lostTicks = 0;
            m_globalPausedTime = 0;
        }
    }
    void restart() {
        m_started = true;
        m_paused = false;
        m_tick = SDL_GetTicks();
        m_lostTicks = 0;
        m_globalPausedTime = 0;
    }
    void stop() {
        m_started = false;
        m_paused = false;
    }
    void pause() {
        if (m_started && !m_paused) {
            m_paused = true;
            m_lostTicks = SDL_GetTicks();
        }
    }
    void resume() {
        if (m_started && m_paused) {
            m_globalPausedTime += SDL_GetTicks() - m_lostTicks;
            m_paused = false;
        }
    }

    float getTicks() const {
        if (m_started) {
            Uint32 pausedTime;
            if (m_paused)
            pausedTime = SDL_GetTicks() - m_lostTicks;
            else pausedTime = 0;
            //m_lastTick = ;
            return (SDL_GetTicks() - m_tick - pausedTime - m_globalPausedTime) / 1000.0f;
        }
        return 1000000.0f;
    }
    bool isFinished(float waitTime) const {
        return getTicks() >= waitTime;
    }
private:
    bool m_started = false;
    bool m_paused = false;
    Uint32 m_globalPausedTime = 0;
    Uint32 m_tick = 0;
    Uint32 m_lostTicks = 0;
};
#endif // ZTIME

