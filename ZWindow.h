#pragma once
#include "ZGUI.h"
#include "ImageButton.h"
#include "PushButton.h"

class ZWindow : public Widget {
public:
    ZWindow(){}
    ZWindow(const glm::vec2 &position, const glm::vec2 &dimentions, const char* textureName, Widget* parent);
    ZWindow(const glm::vec4 &posDimPercent, const char* textureName, Widget* parent);
    ZWindow(int tag, const glm::vec2 &dimentions, const char* textureName, Widget* parent);

    virtual void update(const InputManager &inputManager, std::vector < Widget* > &widgets) override;

    void setLabelSize(float size);
    void setTitle(const TextRGB8 &title);
    void setFont(SpriteFont* font);

    inline SpriteFont* getFont() const { return m_label->getFont(); }
    inline float getLabelSize() const { return m_labelSize; }
    inline ImageButton* getButton() { return m_button; }
    inline ZLabel* getLabel() { return m_label; }
private:
    void updateItems();
    virtual void onResize() override;
private:
    bool m_pushed = false;
    ZLabel* m_label = nullptr;
    ImageButton* m_button = nullptr;
    float m_labelSize = 20.0f;
};
