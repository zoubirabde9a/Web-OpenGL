#include "inputmanager.h"

void InputManager::update() {
    for (auto& it : m_keyMap) {
        m_previousKeyMap[it.first] = it.second;
    }
}

void InputManager::pressKey(unsigned int keyID) {
    m_previousKeyMap[keyID] = m_keyMap[keyID];
    m_keyMap[keyID] = true;
}

void InputManager::releaseKey(unsigned int keyID) {
    m_previousKeyMap[keyID] = m_keyMap[keyID];
    m_keyMap[keyID] = false;
}

void InputManager::setMouseCoords(unsigned int x, unsigned int y) {
    m_mouseCoords.x = (float)x;
    m_mouseCoords.y = (float)y;
}

void InputManager::setMouseRel(unsigned int x, unsigned int y) {
    m_mouseRel.x = (float)x;
    m_mouseRel.y = (float)y;
}

bool InputManager::isKeyDown(unsigned int keyID) const {
    auto it = m_keyMap.find(keyID);
    if (it != m_keyMap.end()) {
        return it->second;
    } else {
        return false;
    }
}
bool InputManager::isKeyPressed(unsigned int keyID) const {
     if (isKeyDown(keyID) == true && wasKeyDown(keyID) == false) {
         return true;
     }
     return false;
}
bool InputManager::isKeyReleased(unsigned int keyID) const {
    if (isKeyDown(keyID) == false && wasKeyDown(keyID) == true) {
        return true;
    }
    return false;
}
bool InputManager::wasKeyDown(unsigned int keyID) const {
    auto it = m_previousKeyMap.find(keyID);
    if (it != m_previousKeyMap.end()) {
        return it->second;
    } else {
        return false;
    }
}

