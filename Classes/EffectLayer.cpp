#include "EffectLayer.h"
#include "CropLayer.h"
#include "Crop.h"

const int EffectLayer::ANIMATION_TAG = 1;

EffectLayer::EffectLayer()
	:m_pRipeSprite(nullptr)
{
}

EffectLayer::~EffectLayer()
{
}

bool EffectLayer::init()
{
	//农场相关
	_eventDispatcher->addEventCustomListener(CropLayer::CUSTOM_EVENT_STRING,
			SDL_CALLBACK_1(EffectLayer::effectCallback, this), this);
	return true;
}

void EffectLayer::showRipeEffect(Crop* crop)
{
	//当前没有作物成熟并且存在成熟特效，则删去
	if (crop == nullptr && m_pRipeSprite != nullptr)
	{
		m_pRipeSprite->stopActionByTag(ANIMATION_TAG);
		this->pushSpriteToPool(m_pRipeSprite);

		m_pRipeSprite->setUserObject(nullptr);
		m_pRipeSprite->removeFromParent();
		m_pRipeSprite = nullptr;
	}
	else if (crop != nullptr 
		&& (m_pRipeSprite == nullptr || m_pRipeSprite->getUserObject() != crop))
	{
		auto pos = crop->getPosition();
		auto size = crop->getContentSize();
		auto anchor = crop->getAnchorPoint();

		pos.y -= size.height * anchor.y;
		//获取成熟特效
		if (m_pRipeSprite == nullptr)
		{
			m_pRipeSprite = this->popSpriteFromPool();
			//设置贴图
			auto frameCache = Director::getInstance()->getSpriteFrameCache();
			auto frame = frameCache->getSpriteFrameByName("farm_ui_ripe.png");
			m_pRipeSprite->setSpriteFrame(frame);
			this->addChild(m_pRipeSprite);
		}
		//设置位置
		auto ripeSize = m_pRipeSprite->getContentSize();
		pos.y -= ripeSize.height / 2;
		m_pRipeSprite->setPosition(pos);
		//设置动作
		MoveBy* move1 = MoveBy::create(0.5f, Point(0, 10));
		MoveBy* move2 = move1->reverse();

		auto seq = Sequence::createWithTwoActions(move1, move2);
		RepeatForever* repeat = RepeatForever::create(seq);
		repeat->setTag(ANIMATION_TAG);
		
		m_pRipeSprite->stopActionByTag(ANIMATION_TAG);
		m_pRipeSprite->runAction(repeat);
		m_pRipeSprite->setUserObject(crop);
	}
}

Sprite* EffectLayer::popSpriteFromPool()
{
	Sprite* sprite = nullptr;

	if (m_spritePool.empty())
	{
		sprite = Sprite::create();
	}
	else
	{
		sprite = m_spritePool.back();
		sprite->autorelease();

		m_spritePool.pop_back();
	}
	return sprite;
}

void EffectLayer::pushSpriteToPool(Sprite* sprite)
{
	SDL_SAFE_RETAIN(sprite);
	sprite->setLocalZOrder(0);
	sprite->setGlobalZOrder(0);
	sprite->setFlipX(false);
	sprite->setRotation(0.f);
	sprite->removeFromParent();
	sprite->setAnchorPoint(Point(0.5f, 0.5f));

	m_spritePool.push_back(sprite);
}

void EffectLayer::effectCallback(EventCustom* eventCustom)
{
	//作物成熟
	if (eventCustom->getEventName() == CropLayer::CUSTOM_EVENT_STRING)
	{
		auto crop = static_cast<Crop*>(eventCustom->getUserData());
		this->showRipeEffect(crop);
	}
}
