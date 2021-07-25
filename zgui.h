#ifndef ZGUI_H
#define ZGUI_H

#include <string>
#include <vector>
#include "tilesheet.h"
#include "inputmanager.h"
#include "camera2d.h"
#include "window.h"
#include "fontmanager.h"
#include "batchrenderer.h"

struct Texture{
    const GLTexture* texture;
    glm::vec4 uvs;
};

namespace Json {
    class Value;
}
enum EventType {
    BUTTON_CLICKED, BUTTON_DOWN, BUTTON_HOVER, VALUE_CHANGED
};
class FunctionSlotBase {
public:
    virtual ~FunctionSlotBase() {};
    virtual void DoFunction() = 0;
};
template <typename T>
class FunctionSlot : public FunctionSlotBase {
public:
    typedef bool(T::*MemberFunctionType)();
    FunctionSlot(MemberFunctionType func, T* obj) {
        d_function = func;
        d_object = obj;
    }
    virtual void DoFunction() {
        (d_object->*d_function)();
    }
    MemberFunctionType d_function;
    T* d_object;
};
class GuiTexture;
class UiTexture;
class ZTexture;
class ZGUI;
class Frame;
class Layout;
enum WidgetPos {
    NoPos, Centered, Left, Right, TopLeft, TopRight, BottomLeft, BottomRight
};
class Widget {
public:
    static bool isMouseOnWidgets(const std::vector < Widget* > &vec, const glm::vec2 &mousePosition);
    static bool isMouseOnWidgets(Widget* list, const glm::vec2 &mousePosition);
public:
    Widget() { ; }
    Widget(const glm::vec2 &position, const glm::vec2 &dimentions, Widget* parent);
    Widget(const glm::vec4 &posDimPercent, Widget* parent);
    Widget(int tag, const glm::vec2 &dimPecent, Widget* parent);

    virtual ~Widget();

    void addChild(Widget* child);
    void addFrame(Frame* frame);
    void addFrameFront(Frame* frame);
    void addGuiTexture(GuiTexture* texture);
    void removeChild(Widget* child);
    void destroyChild(Widget* child);
    void destroyFrame(Frame* frame);
    void destroyChild(const std::vector < Widget* > &children);
    void destroyChildren();
    std::vector < UiTexture* > addUiTexture(const std::string &name);
    std::vector < ZTexture* > addTexture(const std::string &name);

    virtual void draw(BatchRenderer &batchRenderer, GLSLProgram* program);
    virtual void update(const InputManager &inputManager, std::vector < Widget* > &widgets);
    virtual void onSDLEvent(SDL_Event &evnt);

    void updateLayout();

    void setShadow(bool tag);
    void setStatic(bool tag);
    void setRelativePosition(const glm::vec2 &position);
    void setPosition(const glm::vec2 &position);
    void offsetPosition(const glm::vec2 &offset);
    void setDimention(const glm::vec2 &dimentions);
    void offsetDimentions(const glm::vec2 &offset);
    void setPositionPercent(const glm::vec2 &positionPercent);
    void setYPercent(float yPercent);
    void setXPercent(float xPercent);
    void setDimentionsPercent(const glm::vec2 &dimentionsPercent);
    void setWidthPercent(float widthPercent);
    void setHeightPercent(float heightPercent);
    void setOriginalPosition(const glm::vec2& position);
    void setOriginalDimentions(const glm::vec2& dimentions);
    void setOffset(const glm::vec2& offset);
    void setOffsetX(float offset);
    void setOffsetY(float offset);
    void setLayout(Layout* layout);
    void setDepth(float depth);
    void resetOriginalPosition();
    void resetOriginalDimentions();
    void setParent(Widget* parent) { m_parent = parent;}
    void setSelected(bool tag);
    void moveChildToFront(Widget* child);
    void moveChildToBack(Widget* child);
    void moveChildToFront(Frame* frame);
    void moveChildToBack(Frame* frame);
    void moveChildToFront(GuiTexture* texture);
    void moveChildToBack(GuiTexture* texture);
    void moveToFront();
    void moveToBack();


    void align(int tag);

    void setEnabled(bool tag);
    void setGui(ZGUI* gui) { m_gui = gui; }
    void toggleEnabled() {
        if (Enabled)Enabled = false;
        else Enabled = true;
    }
    void setFixedSize(bool tag);
    bool isMouseOnWidget(const glm::ivec2 &mousePos)const;
    bool isMouseOnChildren(const glm::ivec2 &mousePos)const;
    bool isMouseOnAnyChild(const glm::ivec2 &mousePos)const;
    bool isAnyChildSelected() const;
    bool isEnabled() const { return Enabled; }
    bool isStatic() const { return m_static; }
    inline const glm::vec2 &getPosition() const { return m_position; }
    inline const glm::vec2 &getDimentions() const { return m_dimentions; }
    inline const glm::vec2 &getPositionPercent() const { return m_positionPercent; }
    inline const glm::vec2 &getDimentionsPercent() const { return m_dimentionsPercent; }
    inline const glm::vec2 &getOriginalPosition() const { return m_originalPosition; }
    inline const glm::vec2 &getOffset() const { return m_offset; }
    inline Widget* getFrontChild() const { return m_frontChild; }
    inline Widget* getBackChild() const { return m_backChild; }
    inline Widget* getFrontFrame() const { return m_frontFrame; }
    inline Widget* getBackFrame() const { return m_backFrame; }
    inline Widget* getFrontTexture() const { return m_frontTexture; }
    inline Widget* getBackTexture() const { return m_backTexture; }
    inline size_t getChildrenSize() const { return m_childrenSize; }
    inline Widget* getNextWidget() { return m_nextWidget; }
    inline Widget* getPreviousWidget() { return m_previousWidget; }
    float getDepth() const;
    Widget* getParent() { return m_parent; }
    bool isSelected() const;
protected:
    void setNextWidget(Widget* next);
    void setPreviousWidget(Widget* previous);
    void pushChildBack(Widget* child);
    void pushChildFront(Widget* child);
    void pushFrameBack(Widget* frame);
    void pushFrameFront(Widget* frame);
    void pushTextureBack(Widget* texture);
    void pushTextureFront(Widget* texture);
    void updateDestRect();
    void updateDestRectPercent();
    void destroy();
    void unselectChildren();

    virtual void onSelected() {}
    virtual void onUnselected() {}
    virtual void onEnabled() {}
    virtual void onDisabled() {}
    virtual void onResize() {}
    virtual void onHover(const InputManager &inputManager) {}
    virtual void onNotHover(const InputManager &inputManager) {}
protected:
    bool Enabled = true;
    bool m_fixedSize = false;
    bool m_selected = false;
    bool m_static = false;
    bool m_clickInside = false;
    bool m_shadow = false;
    Widget* m_parent = nullptr;
    ZGUI* m_gui = nullptr;
    float m_depth = 0.0;
    glm::vec2 m_position;
    glm::vec2 m_positionPercent;
    glm::vec2 m_dimentions;
    glm::vec2 m_dimentionsPercent;
    glm::vec2 m_originalPosition;
    glm::vec2 m_originalDimentions;
    glm::vec2 m_offset;
    Widget* m_nextWidget = nullptr;
    Widget* m_previousWidget = nullptr;
    Widget* m_frontChild = nullptr;
    Widget* m_backChild = nullptr;
    size_t m_childrenSize = 0;
    Widget* m_frontFrame = nullptr;
    Widget* m_backFrame = nullptr;
    size_t m_framesSize = 0;
    Widget* m_frontTexture = nullptr;
    Widget* m_backTexture = nullptr;
    size_t m_texturesSize = 0;
    Layout* m_layout = nullptr;
};
class Frame : public Widget {
public:
    Frame(){}
    Frame(const glm::vec2 &position, const glm::vec2 &dimentions, Widget* parent);
    Frame(const glm::vec4 &posDimPercent, Widget* parent);
    Frame(int tag, const glm::vec2 &dimPecent, Widget* parent);
    virtual void update(const InputManager &inputManager, std::vector < Widget* > &widgets)override;
};
class GuiTexture : public Frame{
public:
    GuiTexture() : Frame() {}
    GuiTexture(const glm::vec2 &position, const glm::vec2 &dimentions, Widget* parent);
    GuiTexture(const glm::vec4 &posDimPercent, Widget* parent);
    GuiTexture(int tag, const glm::vec2 &dimPecent, Widget* parent);
};
class UiTextureDef {
public:
    UiTextureDef(){}

    UiTextureDef(const std::string &filePath);
    UiTextureDef(const Json::Value &root, const std::string &currentPath);

    void load(const std::string &filePath);
    void load(const Json::Value &root, const std::string &currentPath);
    void setColor(const ColorRGBA8 &color) { m_color = color; }

    glm::vec2 getBottomLeftOffset() const { return glm::vec2(m_bottomLeft.z, m_bottomLeft.w); }
    glm::vec2 getTopLeftOffset() const { return glm::vec2(m_topLeft.z, m_topLeft.w); }
    glm::vec2 getBottomRightOffset() const { return glm::vec2(m_bottomRight.z, m_bottomRight.w); }
    glm::vec2 getTopRightOffset() const { return glm::vec2(m_topRight.z, m_topRight.w); }

    const glm::vec4 &getBottomLeftUvs() const { return m_bottomLeft; }
    const glm::vec4 &getTopLeftUvs() const { return m_topLeft; }
    const glm::vec4 &getBottomRightUvs() const { return m_bottomRight; }
    const glm::vec4 &getTopRightUvs() const { return m_topRight; }
    const glm::vec4 &getMiddleUvs() const { return m_middle; }
    const glm::vec4 &getTopUvs() const { return m_top; }
    const glm::vec4 &getBottomUvs() const { return m_bottom; }
    const glm::vec4 &getLeftUvs() const { return m_left; }
    const glm::vec4 &getRightUvs() const { return m_right; }

    const TileSheet &getTileSheet() const { return m_tileSheet; }
    const ColorRGBA8 &getColor() const { return m_color; }
protected:
    TileSheet m_tileSheet;
    ColorRGBA8 m_color = WhiteRGBA8;
    glm::vec4 m_bottomLeft;
    glm::vec4 m_topLeft;
    glm::vec4 m_bottomRight;
    glm::vec4 m_topRight;
    glm::vec4 m_middle;
    glm::vec4 m_top;
    glm::vec4 m_bottom;
    glm::vec4 m_left;
    glm::vec4 m_right;
};
class UiTexture : public GuiTexture, public UiTextureDef {
public:
    UiTexture(): GuiTexture() {}
    UiTexture(const UiTextureDef& def, const glm::vec2 &position, const glm::vec2 &dimentions, Widget* parent);
    UiTexture(const UiTextureDef& def, const glm::vec4 &posDimPercent, Widget* parent);
    virtual void draw(BatchRenderer &batchRenderer, GLSLProgram* program);
private:
    void copyFrom(const UiTextureDef& def);
};

class ZTextureDef {
public:
    ZTextureDef(){}
    ZTextureDef(const GLTexture* texture);

    ZTextureDef(const std::string &filePath);
    ZTextureDef(const Json::Value &root, const std::string &currentPath);

    void load(const std::string &filePath);
    void load(const Json::Value &root, const std::string &currentPath);

    void setColor(const ColorRGBA8 &color);
    void setTexture(const Texture &texture);
    void setTexture(const GLTexture* texture);

    inline const Texture &getTexture() const { return m_texture; }
    inline const ColorRGBA8 &getColor() const { return m_color; }
protected:
    Texture m_texture = {};
    ColorRGBA8 m_color = WhiteRGBA8;
};
class ZTexture : public Frame, public ZTextureDef {
public:
    ZTexture() : Frame(){};
    ZTexture(const ZTextureDef& def, Widget* parent);
    ZTexture(const glm::vec2& position, const glm::vec2& dimentions, const ZTextureDef& def, Widget* parent);
    ZTexture(const glm::vec4 &posDimPercent, const ZTextureDef& def, Widget* parent);
    ZTexture(const glm::vec4 &posDimPercent, Widget* parent);
    ZTexture(const glm::vec2& position, const glm::vec2& dimentions, Widget* parent);

    virtual void draw(BatchRenderer &batchRenderer, GLSLProgram* program)override;
};
class UiResources
{
public:
    UiResources() {}
    static void dispose();

    static void loadScheme(const std::string &filePath);
    static void loadSchemeFromBuffer(const std::string& buffer);
    static void loadScheme(const Json::Value &root, const std::string &currentPath);

    static void loadImageSets(const std::string &filePath);
    static void loadImageSetsFromBuffer(const std::string &filePath);
    static void loadImageSets(const Json::Value &root, const std::string &currentPath);
    static void setCopiedText(const std::string& text);

    static std::vector < UiTextureDef > getUiTextureDef(const std::string &name);
    static UiTexture* getUiTexture(const std::string &name, Widget* parent);
    static std::vector < UiTexture* > getUiTextures(const std::string &name, Widget* parent);
    static std::vector < ZTextureDef > getTextureDef(const std::string &name);
    static std::vector < ZTexture* > getTextures(const std::string &name, Widget* parent);
    static ZTexture* getTexture(const std::string &name, Widget* parent);
    static const std::string& getCopiedText();
private:
    static std::string m_copiedText;
    static std::unordered_map < std::string, std::vector < UiTextureDef > > m_uiTextureMap;
    static std::unordered_map < std::string, std::vector < ZTextureDef > > m_textureMap;
};
class ZLabel : public Frame {
public:
    ZLabel() {}
    ZLabel(const glm::vec2 &position, const glm::vec2 &dimentions, Widget* parent);
    ZLabel(const glm::vec4 &posDimPercent, Widget* parent);
    ZLabel(int tag, const glm::vec2 &dimPercent, Widget* parent);

    void update(const InputManager& inputManager, std::vector < Widget* > &list)override;
    virtual void draw(BatchRenderer &batchRenderer, GLSLProgram* program)override;

    void shrinkWidthToFit();
    void shrinkHeightToFit();
    inline void setText(const TextRGB8 &text) { m_text = text; updateTextItems(); }
    inline void popText() { if (m_text.size() > 0) m_text.popFront(); }
    inline void appendText(const TextRGB8 &text) { m_text.append(text); }
    inline void clear() { m_text.clear(); }
    inline void setScale(const glm::vec2 &scale) { m_scale = scale; }
    inline void setJustification(const Justification &j) { m_justification = j; }
    inline void setColor(const ColorRGBA8 &color) { m_color = color; }
    inline void setTextColor(const ColorRGB8& color) { m_text.setColor(color); }
    inline void setPadding(float padding) { m_padding = glm::vec2(padding); }
    inline void setPadding(const glm::vec2 &padding) { m_padding = padding; }
    inline void setFont(SpriteFont* font) { m_font = font; }

    const TextRGB8 &getText() const { return m_text;  }
    const TextRGB8 &getRendererdText() const { return m_renderedText; }
    const glm::vec2 &getScale() const { return m_scale; }
    const glm::vec2 &getTextPosition() const { return m_textPosition; }
    const glm::vec2 &getPadding() const { return m_padding; }
    SpriteFont* getFont() const { return m_font; }
private:
    void updateTextItems();
private:
    glm::vec2 m_scale = glm::vec2(1.0f);
    float m_scaleY = 1.0f;
    glm::vec2 m_textPosition;
    TextRGB8 m_text;
    glm::vec2 m_padding = glm::vec2(4.0f);
    ColorRGBA8 m_color = WhiteRGBA8;
    TextRGB8 m_renderedText;
    Justification m_justification = Justification::LEFT;
    SpriteFont* m_font = nullptr;
};
class Layout {
public:
    Layout(){}
    virtual ~Layout(){}
    virtual void layout(Widget* widget) = 0;
};
class ZGUI
{
public:
    static void load();
public:
    ZGUI();
    ~ZGUI();
    void init(Window* window);
    void destroy();
    void draw();
    void update(const InputManager &inputManager);
    void onSDLEvent(SDL_Event &evnt);
    void addWidget(Widget* widget);
    Widget* getRoot() { return m_root; }
//	static lua_State* getLuaState() { return m_L; }
public:
    void onWindowSizeChanged();
private:
    std::vector < Widget* > m_tempWidgets;
    BatchRenderer m_batchRenderer;
    static GLSLProgram m_textureProgram;
    Camera2D m_camera;
    Widget* m_root = nullptr;
    Window* m_window;
};

#endif // ZGUI_H
