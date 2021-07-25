#pragma once
#include "ZGUI.h"
#include <memory>
class Button : public Widget
{
public:
    Button() { ; }
    Button(const glm::vec2 &position, const glm::vec2 &dimentions, Widget* parent);
    Button(const glm::vec4 &posDimPercent, Widget* parent);
    Button(int tag, const glm::vec2 &dimPercent, Widget* parent);
    virtual void subscribeEvent(int evnt, const std::function<void()> &func);
    //virtual void subscribeEvent_lua(int evnt, luabridge::LuaRef func);
    virtual void triggerEvent(int evnt);
    void setDownInterval(float interval) { m_downInterval = interval; }
    virtual void setActive(bool Bool) { m_active = Bool; }
    virtual void setNormal(){}
    virtual void setHover(){}
    virtual void setPushed(){}

    virtual void onHover(const InputManager &inputManager) override;
    virtual void onNotHover(const InputManager &inputManager) override;


    inline void setKeyButton(bool tag) { m_keyButton = tag; }

    void setText(const TextRGB8 &text);
    void setTextScale(const glm::vec2 &scale);
    virtual void setJustification(Justification just);
    void setPadding(float padding);
    void setPadding(const glm::vec2 &padding);
    virtual void setFont(SpriteFont* font);


    inline bool isActive() const { return m_active; }
    inline bool isPushed() const { return m_pushed; }

    inline const TextRGB8 &getText() const { return m_label->getText(); }
    inline SpriteFont* getFont() const { return m_label->getFont(); }
    inline ZLabel* getLabel() { return m_label; }
private:
    bool m_active = true;
protected:
    ZLabel* m_label = nullptr;
    float m_downInterval = 40.0f;
    bool m_keyButton = false;
    std::function< void() > m_clickedFunction;
    std::function< void() > m_buttonDownFunction;
    std::function< void() > m_HoverFunction;
    int m_frame = 0;
    bool m_pushed = false;
};
