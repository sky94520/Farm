#ifndef __FarmScene_H__
#define __FarmScene_H__

#include "SDL_Engine/SDL_Engine.h"

USING_NS_SDL;

class SoilLayer;
class CropLayer;

class FarmScene : public Scene
{
public:
	FarmScene();
	~FarmScene();
	CREATE_FUNC(FarmScene);
	bool init();

public:
	//处理触碰事件
	bool handleTouchEvent(Touch* touch, SDL_Event* event);
private:
	bool preloadResources();
	//初始化土壤和作物
	void initializeSoilsAndCrops();
private:
	SoilLayer* m_pSoilLayer;
	CropLayer* m_pCropLayer;
};
#endif
