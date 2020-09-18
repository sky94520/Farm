#ifndef __EffectLayer_H__
#define __EffectLayer_H__
#include <vector>

#include "SDL_Engine/SDL_Engine.h"

USING_NS_SDL;
using namespace std;

class Crop;

class EffectLayer : public Layer
{
private:
	static const int ANIMATION_TAG;
private:
	vector<Sprite*> m_spritePool;
	//农场
	//成熟特效
	Sprite* m_pRipeSprite;
public:
	EffectLayer();
	~EffectLayer();

	CREATE_FUNC(EffectLayer);
	bool init();
private:
	//展示果实成熟动作
	void showRipeEffect(Crop* crop);
private:
	Sprite* popSpriteFromPool();
	void pushSpriteToPool(Sprite* sprite);
	//农场相关
	//调用特效
	void effectCallback(EventCustom* eventCustom);
};
#endif
