#include "zgui.h"
#include "utils.h"
#include "ResourceManager.h"
#include "jsonfunctions.h"
#ifdef EMSCRIPTEN
const char* GUI_VERT_SRC = R"(
precision mediump float;
attribute vec2 vertexPosition;
attribute vec4 vertexColor;
attribute vec2 vertexUV;

varying vec2 fragmentPosition;
varying vec4 fragmentColor;
varying vec2 fragmentUV;

uniform mat4 P;

void main() {
    gl_Position.xy = (P * vec4(vertexPosition, 0.0, 1.0)).xy;
    gl_Position.z = 0.0;
    gl_Position.w = 1.0;

    fragmentPosition = vertexPosition;

    fragmentColor = vertexColor;

    fragmentUV = vec2(vertexUV.x, 1.0 - vertexUV.y);
})";

const char* GUI_FRAG_SRC = R"(
precision mediump float;

varying vec2 fragmentPosition;
varying vec4 fragmentColor;
varying vec2 fragmentUV;

uniform sampler2D mySampler;

void main() {
    vec4 textureColor = texture2D(mySampler, fragmentUV);
    gl_FragColor = fragmentColor * textureColor;
})";
#else
const char* GUI_VERT_SRC = R"(#version 130
//The vertex shader operates on each vertex

//input data from the VBO. Each vertex is 2 floats
in vec2 vertexPosition;
in vec4 vertexColor;
in vec2 vertexUV;

out vec2 fragmentPosition;
out vec4 fragmentColor;
out vec2 fragmentUV;

uniform mat4 P;

void main() {
    //Set the x,y position on the screen
    gl_Position.xy = (P * vec4(vertexPosition, 0.0, 1.0)).xy;
    //the z position is zero since we are in 2D
    gl_Position.z = 0.0;

    //Indicate that the coordinates are normalized
    gl_Position.w = 1.0;

    fragmentPosition = vertexPosition;

    fragmentColor = vertexColor;

    fragmentUV = vec2(vertexUV.x, 1.0 - vertexUV.y);
})";

const char* GUI_FRAG_SRC = R"(#version 130
//The fragment shader operates on each pixel in a given polygon

in vec2 fragmentPosition;
in vec4 fragmentColor;
in vec2 fragmentUV;

//This is the 3 component float vector that gets outputted to the screen
//for each pixel.
out vec4 color;

uniform sampler2D mySampler;

void main() {

    //cos(x) returns a number between -1 and 1. To convert it into the range 0 to 1
    //you simply do (cos(x) + 1.0) * 0.5

    vec4 textureColor = texture(mySampler, fragmentUV);

    //Make crazy colors using time and position!
    color = fragmentColor * textureColor;
})";
#endif
bool Widget::isMouseOnWidgets(const std::vector < Widget* > &vec, const glm::vec2 &mousePosition) {
    for (auto &it : vec) {
        if (it->isMouseOnWidget(mousePosition))return true;
    }
    return false;
}
bool Widget::isMouseOnWidgets(Widget* list, const glm::vec2 &mousePosition) {
    for (Widget* it = list; it != nullptr; it = it->getNextWidget()) {
        if (it->isMouseOnWidget(mousePosition))return true;
    }
        return false;
}
void Widget::setFixedSize(bool tag) {
    m_fixedSize = tag;
}
void Widget::setNextWidget(Widget* next) {
    m_nextWidget = next;
}
void Widget::setPreviousWidget(Widget* previous) {
    m_previousWidget = previous;
}
void Widget::pushChildBack(Widget* child) {
    if (m_backChild) {
        m_backChild->setNextWidget(child);
        child->setPreviousWidget(m_backChild);
        child->setNextWidget(nullptr);
        m_backChild = child;
    }
    else {
        m_frontChild = child;
        m_backChild = child;
    }
    m_childrenSize++;
}
void Widget::pushChildFront(Widget* child) {
    if (m_frontChild) {
        m_frontChild->setPreviousWidget(child);
        child->setNextWidget(m_frontChild);
        m_frontChild = child;
    }
    else {
        m_frontChild = child;
        m_backChild = child;
    }
    m_childrenSize++;
}
void Widget::pushFrameBack(Widget* frame) {
    if (m_backFrame) {
        m_backFrame->setNextWidget(frame);
        frame->setPreviousWidget(m_backFrame);
        m_backFrame = frame;
    }
    else {
        m_frontFrame = frame;
        m_backFrame = frame;
    }
    m_framesSize++;
}
void Widget::pushFrameFront(Widget* frame) {
    if (m_frontFrame) {
        m_frontFrame->setPreviousWidget(frame);
        frame->setNextWidget(m_frontFrame);
        m_frontFrame = frame;
    }
    else {
        m_frontFrame = frame;
        m_backFrame = frame;
    }
    m_framesSize++;
}
void Widget::pushTextureBack(Widget* texture) {
    if (m_backTexture) {
        m_backTexture->setNextWidget(texture);
        texture->setPreviousWidget(m_backTexture);
        m_backTexture = texture;
    }
    else {
        m_frontTexture = texture;
        m_backTexture = texture;
    }
    m_texturesSize++;
}
void Widget::pushTextureFront(Widget* texture) {
    if (m_frontTexture) {
        m_frontTexture->setPreviousWidget(texture);
        texture->setNextWidget(m_frontTexture);
        m_frontTexture = texture;
    }
    else {
        m_frontTexture = texture;
        m_backTexture = texture;
    }
    m_texturesSize++;
}
void Widget::updateDestRect() {
    if (!m_fixedSize) {
        if (m_parent) {
            m_position = m_parent->m_offset + m_parent->m_position + m_parent->m_dimentions * m_positionPercent;
            m_dimentions = m_parent->m_dimentions * m_dimentionsPercent;
            onResize();
        }
        for (Widget* it = m_frontTexture; it != nullptr; it = it->getNextWidget()) {
            it->updateDestRect();
        }
        for (Widget* it = m_frontFrame; it != nullptr; it = it->getNextWidget()) {
            it->updateDestRect();
        }
        for (Widget* it = m_frontChild; it != nullptr; it = it->getNextWidget()) {
            it->updateDestRect();
        }

    }
    else {
        if (m_parent) {
            m_position = m_parent->m_offset + m_parent->m_position + m_parent->m_dimentions * m_positionPercent;
            m_dimentionsPercent = m_dimentions / m_parent->m_dimentions;
            onResize();
        }
        for (Widget* it = m_frontTexture; it != nullptr; it = it->getNextWidget()) {
            it->updateDestRect();
        }
        for (Widget* it = m_frontFrame; it != nullptr; it = it->getNextWidget()) {
            it->updateDestRect();
        }
        for (Widget* it = m_frontChild; it != nullptr; it = it->getNextWidget()) {
            it->updateDestRect();
        }
    }
}
void Widget::updateDestRectPercent() {
    if (m_parent) {
        m_positionPercent = (m_position - m_parent->m_position) / m_parent->m_dimentions;
        m_dimentionsPercent = m_dimentions / m_parent->m_dimentions;
        onResize();
    }
    for (Widget* it = m_frontTexture; it != nullptr; it = it->getNextWidget()) {
        it->updateDestRect();
    }
    for (Widget* it = m_frontFrame; it != nullptr; it = it->getNextWidget()) {
        it->updateDestRect();
    }
    for (Widget* it = m_frontChild; it != nullptr; it = it->getNextWidget()) {
        it->updateDestRect();
    }
}
Widget::Widget(const glm::vec2 &position, const glm::vec2 &dimentions, Widget* parent)
{
    m_dimentions = dimentions;
    m_position = position;
    m_parent = parent;
    if (m_parent) {
        m_position += parent->getPosition();
        m_positionPercent = (m_position - m_parent->getPosition()) / m_parent->getDimentions();
        m_dimentionsPercent = m_dimentions / m_parent->getDimentions();
        m_parent->addChild(this);
    }
    m_originalPosition = getPosition();
    m_originalDimentions = getDimentions();
}
Widget::Widget(const glm::vec4 &posDimPercent, Widget* parent){
    m_parent = parent;
    m_positionPercent = glm::vec2(posDimPercent.x, posDimPercent.y);
    m_dimentionsPercent = glm::vec2(posDimPercent.z, posDimPercent.w);
    if (m_parent) {
        m_parent->addChild(this);
    }
    m_originalPosition = getPosition();
    m_originalDimentions = getDimentions();
}
Widget::Widget(int tag, const glm::vec2 &dimPercent, Widget* parent) {
    m_parent = parent;
    m_dimentionsPercent = dimPercent;
    align(tag);
    if (m_parent) {
        m_parent->addChild(this);
    }
    m_originalPosition = getPosition();
    m_originalDimentions = getDimentions();
}
Widget::~Widget() {
    destroy();
}
void Widget::destroy() {
    if (m_layout) {
        delete m_layout;
        m_layout = nullptr;
    }
    destroyChildren();
}
void Widget::unselectChildren() {
    for (Widget* it = m_frontChild; it != nullptr; it = it->getNextWidget()) {
        it->setSelected(false);
        it->unselectChildren();
    }
}
void Widget::addChild(Widget* child) {
    pushChildBack(child);
    child->setParent(this);
    child->setGui(m_gui);
    child->updateDestRect();
    updateLayout();
}
void Widget::addFrame(Frame* frame) {
    pushFrameBack(frame);
    frame->setParent(this);
    frame->setGui(m_gui);
    frame->updateDestRect();
}
void Widget::addFrameFront(Frame* frame) {
    pushFrameFront(frame);
    frame->setParent(this);
    frame->setGui(m_gui);
    frame->updateDestRect();
}
void Widget::addGuiTexture(GuiTexture* texture) {
    pushTextureBack(texture);
    texture->setParent(this);
    texture->setGui(m_gui);
    texture->updateDestRect();
}
void Widget::removeChild(Widget* child) {
    if (child->getParent() == this) {
        Widget* next = child->getNextWidget();
        Widget* previous = child->getPreviousWidget();
        if (next && previous) {
            next->setPreviousWidget(previous);
            previous->setNextWidget(next);
        }
        else if (next) {
            m_frontChild = next;
            next->setPreviousWidget(nullptr);
        }
        else if (previous) {
            m_backChild = previous;
            previous->setNextWidget(nullptr);
        }
        else {
            m_frontChild = nullptr;
            m_backChild = nullptr;
        }
        m_childrenSize--;
        updateLayout();
    }
}
void Widget::destroyChild(Widget* child) {
    if (child->getParent() == this) {
        Widget* next = child->getNextWidget();
        Widget* previous = child->getPreviousWidget();
        if (next && previous) {
            next->setPreviousWidget(previous);
            previous->setNextWidget(next);
        }
        else if (next) {
            m_frontChild = next;
            next->setPreviousWidget(nullptr);
        }
        else if (previous) {
            m_backChild = previous;
            previous->setNextWidget(nullptr);
        }
        else {
            m_frontChild = nullptr;
            m_backChild = nullptr;
        }
        delete child;
        m_childrenSize--;
        updateLayout();
    }
}
void Widget::destroyFrame(Frame* frame) {
    if (frame->getParent() == this) {
        Widget* next = frame->getNextWidget();
        Widget* previous = frame->getPreviousWidget();
        if (next && previous) {
            next->setPreviousWidget(previous);
            previous->setNextWidget(next);
        }
        else if (next) {
            m_frontFrame = next;
            next->setPreviousWidget(nullptr);
        }
        else if (previous) {
            m_backFrame = previous;
            previous->setNextWidget(nullptr);
        }
        else {
            m_frontFrame = nullptr;
            m_backFrame = nullptr;
        }
        delete frame;
        m_framesSize--;
    }
}
void Widget::destroyChild(const std::vector < Widget* > &children) {
    for (auto &it : children) {
        destroyChild(it);
    }
}
void Widget::destroyChildren() {
    Widget* tmp;
    Widget* it;

    for (it = m_frontTexture; it != nullptr;) {
        tmp = it->getNextWidget();
        delete it;
        it = tmp;
    }
    m_frontTexture = nullptr;
    m_backTexture = nullptr;
    m_texturesSize = 0;


    for (it = m_frontFrame; it != nullptr;) {
        tmp = it->getNextWidget();
        delete it;
        it = tmp;
    }
    m_frontFrame = nullptr;
    m_backFrame = nullptr;
    m_framesSize = 0;

    for (it = m_frontChild; it != nullptr;) {
        tmp = it->getNextWidget();
        delete it;
        it = tmp;
    }
    m_frontChild = nullptr;
    m_backChild = nullptr;
    m_childrenSize = 0;
}
void Widget::draw(BatchRenderer &batchRenderer, GLSLProgram* program) {
    for (Widget* it = m_backTexture; it != nullptr; it = it->getPreviousWidget()) {
        if (it->Enabled) {
            it->draw(batchRenderer, program);
        }
    }
    for (Widget* it = m_backFrame; it != nullptr; it = it->getPreviousWidget()) {
        if (it->Enabled) {
            it->draw(batchRenderer, program);
        }
    }
    for (Widget* it = m_backChild; it != nullptr; it = it->getPreviousWidget()) {
        if (it->Enabled) {
            it->draw(batchRenderer, program);
        }
    }
}
void Widget::update(const InputManager &inputManager, std::vector < Widget* >  &widgets) {
    for (Widget* it = m_frontChild; it != nullptr; it = it->getNextWidget()) {
        if (it->Enabled) {
            it->update(inputManager, widgets);
            if (!it->m_shadow) {
                widgets.push_back(it);
            }
        }
    }
    for (Widget* it = m_frontFrame; it != nullptr; it = it->getNextWidget()) {
        if (it->Enabled) {
            it->update(inputManager, widgets);
        }
    }

    const glm::vec2& mousePosition = inputManager.getMouseCoords();
    bool mouseOnWidget = isMouseOnWidget(mousePosition) && !isMouseOnWidgets(widgets, mousePosition);
    if (inputManager.isKeyPressed(SDL_BUTTON_LEFT) && mouseOnWidget) {
        m_clickInside = true;
    }
    else if (inputManager.isKeyReleased(SDL_BUTTON_LEFT)) {
        if (mouseOnWidget && m_clickInside) {
            if (!isStatic()) {
                moveToFront();
            }
            Widget* parent = getParent();
            if (parent) {
                for (Widget* it = parent->getNextWidget(); it != nullptr; it = it->getNextWidget()) {
                    it->setSelected(false);
                    it->unselectChildren();
                }
                for (Widget* it = parent->getPreviousWidget(); it != nullptr; it = it->getPreviousWidget()) {
                    it->setSelected(false);
                    it->unselectChildren();
                }
            }
            setSelected(true);
        }
        else if (!m_clickInside) {
            setSelected(false);
        }

        m_clickInside = false;
    }
    if (mouseOnWidget) {
        onHover(inputManager);
    }
    else {
        onNotHover(inputManager);
    }
}
void Widget::setDimention(const glm::vec2 &dimentions) {
    m_dimentions = dimentions;
    updateDestRectPercent();
}
void Widget::offsetDimentions(const glm::vec2 &offset) {
    m_dimentions += offset;
    updateDestRectPercent();
}
void Widget::setShadow(bool tag) {
    m_shadow = tag;
}
void Widget::setStatic(bool tag) {
    m_static = tag;
    for (Widget* it = m_frontChild; it != nullptr; it = it->getNextWidget()) {
        it->setStatic(tag);
    }
    for (Widget* it = m_frontFrame; it != nullptr; it = it->getNextWidget()) {
        it->setStatic(tag);
    }
}
void Widget::setRelativePosition(const glm::vec2 &position) {
    if (m_parent) {
        m_position = m_parent->getPosition() + position;
    }
    else {
        m_position = position;
    }
    updateDestRectPercent();
}
void Widget::setPosition(const glm::vec2 &position) {
    m_position = position;
    updateDestRectPercent();
}
void Widget::offsetPosition(const glm::vec2 &offset) {
    m_position += offset;
    updateDestRectPercent();
}
void Widget::setPositionPercent(const glm::vec2 &positionPercent) {
    m_positionPercent = positionPercent;
    updateDestRect();
}
void Widget::setYPercent(float yPercent) {
    m_positionPercent = glm::vec2(m_positionPercent.x, yPercent);
    updateDestRect();
}
void Widget::setXPercent(float xPercent) {
    m_positionPercent = glm::vec2(xPercent, m_positionPercent.y);
    updateDestRect();
}
void Widget::setDimentionsPercent(const glm::vec2 &dimentionsPercent) {
    m_dimentionsPercent = dimentionsPercent;
    updateDestRect();
}
void Widget::setWidthPercent(float widthPercent) {
    m_dimentionsPercent = glm::vec2(widthPercent, m_dimentionsPercent.y);
    updateDestRect();
}
void Widget::setHeightPercent(float heightPercent) {
    m_dimentionsPercent = glm::vec2(m_dimentionsPercent.x, heightPercent);
    updateDestRect();
}
void Widget::setOriginalPosition(const glm::vec2& position) {
    m_originalPosition = position;
}
void Widget::setOriginalDimentions(const glm::vec2& dimentions) {
    m_originalDimentions = dimentions;
}
void Widget::setOffset(const glm::vec2& offset) {
    m_offset = offset;
    updateDestRect();
}
void Widget::setOffsetX(float offset) {
    m_offset.x = offset;
    updateDestRect();
}
void Widget::setOffsetY(float offset) {
    m_offset.y = offset;
    updateDestRect();
}
void Widget::setLayout(Layout* layout) {
    if (m_layout) {
        delete m_layout;
    }
    m_layout = layout;
    updateLayout();
}
void Widget::setDepth(float depth) {
    m_depth = depth;
}
void Widget::resetOriginalPosition() {
    m_position = m_originalPosition;
}
void Widget::resetOriginalDimentions() {
    m_dimentions = m_originalDimentions;
}
void Widget::setSelected(bool tag) {
    m_selected = tag;
    if (tag) {
        onSelected();
    }
    else onUnselected();
}
bool Widget::isMouseOnWidget(const glm::ivec2 &mousePos) const {
    if (mousePos.x > m_position.x && mousePos.y > m_position.y && Enabled &&
        mousePos.x < m_position.x + m_dimentions.x && mousePos.y < m_position.y + m_dimentions.y)
        return true;
    else return false;
}
bool Widget::isMouseOnChildren(const glm::ivec2 &mousePos)const {
    if (isMouseOnWidgets(m_frontChild, mousePos)) {
        return true;
    }
    else if (isMouseOnWidgets(m_frontFrame, mousePos)) {
        return true;
    }
    else {
        return false;
    }
}
bool Widget::isMouseOnAnyChild(const glm::ivec2 &mousePos) const {
    if (Enabled) {
        for (Widget* it = m_frontChild; it != nullptr; it = it->getNextWidget()) {
            if (it->isMouseOnWidget(mousePos)) {
                return true;
            }
            else if (it->isMouseOnAnyChild(mousePos)) {
                return true;
            }
        }
        for (Widget* it = m_frontFrame; it != nullptr; it = it->getNextWidget()) {
            if (it->isMouseOnWidget(mousePos)) {
                return true;
            }
            else if (it->isMouseOnAnyChild(mousePos)) {
                return true;
            }
        }
    }
    return false;
}
bool Widget::isAnyChildSelected() const {
    for (Widget* it = m_frontChild; it != nullptr; it = it->getNextWidget()) {
        if (it->isSelected() || it->isAnyChildSelected()) {
            return true;
        }
    }
    return false;
}
float Widget::getDepth() const {
    if (m_parent) {
        return m_parent->getDepth() + m_depth;
    }
    else return m_depth;
}
void Widget::onSDLEvent(SDL_Event &evnt) {
    for (Widget* it = m_frontFrame; it != nullptr; it = it->getNextWidget()) {
        if (it->Enabled)
        it->onSDLEvent(evnt);
    }
    for (Widget* it = m_frontChild; it != nullptr; it = it->getNextWidget()) {
        if (it->Enabled)
        it->onSDLEvent(evnt);
    }
}
void Widget::updateLayout() {
    if (m_layout) {
        m_layout->layout(this);
    }
}
bool Widget::isSelected() const {
    return m_selected;
}
std::vector < UiTexture* > Widget::addUiTexture(const std::string &name) {
    return UiResources::getUiTextures(name, this);
}
std::vector < ZTexture* > Widget::addTexture(const std::string &name) {
    return UiResources::getTextures(name, this);
}
void Widget::moveChildToFront(Widget* child) {
    if (child->getParent() == this) {
        Widget* next = child->getNextWidget();
        Widget* previous = child->getPreviousWidget();
        if (next && previous) {
            next->setPreviousWidget(previous);
            previous->setNextWidget(next);
            m_frontChild->setPreviousWidget(child);
            child->setNextWidget(m_frontChild);
            child->setPreviousWidget(nullptr);
            m_frontChild = child;
        }
        else if (previous) {
            m_backChild = previous;
            previous->setNextWidget(nullptr);
            m_frontChild->setPreviousWidget(child);
            child->setNextWidget(m_frontChild);
            child->setPreviousWidget(nullptr);
            m_frontChild = child;
        }
    }
}
void Widget::moveChildToBack(Widget* child) {
    if (child->getParent() == this) {
        Widget* next = child->getNextWidget();
        Widget* previous = child->getPreviousWidget();
        if (next && previous) {
            next->setPreviousWidget(previous);
            previous->setNextWidget(next);
            m_backChild->setNextWidget(child);
            child->setPreviousWidget(m_backChild);
            child->setNextWidget(nullptr);
            m_backChild = child;
        }
        else if (next) {
            m_frontChild = next;
            next->setNextWidget(nullptr);
            m_backChild->setNextWidget(child);
            child->setNextWidget(nullptr);
            child->setPreviousWidget(m_backChild);
            m_backChild = child;
        }
    }
}
void Widget::moveToFront() {
    if (m_parent) {
        m_parent->moveChildToFront(this);
        m_parent->moveToFront();
    }
}
void Widget::moveToBack() {
    if (m_parent) {
        m_parent->moveChildToBack(this);
    }
}
void Widget::moveChildToFront(Frame* frame) {
    if (frame->getParent() == this) {
        Widget* next = frame->getNextWidget();
        Widget* previous = frame->getPreviousWidget();
        if (next && previous) {
            next->setPreviousWidget(previous);
            previous->setNextWidget(next);
            m_frontFrame->setPreviousWidget(frame);
            frame->setNextWidget(m_frontFrame);
            frame->setPreviousWidget(nullptr);
            m_frontFrame = frame;
        }
        else if (previous) {
            m_backFrame = previous;
            previous->setNextWidget(nullptr);
            m_frontFrame->setPreviousWidget(frame);
            frame->setNextWidget(m_frontFrame);
            frame->setPreviousWidget(nullptr);
            m_frontFrame = frame;
        }
    }
}
void Widget::moveChildToBack(Frame* frame) {
    if (frame->getParent() == this) {
        Widget* next = frame->getNextWidget();
        Widget* previous = frame->getPreviousWidget();
        if (next && previous) {
            next->setPreviousWidget(previous);
            previous->setNextWidget(next);
            m_backFrame->setNextWidget(frame);
            frame->setPreviousWidget(m_backFrame);
            frame->setNextWidget(nullptr);
            m_backFrame = frame;
        }
        else if (next) {
            m_frontFrame = next;
            next->setNextWidget(nullptr);
            m_backFrame->setNextWidget(frame);
            frame->setNextWidget(nullptr);
            frame->setPreviousWidget(m_backFrame);
            m_backFrame = frame;
        }
    }
}
void Widget::moveChildToFront(GuiTexture* texture) {
    if (texture->getParent() == this) {
        Widget* next = texture->getNextWidget();
        Widget* previous = texture->getPreviousWidget();
        if (next && previous) {
            next->setPreviousWidget(previous);
            previous->setNextWidget(next);
            m_frontTexture->setPreviousWidget(texture);
            texture->setNextWidget(m_frontTexture);
            texture->setPreviousWidget(nullptr);
            m_frontTexture = texture;
        }
        else if (previous) {
            m_backTexture = previous;
            previous->setNextWidget(nullptr);
            m_frontTexture->setPreviousWidget(texture);
            texture->setNextWidget(m_frontTexture);
            texture->setPreviousWidget(nullptr);
            m_frontTexture = texture;
        }
    }
}
void Widget::moveChildToBack(GuiTexture* texture) {
    if (texture->getParent() == this) {
        Widget* next = texture->getNextWidget();
        Widget* previous = texture->getPreviousWidget();
        if (next && previous) {
            next->setPreviousWidget(previous);
            previous->setNextWidget(next);
            m_backTexture->setNextWidget(texture);
            texture->setPreviousWidget(m_backTexture);
            texture->setNextWidget(nullptr);
            m_backTexture = texture;
        }
        else if (next) {
            m_frontTexture = next;
            next->setNextWidget(nullptr);
            m_backTexture->setNextWidget(texture);
            texture->setNextWidget(nullptr);
            texture->setPreviousWidget(m_backTexture);
            m_backTexture = texture;
        }
    }
}
void Widget::align(int tag) {
    switch (tag) {
    case Centered:
    {
        setPositionPercent(glm::vec2(0.5f - m_dimentionsPercent.x / 2.0f, 0.5f - m_dimentionsPercent.y / 2.0f));
    }
    break;
    case Left:
    {
        setPositionPercent(glm::vec2(0.0f, 0.5f - m_dimentionsPercent.y / 2.0f));
    }
    break;
    case Right:
    {
        setPositionPercent(glm::vec2(1.0f - m_dimentionsPercent.x, 0.5f - m_dimentionsPercent.y / 2.0f));
    }
    break;
    case TopLeft:
    {
        setPositionPercent(glm::vec2(0.0f, 1.0f - m_dimentionsPercent.y));
    }
    break;
    case TopRight:
    {
        setPositionPercent(glm::vec2(1.0f - m_dimentionsPercent.x, 1.0f - m_dimentionsPercent.y));
    }
    break;
    case BottomLeft:
    {
        setPositionPercent(glm::vec2(0.0f, 0.0f));
    }
    break;
    case BottomRight:
    {
        setPositionPercent(glm::vec2(1.0f - m_dimentionsPercent.x, 0.0f));
    }
    break;
    }
}
void Widget::setEnabled(bool tag) {
    Enabled = tag;
    if (Enabled) {
        onEnabled();
    }
    else {
        onDisabled();
    }
}
Frame::Frame(const glm::vec2 &position, const glm::vec2 &dimentions, Widget* parent)
{
    m_dimentions = dimentions;
    m_position = position;
    m_parent = parent;
    if (m_parent) {
        m_position += parent->getPosition();
        m_positionPercent = (m_position - m_parent->getPosition()) / m_parent->getDimentions();
        m_dimentionsPercent = m_dimentions / m_parent->getDimentions();
        m_parent->addFrame(this);
    }
    m_originalPosition = getPosition();
    m_originalDimentions = getDimentions();
}
Frame::Frame(const glm::vec4 &posDimPercent, Widget* parent)
{
    m_parent = parent;
    m_positionPercent = glm::vec2(posDimPercent.x, posDimPercent.y);
    m_dimentionsPercent = glm::vec2(posDimPercent.z, posDimPercent.w);
    if (m_parent) {
        m_parent->addFrame(this);
    }
    m_originalPosition = getPosition();
    m_originalDimentions = getDimentions();
}
Frame::Frame(int tag, const glm::vec2 &dimPercent, Widget* parent)
{
    m_parent = parent;
    align(tag);
    if (m_parent) {
        m_parent->addFrame(this);
    }
    m_originalPosition = getPosition();
    m_originalDimentions = getDimentions();
}
void Frame::update(const InputManager &inputManager, std::vector < Widget* > &widgets) {
    for (Widget* it = m_frontChild; it != nullptr; it = it->getNextWidget()) {
        if (it->isEnabled()) {
            it->update(inputManager, widgets);
            widgets.push_back(it);
        }
    }
    for (Widget* it = m_frontFrame; it != nullptr; it = it->getNextWidget()) {
        if (it->isEnabled()) {
            it->update(inputManager, widgets);
        }
    }
}
GuiTexture::GuiTexture(const glm::vec2 &position, const glm::vec2 &dimentions, Widget* parent) {
    m_dimentions = dimentions;
    m_position = position;
    m_parent = parent;
    if (m_parent) {
        m_position += parent->getPosition();
        m_positionPercent = (m_position - m_parent->getPosition()) / m_parent->getDimentions();
        m_dimentionsPercent = m_dimentions / m_parent->getDimentions();
        m_parent->addGuiTexture(this);
    }
    m_originalPosition = getPosition();
    m_originalDimentions = getDimentions();
}
GuiTexture::GuiTexture(const glm::vec4 &posDimPercent, Widget* parent) {
    m_parent = parent;
    m_positionPercent = glm::vec2(posDimPercent.x, posDimPercent.y);
    m_dimentionsPercent = glm::vec2(posDimPercent.z, posDimPercent.w);
    if (m_parent) {
        m_parent->addGuiTexture(this);
    }
    m_originalPosition = getPosition();
    m_originalDimentions = getDimentions();
}
GuiTexture::GuiTexture(int tag, const glm::vec2 &dimPecent, Widget* parent) {
    m_parent = parent;
    align(tag);
    if (m_parent) {
        m_parent->addGuiTexture(this);
    }
    m_originalPosition = getPosition();
    m_originalDimentions = getDimentions();
}

UiTextureDef::UiTextureDef(const std::string &filePath) {
    load(filePath);
}
UiTextureDef::UiTextureDef(const Json::Value &root, const std::string &currentPath) {
    load(root, currentPath);
}

void UiTextureDef::load(const std::string &filePath) {
    load(json::parseFile(filePath.c_str()), getPath(filePath));
}
void UiTextureDef::load(const Json::Value &root, const std::string &currentPath) {
    using namespace Json;
    if (root.isObject()) {
        m_tileSheet = TileSheet(json::getCString(root["path"], ""));
        m_color = json::getColor(root["color"], WhiteRGBA8);
        m_bottomLeft = json::getVec4(root["bottomLeft"], glm::vec4());
        m_topLeft = json::getVec4(root["topLeft"], glm::vec4());
        m_bottomRight = json::getVec4(root["bottomRight"], glm::vec4());
        m_topRight = json::getVec4(root["topRight"], glm::vec4());
        m_middle = json::getVec4(root["middle"], glm::vec4());
        m_top = json::getVec4(root["top"], glm::vec4());
        m_bottom = json::getVec4(root["bottom"], glm::vec4());
        m_left = json::getVec4(root["left"], glm::vec4());
        m_right = json::getVec4(root["right"], glm::vec4());
    }
    else if (root.isString()) {
        load(currentPath + root.asString());
    }
    else fprintf(stderr, "%s", "Couldnt Find UiTexture Table Or FilePath");
}
UiTexture::UiTexture(const UiTextureDef& def, const glm::vec2 &position, const glm::vec2 &dimentions, Widget* parent)
: GuiTexture(position, dimentions, parent){
    copyFrom(def);
}
UiTexture::UiTexture(const UiTextureDef& def, const glm::vec4 &posDimPercent, Widget* parent)
: GuiTexture(posDimPercent, parent) {
    copyFrom(def);
}
void UiTexture::copyFrom(const UiTextureDef& def) {
    m_tileSheet = def.getTileSheet();
    m_color = def.getColor();
    m_bottomLeft = def.getBottomLeftUvs();
    m_topLeft = def.getTopLeftUvs();
    m_bottomRight = def.getBottomRightUvs();
    m_topRight = def.getTopRightUvs();
    m_middle = def.getMiddleUvs();
    m_top = def.getTopUvs();
    m_bottom = def.getBottomUvs();
    m_left = def.getLeftUvs();
    m_right = def.getRightUvs();
}
void UiTexture::draw(BatchRenderer &batchRenderer, GLSLProgram* program) {
    if (m_tileSheet.getTexture()) {
        const glm::vec4 destRect(m_position, m_dimentions);
        float maxWidthLeft = max(m_topLeft.z, m_left.z, m_bottomLeft.z);

        glm::vec4 bottomLeftUvs = m_tileSheet.getUvs(m_bottomLeft);
        glm::vec4 bottomLeft(m_position.x, m_position.y, m_bottomLeft.z, m_bottomLeft.w);
        applyBoundry(bottomLeftUvs, bottomLeft, destRect);

        glm::vec4 topLeftUvs = m_tileSheet.getUvs(m_topLeft);
        glm::vec4 topLeft(m_position.x, m_position.y + m_dimentions.y - m_topLeft.w, m_topLeft.z, m_topLeft.w);
        applyBoundry(topLeftUvs, topLeft, destRect);


        glm::vec4 bottomRightUvs = m_tileSheet.getUvs(m_bottomRight);
        glm::vec4 bottomRight(m_position.x + m_dimentions.x - m_bottomRight.z, m_position.y, m_bottomRight.z, m_bottomRight.w);
        applyBoundry(bottomRightUvs, bottomRight, destRect + glm::vec4(maxWidthLeft, 0, -maxWidthLeft, 0));

        glm::vec4 topRightUvs = m_tileSheet.getUvs(m_topRight);
        glm::vec4 topRight(m_position.x + m_dimentions.x - m_topRight.z, m_position.y + m_dimentions.y - m_topRight.w, m_topRight.z, m_topRight.w);
        applyBoundry(topRightUvs, topRight, destRect + glm::vec4(maxWidthLeft, m_bottomLeft.w, -maxWidthLeft, -m_bottomLeft.w));

        glm::vec4 rightUvs = m_tileSheet.getUvs(m_right);
        glm::vec4 right(m_position.x + m_dimentions.x - m_right.z, m_position.y + m_bottomRight.w, m_right.z, m_dimentions.y - m_bottomRight.w - m_topRight.w);
        applyBoundry(rightUvs, right, destRect + glm::vec4(maxWidthLeft, m_bottomLeft.w, -maxWidthLeft, -m_bottomLeft.w));

        batchRenderer.draw(bottomRight, bottomRightUvs, m_tileSheet.getTextureId(), getDepth(), program, m_color);
        batchRenderer.draw(topRight, topRightUvs, m_tileSheet.getTextureId(), getDepth(), program, m_color);
        batchRenderer.draw(right, rightUvs, m_tileSheet.getTextureId(), getDepth(), program, m_color);


        if (m_dimentions.x - m_bottomLeft.z - m_bottomRight.z > 0 && m_dimentions.y - m_bottom.w - m_top.w > 0) {
            glm::vec4 middleUvs = m_tileSheet.getUvs(m_middle);
            glm::vec4 middle(m_position.x + m_bottomLeft.z, m_position.y + m_bottom.w, m_dimentions.x - m_bottomLeft.z - m_bottomRight.z, m_dimentions.y - m_bottom.w - m_top.w);
            applyBoundry(middleUvs, middle, destRect);
            batchRenderer.draw(middle, middleUvs, m_tileSheet.getTextureId(), getDepth(), program, m_color);
        }

        if (m_dimentions.x - m_topLeft.z - m_topRight.z > 0) {
            glm::vec4 topUvs = m_tileSheet.getUvs(m_top);
            glm::vec4 top(m_position.x + m_topLeft.z, m_position.y + m_dimentions.y - m_top.w, m_dimentions.x - m_topLeft.z - m_topRight.z, m_top.w);
            applyBoundry(topUvs, top, destRect);
            batchRenderer.draw(top, topUvs, m_tileSheet.getTextureId(), getDepth(), program, m_color);
        }
        if (m_dimentions.x - m_bottomLeft.z - m_bottomRight.z > 0) {
            glm::vec4 bottomUvs = m_tileSheet.getUvs(m_bottom);
            glm::vec4 bottom(m_position.x + m_bottomLeft.z, m_position.y, m_dimentions.x - m_bottomLeft.z - m_bottomRight.z, m_bottom.w);
            applyBoundry(bottomUvs, bottom, destRect);
            batchRenderer.draw(bottom, bottomUvs, m_tileSheet.getTextureId(), getDepth(), program, m_color);
        }

        glm::vec4 leftUvs = m_tileSheet.getUvs(m_left);
        glm::vec4 left(m_position.x, m_position.y + m_bottomLeft.w, m_left.z, m_dimentions.y - m_bottomLeft.w - m_topLeft.w);
        applyBoundry(leftUvs, left, destRect);


        batchRenderer.draw(bottomLeft, bottomLeftUvs, m_tileSheet.getTextureId(), getDepth(), program, m_color);
        batchRenderer.draw(topLeft, topLeftUvs, m_tileSheet.getTextureId(), getDepth(), program, m_color);
        batchRenderer.draw(left, leftUvs, m_tileSheet.getTextureId(), getDepth(), program, m_color);

    }
    Frame::draw(batchRenderer, program);
}
ZTexture::ZTexture(const ZTextureDef& def, Widget* parent)
    : Frame(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), parent) {
    m_texture = def.getTexture();
    m_color = def.getColor();
}
ZTexture::ZTexture(const glm::vec4 &posDimPercent, Widget* parent)
: Frame(posDimPercent, parent){
    m_texture = {};
}
ZTexture::ZTexture(const glm::vec2& position, const glm::vec2& dimentions, Widget* parent)
: Frame(position, dimentions, parent) {
    m_texture = {};
}
ZTexture::ZTexture(const glm::vec2& position, const glm::vec2& dimentions, const ZTextureDef& def, Widget* parent)
: Frame(position, dimentions, parent) {
    m_texture = def.getTexture();
    m_color = def.getColor();
}
ZTexture::ZTexture(const glm::vec4 &posDimPercent, const ZTextureDef& def, Widget* parent)
    : Frame(posDimPercent, parent) {
    m_texture = def.getTexture();
    m_color = def.getColor();
}
void ZTexture::draw(BatchRenderer &batchRenderer, GLSLProgram* program){
    if (Enabled) {
        Frame::draw(batchRenderer, program);
        if (m_texture.texture)
            batchRenderer.draw(glm::vec4(m_position, m_dimentions), m_texture.uvs, m_texture.texture->id, getDepth(), program, m_color);
    }
}
ZTextureDef::ZTextureDef(const GLTexture* texture) {
    setTexture(texture);
    setColor(WhiteRGBA8);
}

ZTextureDef::ZTextureDef(const std::string &filePath) {
    load(filePath);
}
ZTextureDef::ZTextureDef(const Json::Value &root, const std::string &currentPath) {
    load(root, currentPath);
}

void ZTextureDef::load(const std::string &filePath) {
    load(json::parseFile(filePath.c_str()), getPath(filePath));
}
void ZTextureDef::load(const Json::Value &root, const std::string &currentPath) {
    using namespace Json;
    if (root.isObject()) {
        m_texture.texture = ResourceManager::getSingelton().getTexture(json::getCString(root["path"], ""));
        m_texture.uvs = json::getVec4(root["uvs"], glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
        if (m_texture.uvs.x > 1.0f || m_texture.uvs.y > 1.0f || m_texture.uvs.z > 1.0f || m_texture.uvs.w > 1.0f) {
            m_texture.uvs = TileSheet::getUvs(m_texture.uvs, m_texture.texture);
        }
        m_color = json::getColor(root["color"], WhiteRGBA8);
    }
    else if (root.isString()) {
        load(currentPath + root.asString());
    }
    else fprintf(stderr, "%s", "couldnt find Texture Objet or Path");
}
void ZTextureDef::setColor(const ColorRGBA8 &color) {
    m_color = color;
}
void ZTextureDef::setTexture(const Texture &texture) {
    m_texture = texture;
}
void ZTextureDef::setTexture(const GLTexture* texture) {
    m_texture = { texture , glm::vec4(0.0f, 0.0f, 1.0f, 1.0f) };
}
std::unordered_map < std::string, std::vector < UiTextureDef > > UiResources::m_uiTextureMap;
std::unordered_map < std::string, std::vector < ZTextureDef > > UiResources::m_textureMap;

std::string UiResources::m_copiedText;
void UiResources::dispose() {
    m_uiTextureMap.clear();
    m_textureMap.clear();
}

void UiResources::loadScheme(const std::string &filePath) {
    loadScheme(json::parseFile(filePath.c_str()), getPath(filePath));
}
void UiResources::loadSchemeFromBuffer(const std::string& buffer) {
    loadScheme(json::parseBuffer(buffer), "");
}
void UiResources::loadScheme(const Json::Value &root, const std::string &currentPath) {
    using namespace Json;
    if (root.isObject()) {
        m_uiTextureMap.clear();
        for (auto it = root.begin(); it != root.end(); it++) {
            if ((*it).isArray()) {
                std::vector < UiTextureDef > tvec((*it).size());
                for (unsigned int i = 0; i < (*it).size(); i++) {
                    tvec[i] = UiTextureDef((*it)[i], currentPath);
                }
                m_uiTextureMap[it.name()] = tvec;
            }
        }
    }
    else if (root.isString()) {
        loadScheme(currentPath + root.asString());
    }
    else fprintf(stderr, "%s", "Couldnt Find Scheme Object Or FilePath");
}

void UiResources::loadImageSets(const std::string &filePath) {
    loadImageSets(json::parseFile(filePath.c_str()), getPath(filePath));
}
void UiResources::loadImageSetsFromBuffer(const std::string &buffer) {
    loadImageSets(json::parseBuffer(buffer), "");
}
void UiResources::loadImageSets(const Json::Value &root, const std::string &currentPath) {
    using namespace Json;
    if (root.isObject()) {
        m_textureMap.clear();
        for (auto it = root.begin(); it != root.end(); it++) {
            if ((*it).isArray()) {
                std::vector < ZTextureDef > imageset((*it).size());
                for (unsigned int i = 0; i < (*it).size(); i++) {
                    imageset[i] = ZTextureDef((*it)[i], currentPath);
                }
                m_textureMap[it.name()] = imageset;
            }
        }
    }
    else if (root.isString()) {
        loadImageSets(currentPath + root.asString());
    }
    else fprintf(stderr, "%s", "Couldnt find ImageSets Object Or String");
}
void UiResources::setCopiedText(const std::string& text) {
    m_copiedText = text;
}
std::vector < UiTextureDef > UiResources::getUiTextureDef(const std::string &name) {
    if (name.size() > 0) {
        auto it = m_uiTextureMap.find(name);
        if (it != m_uiTextureMap.end()) {
            return it->second;
        }
        else fprintf(stderr, "%s%s", "Couldnt Find UiTexture : ", name.c_str());
    }
    else return std::vector < UiTextureDef >();
}
UiTexture* UiResources::getUiTexture(const std::string &name, Widget* parent) {
    std::vector < UiTextureDef > vec = getUiTextureDef(name);
    if (vec.size() > 0) {
        return new UiTexture(vec[0], glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), parent);
    }
    else return nullptr;
}
std::vector < UiTexture* > UiResources::getUiTextures(const std::string &name, Widget* parent) {
    std::vector < UiTextureDef > vec = getUiTextureDef(name);
    std::vector < UiTexture* >  output(vec.size());
    for (size_t i = 0; i < vec.size(); i++) {
        output[i] = new UiTexture(vec[i], glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), parent);
    }
    return output;
}
std::vector < ZTextureDef > UiResources::getTextureDef(const std::string &name) {
    if (name.size() > 0) {
        auto it = m_textureMap.find(name);
        if (it != m_textureMap.end()) {
            return it->second;
        }
        else fprintf(stderr, "%s%s", "Couldnt Find Texture : ", name.c_str());
    }
    else return std::vector < ZTextureDef >();
}
std::vector < ZTexture* > UiResources::getTextures(const std::string &name, Widget* parent) {
    std::vector < ZTextureDef > vec = getTextureDef(name);
    std::vector < ZTexture* > output(vec.size());
    for (size_t i = 0; i < vec.size(); i++) {
        output[i] = new ZTexture(vec[i], parent);
    }
    return output;
}
ZTexture* UiResources::getTexture(const std::string &name, Widget* parent) {
    std::vector < ZTextureDef > vec = getTextureDef(name);
    if (vec.size() > 0) {
        return new ZTexture(vec[0], parent);
    }
    else return nullptr;
}
const std::string& UiResources::getCopiedText() {
    return m_copiedText;
}
ZLabel::ZLabel(const glm::vec2 &position, const glm::vec2 &dimentions, Widget* parent)
: Frame(position, dimentions, parent){
    m_font = FontManager::get("Font/comic.ttf", 18);
}
ZLabel::ZLabel(const glm::vec4 &posDimPercent, Widget* parent) :
    Frame(posDimPercent, parent) {
    m_font = FontManager::get("Font/comic.ttf", 18);
}
ZLabel::ZLabel(int tag, const glm::vec2 &dimPercent, Widget* parent)
: Frame(tag, dimPercent, parent){
    m_font = FontManager::get("Font/comic.ttf", 18);
}
void ZLabel::update(const InputManager& inputManager, std::vector < Widget* > &list) {
    Frame::update(inputManager, list);
    updateTextItems();
}
void ZLabel::draw(BatchRenderer &batchRenderer, GLSLProgram* program) {
    Frame::draw(batchRenderer, program);
    updateTextItems();
    if (m_justification == Justification::MIDDLE) {
        m_textPosition.x = m_position.x + (m_dimentions.x / 2.0f);
    }
    else if (m_justification == Justification::RIGHT) {
        m_textPosition.x = m_position.x + m_dimentions.x;
    }
    batchRenderer.drawText(m_font, m_textPosition, m_renderedText, glm::vec2(m_scale.x, m_scaleY), getDepth(), program, Justification::LEFT, 255);
}
void ZLabel::updateTextItems() {
    if (m_dimentions.y < m_scale.y * m_font->getFontHeight()) {
        m_scaleY = m_dimentions.y / m_font->getFontHeight();
    }
    else {
        m_scaleY = m_scale.y;
    }
    m_textPosition = glm::vec2(m_position.x + m_padding.x, m_position.y + m_dimentions.y / 2.0f - m_font->getFontHeight() / 2.0 * m_scaleY);
    m_renderedText = m_text;
    while (m_font->mesureLine(m_renderedText) * m_scale.x > abs(m_dimentions.x - m_padding.x - m_padding.y)) {
        m_renderedText.popBack();
    }
}
void ZLabel::shrinkWidthToFit() {
    if (m_font) {
        setDimention(glm::vec2(m_font->mesureLine(m_text) + m_padding.x + m_padding.y, getDimentions().y));
    }
}
void ZLabel::shrinkHeightToFit() {
    if (m_font) {
        setDimention(glm::vec2(getDimentions().x, m_font->getFontHeight() * m_scale.y));
    }
}
GLSLProgram ZGUI::m_textureProgram;
void ZGUI::load() {
    m_textureProgram.compileShadersFromSource(GUI_VERT_SRC, GUI_FRAG_SRC);
    m_textureProgram.addAttribute("vertexPosition");
    m_textureProgram.addAttribute("vertexColor");
    m_textureProgram.addAttribute("vertexUV");
    m_textureProgram.linkShaders();
}
ZGUI::ZGUI()
{
}


ZGUI::~ZGUI()
{
    destroy();
}
void ZGUI::destroy() {
    m_batchRenderer.dispose();
    //m_textureProgram.destroy();
    m_window = nullptr;
    if (m_root) {
        delete m_root;
        m_root = nullptr;
    }
}
void ZGUI::init(Window* window) {
    destroy();
    m_tempWidgets.reserve(1024);
    m_batchRenderer.init();

    m_camera.init(window->getWidth(), window->getHeight());
    m_camera.setPosition(glm::vec2((float)window->getWidth() / 2.f,  (float)window->getHeight() / 2.f));
    m_window = window;
    m_root = new Widget(glm::vec2(0), glm::vec2((float)window->getWidth(), (float)window->getHeight()), nullptr);
    m_root->setGui(this);
}
void ZGUI::draw() {

    // Upload texture uniform
    GLint textureUniform = m_textureProgram.getUniformLocation("mySampler");
    glUniform1i(textureUniform, 0);
    glActiveTexture(GL_TEXTURE0);
    // Camera matrix
    glm::mat4 projectionMatrix = m_camera.getCameraMatrix();
    GLint pUniform = m_textureProgram.getUniformLocation("P");
    glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

    m_batchRenderer.begin(BatchSortType::FRONT_TO_BACK);
    m_root->draw(m_batchRenderer, &m_textureProgram);
    m_batchRenderer.render(projectionMatrix);

}
void ZGUI::update(const InputManager &inputManager) {
    m_camera.update();
    std::vector < Widget* > m_tempWidgets;
    m_tempWidgets.clear();
    m_root->update(inputManager, m_tempWidgets);
}
void ZGUI::addWidget(Widget* widget) {
    m_root->addChild(widget);
}
void ZGUI::onSDLEvent(SDL_Event &evnt) {
    m_root->onSDLEvent(evnt);
}
void ZGUI::onWindowSizeChanged() {
    m_root->setDimention(glm::vec2(m_window->getWidth(), m_window->getHeight()));
    m_camera.init(m_window->getWidth(), m_window->getHeight());
}


