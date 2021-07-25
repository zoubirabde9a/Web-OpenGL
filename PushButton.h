#pragma once
#include "Button.h"
#include <functional>

class PushButton : public Button
{
public:
	enum {
	 NORMAL, Hover, PUSHED , INACTIVE
	};
	PushButton(const glm::vec2 &position, const glm::vec2 &dimentions, const char* textureName, Widget* parent);
	PushButton(const glm::vec4 &posDimPercent, const char* textureName, Widget* parent);
	PushButton(int tag, const glm::vec2 &dimPercent, const char* textureName, Widget* parent);

	virtual void setActive(bool Bool);
	virtual void setNormal()override;
	virtual void setHover()override;
	virtual void setPushed()override;

protected:
	std::vector < UiTexture* > m_textures;
};

