#ifndef __Entity_H__
#define __Entity_H__

#include<string>
#include "SDL_Engine/SDL_Engine.h"

using namespace SDL;
using namespace std;

class Entity:public Node
{
public:
	Entity();
	~Entity();
	CREATE_FUNC(Entity);
	bool init();
	Sprite* getSprite() const;
	//和bind不同，此函数不改变content size
	void setSprite(Sprite* sprite);
	void bindSprite(Sprite* sprite);

	Sprite* bindSpriteWithSpriteFrame(SpriteFrame* spriteFrame);
	Sprite* bindSpriteWithSpriteFrameName(const string& spriteName);
	//以animation 的第一帧为贴图 并且运行该动画
	Sprite* bindSpriteWithAnimate(Animate* animate);
	void unbindSprite();
	//创建动画
	static Animate* createAnimate(const string& format, int begin, int end
			, float delayPerUnit, unsigned int loops = -1);
public:
	static const int ANIMATION_TAG;
	static const int ACTION_TAG;
protected:
	Sprite* m_pSprite;
};
#endif
