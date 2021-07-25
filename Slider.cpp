
#include "Slider.h"
#include "ResourceManager.h"


Slider::SliderButton::SliderButton(const glm::vec2 &position, const glm::vec2 &dimentions, const char* textureName, Widget* parent)
: PushButton(position, dimentions, textureName, parent){}
Slider::SliderButton::SliderButton(const glm::vec4 &posDimPercent, const char* textureName, Widget* parent)
: PushButton(posDimPercent, textureName, parent) {}
Slider::SliderButton::SliderButton(int tag, const glm::vec2 &dimPercent, const char* textureName, Widget* parent)
: PushButton(tag, dimPercent, textureName, parent) {}
void Slider::SliderButton::onNotHover(const InputManager& inputManager) {
    if (!inputManager.isKeyDown(SDL_BUTTON_LEFT) && m_pushed) {
        m_pushed = false;
        setNormal();
    }
    else if (!m_pushed) {
        setNormal();
    }
}
Slider::Slider(const glm::vec2 &position, const glm::vec2 &dimentions, const char** textures, Widget* parent)
: Widget(position, dimentions, parent){
    construct(textures);
}
Slider::Slider(const glm::vec4 &posDimPercent, const char** textures, Widget* parent)
: Widget(posDimPercent, parent) {
    construct(textures);
}
Slider::Slider(int tag, const glm::vec2 &dimPecent, const char** textures, Widget* parent)
: Widget(tag, dimPecent, parent) {
    construct(textures);
}
void Slider::update(const InputManager &inputManager, std::vector < Widget* > &widgets) {
    Widget::update(inputManager, widgets);
    if (m_button->isPushed())
    {
        glm::vec2 position;
        float rawXCoords = inputManager.getMouseCoords().x - (m_button->getDimentions().x / 2.0f);
        position.x = rawXCoords * ((rawXCoords >= m_container->getPosition().x) && (rawXCoords <= m_container->getPosition().x + m_container->getDimentions().x - m_button->getDimentions().x))
            + m_container->getPosition().x * (rawXCoords < m_container->getPosition().x) + (m_container->getPosition().x + m_container->getDimentions().x - m_button->getDimentions().x) * (rawXCoords > m_container->getPosition().x + m_container->getDimentions().x - m_button->getDimentions().x);
        position.y = m_button->getPosition().y;
        setCurrentValue(m_minValue + (position.x - m_container->getPosition().x) / (m_container->getDimentions().x - m_button->getDimentions().x) * getIntervalLen());
    }
}
void Slider::setMaxValue(float value) {
    float valuePercent = (m_currentValue - m_minValue) / getIntervalLen();
    m_maxValue = value * (value >= m_minValue) + m_minValue * (value < m_minValue);
    m_currentValue = getIntervalLen() * valuePercent;
}
void Slider::setMinValue(float value) {
    float valuePercent = (m_currentValue - m_minValue) / getIntervalLen();
    m_minValue = value * (value <= m_maxValue) + m_maxValue * (value > m_maxValue);
    m_currentValue = getIntervalLen() * valuePercent;
}
void Slider::append(float value) {
    m_currentValue =
        (m_currentValue + value) * ((m_currentValue + value) <= m_maxValue && (m_currentValue + value) >= m_minValue)
        + (m_currentValue + value > m_maxValue) * m_maxValue + (m_currentValue + value < m_minValue) * m_minValue;

    m_button->setPosition(glm::vec2(m_container->getPosition().x + ((m_currentValue - m_minValue) / getIntervalLen()) * (m_container->getDimentions().x - m_button->getDimentions().x), m_button->getPosition().y));

    updateSliderTexture();
    if (m_valueChangedFunc)m_valueChangedFunc();
}
void Slider::setCurrentValue(float value) {
    m_currentValue = value * ((value <= m_maxValue) && (value >= m_minValue)) +
        m_maxValue * (value > m_maxValue) + m_minValue * (value < m_minValue);

    m_button->setPosition(glm::vec2(m_container->getPosition().x + ((m_currentValue - m_minValue) / getIntervalLen()) * (m_container->getDimentions().x - m_button->getDimentions().x), m_button->getPosition().y));

    updateSliderTexture();
    if (m_valueChangedFunc)m_valueChangedFunc();
}
void Slider::setColor(const ColorRGBA8& color)
{
    m_texture->setColor(color);
}

void Slider::subscribeEvent(EventType type, const std::function < void() > &fn) {
    if (type == EventType::VALUE_CHANGED) {
        m_valueChangedFunc = fn;
    }
}
void Slider::construct(const char** textures) {
    if (!m_constructed) {
        addUiTexture(textures[TEXTURE]);
        m_container = new Widget(glm::vec4(0.01f, 0.06f, 0.98f , 0.88f), this);
        m_button = new SliderButton(glm::vec4(0.0f, 0.0f, 0.1f, 1.0f), textures[BUTTON], m_container);
        m_texture = m_container->addUiTexture(textures[PROGRESSBAR])[0];
        m_texture->setColor(ColorRGBA8(255, 255, 255, 255));
        updateSliderTexture();
        m_constructed = true;
    }
}
void Slider::updateSliderTexture() {
    m_texture->setDimentionsPercent(glm::vec2((m_currentValue - m_minValue) / getIntervalLen(), 1.0f));
}
