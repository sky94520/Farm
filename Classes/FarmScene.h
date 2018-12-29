#ifndef __FarmScene_H__
#define __FarmScene_H__
#include <vector>

#include "SDL_Engine/SDL_Engine.h"

#include "FarmUILayer.h"
#include "GoodLayer.h"

using namespace std;
USING_NS_SDL;

class SoilLayer;
class CropLayer;
class Good;
class Soil;
class SliderDialog;

/*物品层显示类型*/
enum class GoodLayerType
{
	None,
	Warehouse, //仓库
	Shop, //商店
	SeedBag, //种子背包
};

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
private:
	//显示种子背包
	void showSeedBag();
public:
	virtual void saveData();
public:
	virtual void pageBtnCallback(GoodLayer* goodLayer, int value);
	virtual void useBtnCallback(GoodLayer* goodLayer);
	virtual void equipBtnCallback(GoodLayer* goodLayer);
	virtual void closeBtnCallback(GoodLayer* goodLayer);
	virtual void selectGoodCallback(GoodLayer* goodLayer, GoodInterface* good);
private:
	//滑动条对话框回调函数
	void sliderDialogCallback(bool ret, int percent);
private:
	bool preloadResources();
	//初始化土壤和作物
	void initializeSoilsAndCrops();
	//初始化商店物品列表
	void initializeShopGoods();
	//是否显示物品层
	void setVisibleofGoodLayer(bool visible);
	/**
	 * 显示物品层
	 * @param titleFrameName 标题贴图名
	 * @param btnFrameName 按钮贴图名
	 * @param list 显示的列表
	 * @param curPage 当前页面 如果出界则默认为第一页
	 */
	void showGoodLayer(const string& titleFrameName, const string& btnFrameName
			, const vector<Good*>* list, int curPage = 1);
	//稍微封装了一下DynamicData
	Value getValueOfKey(const string& key);
private:
	SoilLayer* m_pSoilLayer;
	CropLayer* m_pCropLayer;
	FarmUILayer* m_pFarmUILayer;

	GoodLayer* m_pGoodLayer;
	//背包-当前页面
	int m_nCurPage;
	//背包层类型
	GoodLayerType m_goodLayerType;
	
	//滑动条对话框
	SliderDialog* m_pSliderDialog;
	//当前选中的土壤
	Soil* m_pSelectedSoil;
	//当前选中的物品
	Good* m_pSelectedGood;
	//商店物品列表
	vector<Good*> m_shopList;
};
#endif
