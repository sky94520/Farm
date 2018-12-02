#include "Soil.h"

Soil::Soil()
	:m_pSprite(nullptr)
	,m_id(0)
	,m_level(1)
	,m_pCrop(nullptr)
{
}

Soil::~Soil()
{
	SDL_SAFE_RELEASE(m_pSprite);
}

Soil* Soil::create(Sprite* sprite,int id, int level)
{
	auto soil = new Soil();

	if (soil && soil->init(sprite, id, level))
		soil->autorelease();
	else
		SDL_SAFE_DELETE(soil);

	return soil;
}

bool Soil::init(Sprite* sprite, int id, int level)
{
	SDL_SAFE_RETAIN(sprite);
	SDL_SAFE_RELEASE(m_pSprite);

	m_pSprite = sprite;
	m_id = id;
	m_level = level;

	this->setContentSize(m_pSprite->getContentSize());
	this->setAnchorPoint(Point(0.5f, 0.5f));

	return true;
}

Crop* Soil::getCrop()
{
	return m_pCrop;
}

void Soil::setCrop(Crop* crop)
{
	m_pCrop = crop;
}
