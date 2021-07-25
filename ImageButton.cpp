#include "ImageButton.h"

ImageButton::ImageButton(const glm::vec2 &position, const glm::vec2 &dimentions, Widget* parent)
: Button(position, dimentions, parent){

}
ImageButton::ImageButton(const glm::vec4 &posDimPercent, Widget* parent)
: Button(posDimPercent, parent){

}
ImageButton::ImageButton(const char* textureName, const glm::vec2 &position, const glm::vec2 &dimentions, Widget* parent)
: Button(position, dimentions, parent){
    setTexture(textureName);
}
ImageButton::ImageButton(const char* textureName, const glm::vec4 &posDimPercent, Widget* parent)
: Button(posDimPercent, parent){
    setTexture(textureName);
}
void ImageButton::setTexture(const char* textureName) {
    for (auto &it : m_textures) {
        destroyFrame(it);
    }
    m_textures.clear();
    m_textures = addTexture(textureName);
    if (m_textures.size() > Hover)
        m_textures[Hover]->setEnabled(false);
    if (m_textures.size() > PUSHED)
        m_textures[PUSHED]->setEnabled(false);
    if (m_textures.size() > INACTIVE)
        m_textures[INACTIVE]->setEnabled(false);
}
void ImageButton::setActive(bool Bool) {
    Button::setActive(Bool);
    if (isActive()) {
        setNormal();
    }
    else {
        if (m_textures.size() == 4) {
            m_textures[NORMAL]->setEnabled(false);
            m_textures[Hover]->setEnabled(false);
            m_textures[PUSHED]->setEnabled(false);
            m_textures[INACTIVE]->setEnabled(true);
        }
    }
}
void ImageButton::setNormal() {
    if (m_textures.size() == 4) {
        m_textures[NORMAL]->setEnabled(true);
        m_textures[Hover]->setEnabled(false);
        m_textures[PUSHED]->setEnabled(false);
        m_textures[INACTIVE]->setEnabled(false);
    }
    else if (m_textures.size()  > 0){
        m_textures[NORMAL]->setPositionPercent(glm::vec2(0.0f));
        m_textures[NORMAL]->setDimentionsPercent(glm::vec2(1.0f));
    }
}
void ImageButton::setHover() {
    if (m_textures.size() == 4) {
        m_textures[NORMAL]->setEnabled(false);
        m_textures[Hover]->setEnabled(true);
        m_textures[PUSHED]->setEnabled(false);
        m_textures[INACTIVE]->setEnabled(false);
    }
    else {

    }
}
void ImageButton::setPushed() {
    if (m_textures.size() == 4) {
        m_textures[NORMAL]->setEnabled(false);
        m_textures[Hover]->setEnabled(false);
        m_textures[PUSHED]->setEnabled(true);
        m_textures[INACTIVE]->setEnabled(false);
    }
    else if (m_textures.size()  > 0) {
        m_textures[NORMAL]->setPositionPercent(glm::vec2(0.05f));
        m_textures[NORMAL]->setDimentionsPercent(glm::vec2(0.9f));
    }
}
bool ImageButton::isHover() const {
    if (m_textures.size() == 4) {
        return m_textures[Hover]->isEnabled();
    }
    else return false;
}
