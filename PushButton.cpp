#include "PushButton.h"


PushButton::PushButton(const glm::vec2 &position, const glm::vec2 &dimentions, const char* textureName, Widget* parent)
: Button(position, dimentions, parent) {
    m_textures = addUiTexture(textureName);
    if (m_textures.size() >= 4) {
        m_textures[Hover]->setEnabled(false);
        m_textures[PUSHED]->setEnabled(false);
        m_textures[INACTIVE]->setEnabled(false);
    }
}
PushButton::PushButton(const glm::vec4 &posDimPercent, const char* textureName, Widget* parent)
: Button(posDimPercent, parent){
    m_textures = addUiTexture(textureName);
    if (m_textures.size() >= 4) {
        m_textures[Hover]->setEnabled(false);
        m_textures[PUSHED]->setEnabled(false);
        m_textures[INACTIVE]->setEnabled(false);
    }
}
PushButton::PushButton(int tag, const glm::vec2 &dimPercent, const char* textureName, Widget* parent)
: Button(tag, dimPercent, parent) {
    m_textures = addUiTexture(textureName);
    if (m_textures.size() >= 3) {
        m_textures[Hover]->setEnabled(false);
        m_textures[PUSHED]->setEnabled(false);
        m_textures[INACTIVE]->setEnabled(false);
    }

}
void PushButton::setActive(bool Bool) {
    Button::setActive(Bool);
    if (isActive()) {
        setNormal();
    }
    else {
        if (m_textures.size() >= 4) {
            m_textures[NORMAL]->setEnabled(false);
            m_textures[Hover]->setEnabled(false);
            m_textures[PUSHED]->setEnabled(false);
            m_textures[INACTIVE]->setEnabled(true);
        }
    }
}
void PushButton::setNormal() {
    if (m_textures.size() >= 4) {
        m_textures[NORMAL]->setEnabled(true);
        m_textures[Hover]->setEnabled(false);
        m_textures[PUSHED]->setEnabled(false);
        m_textures[INACTIVE]->setEnabled(false);
    }
}
void PushButton::setHover() {
    if (m_textures.size() >= 4) {
        m_textures[NORMAL]->setEnabled(false);
        m_textures[Hover]->setEnabled(true);
        m_textures[PUSHED]->setEnabled(false);
        m_textures[INACTIVE]->setEnabled(false);
    }
}
void PushButton::setPushed() {
    if (m_textures.size() >= 4) {
        m_textures[NORMAL]->setEnabled(false);
        m_textures[Hover]->setEnabled(false);
        m_textures[PUSHED]->setEnabled(true);
        m_textures[INACTIVE]->setEnabled(false);
    }
}


