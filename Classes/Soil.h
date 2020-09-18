#ifndef __Soil_H__
#define __Soil_H__

#include "SDL_Engine/SDL_Engine.h"
USING_NS_SDL;

class Crop;

class Soil : public Node
{
public:
	Soil();
	~Soil();
	static Soil* create(Sprite* sprite, int id, int level);
	bool init(Sprite* sprite, int id, int level);

	int getSoilID() const { return m_id; }
	int getSoilLv() const { return m_level; }
	void setSoilLv(int lv) { m_level = lv; }

	Sprite* getSprite() { return m_pSprite; }

	Crop* getCrop();
	void setCrop(Crop* crop);
private:
	Sprite* m_pSprite;
	int m_id;
	//TODO:当前的土地等级1-4
	int m_level;
	Crop* m_pCrop;
};
#endif
