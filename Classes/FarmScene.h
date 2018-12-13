#ifndef __FarmScene_H__
#define __FarmScene_H__

#include "SDL_Engine/SDL_Engine.h"

#include "FarmUILayer.h"
#include "GoodLayer.h"

USING_NS_SDL;

class SoilLayer;
class CropLayer;

class FarmScene : public Scene
		  , public FarmUILayerDelegate, public GoodLayerDelegate
{
public:
	FarmScene();
	~FarmScene();
	CREATE_FUNC(FarmScene);
	bool init();
	void update(float dt);
public:
	//处理触碰事件
	bool handleTouchEvent(Touch* touch, SDL_Event* event);
public://委托
	virtual void harvestCrop(Crop* crop);
	virtual void shovelCrop(Crop* crop);
	virtual void fightCrop(Crop* crop);
	virtual void showWarehouse();
	virtual void showShop();
	virtual void saveData();
public:
	virtual void pageBtnCallback(GoodLayer* goodLayer, int value);
	virtual void useBtnCallback(GoodLayer* goodLayer);
	virtual void equipBtnCallback(GoodLayer* goodLayer);
	virtual void closeBtnCallback(GoodLayer* goodLayer);
	virtual void selectGoodCallback(GoodLayer* goodLayer, GoodInterface* good);
private:
	bool preloadResources();
	//初始化土壤和作物
	void initializeSoilsAndCrops();
	//是否显示物品层
	void setVisibleofGoodLayer(bool visible);
	//稍微封装了一下DynamicData
	Value getValueOfKey(const string& key);
private:
	SoilLayer* m_pSoilLayer;
	CropLayer* m_pCropLayer;
	FarmUILayer* m_pFarmUILayer;
	GoodLayer* m_pGoodLayer;
};
#endif
