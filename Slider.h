#ifndef SLIDER_H
#define SLIDER_H
#include "PushButton.h"

class Slider : public Widget {
public:
    enum { TEXTURE, BUTTON, PROGRESSBAR };
public:
    Slider(const glm::vec2 &position, const glm::vec2 &dimentions, const char** textures, Widget* parent);
    Slider(const glm::vec4 &posDimPercent, const char** textures, Widget* parent);
    Slider(int tag, const glm::vec2 &dimPecent, const char** textures, Widget* parent);

    virtual void update(const InputManager &inputManager, std::vector < Widget* > &widgets)override;

    void setMaxValue(float value);
    void setMinValue(float value);
    void append(float value);
    void setCurrentValue(float value);

    void setColor(const ColorRGBA8& color);

    void subscribeEvent(EventType type, const std::function < void() > &fn);

    inline float getCurrentValue() const { return m_currentValue; }
    inline float getIntervalLen() const { return m_maxValue - m_minValue; }
private:
    class SliderButton : public PushButton {
    public:
        SliderButton(const glm::vec2 &position, const glm::vec2 &dimentions, const char* textureName, Widget* parent);
        SliderButton(const glm::vec4 &posDimPercent, const char* textureName, Widget* parent);
        SliderButton(int tag, const glm::vec2 &dimPercent, const char* textureName, Widget* parent);

        void onNotHover(const InputManager& inputManager) override;
    };
private:
    void construct(const char** textures);
    void updateSliderTexture();
private:
    Widget* m_container;
    PushButton* m_button;
    UiTexture* m_texture;
    bool m_constructed = false;
    float m_currentValue = 0.0f;
    float m_maxValue = 1.0f;
    float m_minValue = 0.0f;
    std::function < void() > m_valueChangedFunc;
};

#endif


