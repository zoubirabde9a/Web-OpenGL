#pragma once
#include "Button.h"
#include "TileSheet.h"

class ImageButton : public Button
{
public:
	enum {
		NORMAL, Hover, PUSHED, INACTIVE
	};
public:
	ImageButton() { ; }
	ImageButton(const glm::vec2 &position, const glm::vec2 &dimentions, Widget* parent);
	ImageButton(const glm::vec4 &posDimPercent, Widget* parent);
	ImageButton(const char* textureName, const glm::vec2 &position, const glm::vec2 &dimentions, Widget* parent);
	ImageButton(const char* textureName, const glm::vec4 &posDimPercent, Widget* parent);

	void setSelectionActive(bool Bool) {m_selectionActive = Bool;}
	void setTexture(const char* textureName);
	virtual void setActive(bool Bool);
	virtual void setNormal()override;
	virtual void setHover()override;
	virtual void setPushed()override;
    
	bool isHover() const;
protected:
	bool m_selectionActive = true;
	std::vector < ZTexture* > m_textures;
};


