#include "Button.h"

Button::Button(const glm::vec2 &position, const glm::vec2 &dimentions, Widget* parent)
: Widget(position, dimentions, parent) {
    m_label = new ZLabel(glm::vec4(0, 0, 1, 1), this);
    m_label->setJustification(Justification::MIDDLE);
}
Button::Button(const glm::vec4 &posDimPercent, Widget* parent)
: Widget(posDimPercent, parent) {
    m_label = new ZLabel(glm::vec4(0, 0, 1, 1), this);
    m_label->setJustification(Justification::MIDDLE);
}
Button::Button(int tag, const glm::vec2 &dimPercent, Widget* parent)
: Widget(tag, dimPercent, parent){
    m_label = new ZLabel(glm::vec4(0, 0, 1, 1), this);
    m_label->setJustification(Justification::MIDDLE);
}
void Button::onHover(const InputManager &inputManager) {
    if (inputManager.isKeyReleased(SDL_BUTTON_LEFT)) {
        m_frame = 0;
        m_pushed = false;
        setNormal();
        if (m_clickedFunction) m_clickedFunction();
    }
    if (inputManager.isKeyDown(SDL_BUTTON_LEFT)) {
        m_frame++;
        m_pushed = true;
        setPushed();
        if (m_frame > m_downInterval) {
            if (m_buttonDownFunction) m_buttonDownFunction();
        }
    }
    else {
        setHover();
        m_pushed = false;
        if (m_HoverFunction)m_HoverFunction();
    }
}
void Button::onNotHover(const InputManager &inputManager) {
    if (!m_keyButton) {
        setNormal();
        m_pushed = false;
    }
}
void Button::subscribeEvent(int evnt, const std::function<void()> &func) {
    switch (evnt) {
    case EventType::BUTTON_CLICKED:
        m_clickedFunction = func;
        break;
    case EventType::BUTTON_DOWN:
        m_buttonDownFunction = func;
        break;
    case EventType::BUTTON_HOVER:
        m_HoverFunction = func;
        break;
    }
}
/*void Button::subscribeEvent_lua(int evnt, luabridge::LuaRef func) {
    switch (evnt) {
    case EventType::BUTTON_CLICKED:
        m_clickedFunction = [=]() {
            try {
                func();
            }
            catch (luabridge::LuaException e) {
                printf("Button Click Function Error : %s", e.what());
            }
        };
        break;
    case EventType::BUTTON_DOWN:
        m_buttonDownFunction = [=]() {
            try {
                func();
            }
            catch (luabridge::LuaException e) {
                printf("Button Down Function Error : %s", e.what());
            }
        };
        break;
    case EventType::BUTTON_Hover:
        m_HoverFunction = [=]() {
            try {
                func();
            }
            catch (luabridge::LuaException e) {
                printf("Button Down Function Error : %s", e.what());
            }
        };
        break;
    }
}*/
void Button::triggerEvent(int evnt) {
    switch (evnt) {
    case EventType::BUTTON_CLICKED:
        if (m_clickedFunction) m_clickedFunction();
        break;
    case EventType::BUTTON_DOWN:
        if (m_buttonDownFunction) m_buttonDownFunction();
        break;
    case EventType::BUTTON_HOVER:
        if (m_HoverFunction) m_HoverFunction();
        break;
    }
}
void Button::setText(const TextRGB8 &text) {
    m_label->setText(text);
}
void Button::setTextScale(const glm::vec2 &scale) {
    m_label->setScale(scale);
}
void Button::setJustification(Justification just) {
    m_label->setJustification(just);
}
void Button::setPadding(float padding) {
    m_label->setPadding(padding);
}
void Button::setPadding(const glm::vec2 &padding) {
    m_label->setPadding(padding);
}
void Button::setFont(SpriteFont* font) {
    m_label->setFont(font);
}

