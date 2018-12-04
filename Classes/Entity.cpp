#include "Entity.h"

const int Entity::ANIMATION_TAG = 100;
const int Entity::ACTION_TAG = 101;

Entity::Entity()
	:m_pSprite(nullptr)
{
}

Entity::~Entity()
{
}

void Entity::setSprite(Sprite* sprite)
{
	if(m_pSprite)
		m_pSprite->removeFromParent();

	m_pSprite = sprite;
	Size size = this->getContentSize();

	m_pSprite->setPosition(size.width / 2, size.height / 2);
	this->addChild(m_pSprite);
}

void Entity::bindSprite(Sprite* sprite)
{
	if(m_pSprite)
		m_pSprite->removeFromParent();

	m_pSprite = sprite;
	auto size = m_pSprite->getContentSize();

	this->setContentSize(size);
	m_pSprite->setPosition(size.width / 2, size.height / 2);
	this->addChild(m_pSprite);
}

Sprite* Entity::bindSpriteWithSpriteFrame(SpriteFrame* spriteFrame)
{
	if(spriteFrame != nullptr)
	{
		Sprite* sprite = Sprite::createWithSpriteFrame(spriteFrame);
		Entity::bindSprite(sprite);

		return sprite;
	}
	return nullptr;
}

Sprite* Entity::bindSpriteWithSpriteFrameName(const string& spriteName)
{
	//获取精灵帧
	auto frameCache = Director::getInstance()->getSpriteFrameCache();
	auto spriteFrame = frameCache->getSpriteFrameByName(spriteName);
	
	return this->bindSpriteWithSpriteFrame(spriteFrame);
}

Sprite*Entity::bindSpriteWithAnimate(Animate* animate)
{
	auto animation = animate->getAnimation();
	auto firstFrame = animation->getFrames().front()->getSpriteFrame();	
	auto sprite = this->bindSpriteWithSpriteFrame(firstFrame);
	//运行动画
	sprite->runAction(animate);

	return sprite;
}

void Entity::unbindSprite()
{
	if (m_pSprite != nullptr)
	{
		m_pSprite->removeFromParent();
		m_pSprite = nullptr;
	}
}

Sprite*Entity::getSprite()const
{
	return m_pSprite;
}

Animate* Entity::createAnimate(const string& format, int begin, int end
		, float delayPerUnit, unsigned int loops)
{
	vector<SpriteFrame*> frames;
	auto frameCache = Director::getInstance()->getSpriteFrameCache();
	//添加资源
	for(int i = begin;i <= end;i++)
	{
		auto frame = frameCache->getSpriteFrameByName(StringUtils::format(format.c_str(),i));
		frames.push_back(frame);
	}
	Animation*animation = Animation::createWithSpriteFrames(frames,delayPerUnit,loops);
	return Animate::create(animation);
}
