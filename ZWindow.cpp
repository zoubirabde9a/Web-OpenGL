#include "ZWindow.h"
const float fixedButtonSize = 16.0f;
const float fixedButtonArea = fixedButtonSize + 4.0f;
ZWindow::ZWindow(const glm::vec2 &position, const glm::vec2 &dimentions,
    const char* textureName, Widget* parent)
    : Widget(position, dimentions, parent) {
    m_label = new ZLabel(glm::vec2(0, getDimentions().y - m_labelSize),
        glm::vec2(getDimentions().x - m_labelSize, m_labelSize), this);
    addUiTexture(textureName);
    m_label->setPadding(12.0f);
    m_label->setFixedSize(true);
    m_button = new ImageButton(getDimentions() - glm::vec2(fixedButtonArea),
        glm::vec2(fixedButtonSize), this);
    m_button->setTexture("crossButton");
    m_button->setFixedSize(true);
    m_button->subscribeEvent(EventType::BUTTON_CLICKED, [=]() {setEnabled(false);});
    updateItems();
}
ZWindow::ZWindow(const glm::vec4 &posDimPercent, const char* textureName,
    Widget* parent)
    : Widget(posDimPercent, parent) {
    m_label = new ZLabel(glm::vec2(0, getDimentions().y - m_labelSize),
        glm::vec2(getDimentions().x - m_labelSize, m_labelSize), this);
    addUiTexture(textureName);
    m_label->setPadding(12.0f);
    m_label->setFixedSize(true);
    m_button = new ImageButton(getDimentions() - glm::vec2(fixedButtonArea), glm::vec2(fixedButtonSize), this);
    m_button->setTexture("crossButton");
    m_button->setFixedSize(true);
    m_button->subscribeEvent(EventType::BUTTON_CLICKED, [=]() { setEnabled(false);} );
    updateItems();
}
ZWindow::ZWindow(int tag, const glm::vec2 &dimentions, const char* textureName, Widget* parent)
: Widget(tag, dimentions, parent){
    m_label = new ZLabel(glm::vec2(0, getDimentions().y - m_labelSize), glm::vec2(getDimentions().x - m_labelSize, m_labelSize), this);
    addUiTexture(textureName);
    m_label->setPadding(12.0f);
    m_label->setFixedSize(true);
    m_button = new ImageButton(getDimentions() - glm::vec2(fixedButtonArea), glm::vec2(fixedButtonSize), this);
    m_button->setTexture("crossButton");
    m_button->setFixedSize(true);
    m_button->subscribeEvent(EventType::BUTTON_CLICKED, [=]() {setEnabled(false);});
    updateItems();
}
void ZWindow::setLabelSize(float size) {
    m_labelSize = size;
    updateItems();
}
void ZWindow::setTitle(const TextRGB8 &title) {
    m_label->setText(title);
    updateItems();
}
void ZWindow::setFont(SpriteFont* font) {
    m_label->setFont(font);
    updateItems();
}
void ZWindow::update(const InputManager &inputManager, std::vector < Widget* > &widgets) {
    Widget::update(inputManager, widgets);
    const glm::vec2& mouseCoords = inputManager.getMouseCoords();

    if (inputManager.isKeyPressed(SDL_BUTTON_LEFT) && m_label->isMouseOnWidget(mouseCoords) && !isMouseOnWidgets(widgets, mouseCoords)) {
        m_pushed = true;
    }
    else if (inputManager.isKeyReleased(SDL_BUTTON_LEFT)) {
        m_pushed = false;
    }
    if (m_pushed) {
        offsetPosition(inputManager.getMouseRel());
    }
}
void ZWindow::updateItems() {
    float height;
    height = getFont() && m_label->getText().size() > 0 ? getFont()->getFontHeight() + 8.0f : m_labelSize;
    m_label->setDimention(glm::vec2(getDimentions().x - fixedButtonArea, height));
    m_label->setRelativePosition(glm::vec2(0.0f, getDimentions().y - height));
    m_button->setDimention(glm::vec2(fixedButtonSize));
    m_button->setRelativePosition(getDimentions() - glm::vec2(fixedButtonArea));
}
void ZWindow::onResize() {
    updateItems();
}
