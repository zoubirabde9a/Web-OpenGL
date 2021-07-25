#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H
#include <unordered_map>
#include <glm/glm.hpp>

class InputManager
{
public:
    void update();
    void pressKey(unsigned int keyID);
    void releaseKey(unsigned int keyID);
    void setMouseCoords(unsigned int x, unsigned int y);
    void setMouseRel(unsigned int x, unsigned int y);

    bool isKeyDown(unsigned int keyID) const;
    bool isKeyPressed(unsigned int keyID) const;
    bool isKeyReleased(unsigned int keyID) const;

    inline const glm::vec2& getMouseCoords() const { return m_mouseCoords ;}
    inline const glm::vec2& getMouseRel() const { return m_mouseRel ;}
public:
    bool wasKeyDown(unsigned int keyID) const;
private:
    std::unordered_map<unsigned int, bool> m_keyMap;
    std::unordered_map<unsigned int, bool> m_previousKeyMap;
    glm::vec2 m_mouseCoords;
    glm::vec2 m_mouseRel;
};

#endif // INPUTMANAGER_H
