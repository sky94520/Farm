#ifndef __FarmUI_Layer_H__
#define __FarmUI_Layer_H__

#include <string>

#include "SDL_Engine/SDL_Engine.h"

USING_NS_SDL;
using namespace std;

class Crop;

class FarmUILayerDelegate
{
public:
	FarmUILayerDelegate(){}
	virtual ~FarmUILayerDelegate(){}

	virtual void harvestCrop(Crop* crop) = 0;
	virtual void shovelCrop(Crop* crop) = 0;
	virtual void fightCrop(Crop* crop) = 0;

	//打开仓库
	virtual void showWarehouse() = 0;
	//打开商店
	virtual void showShop() = 0;

	virtual void saveData() = 0;
};

class FarmUILayer : public Layer
{
private:
	ui::Button* m_pWarehouseBtn;
	ui::Button* m_pShopBtn;
	//农场背景板
	Node* m_pTagSprite;

	Menu* m_pMenu;
	//收获按钮
	MenuItemSprite* m_pHarvestItem;
	//铲子按钮
	MenuItemSprite* m_pShovelItem;
	//战斗按钮
	MenuItemSprite* m_pFightItem;
	//委托者
	FarmUILayerDelegate* m_pDelegate;
	//当前操作的作物
	Crop* m_pOperatingCrop;
	//面板对应作物
	Crop* m_pInfoCrop;
	//作物经过秒数 主要用于刷新显示面板
	int m_nCropSecond;
	//作物状态面板
	Node* m_pCropInfoPanel;
public:
	FarmUILayer();
	~FarmUILayer();

	CREATE_FUNC(FarmUILayer);
	bool init();
	void update(float dt);

	void setDelegate(FarmUILayerDelegate* pDelegate) { m_pDelegate = pDelegate; }
	/** 
	 * 展示操作按钮 会根据crop的状态生成不同的按钮
	 * @param crop 要操作的作物
	 */
	void showOperationBtns(Crop* crop);
	/** 
	 * 隐藏操作按钮
	 */
	void hideOperationBtns();
	/**
	 * 更新显示的金币
	 * @param goldNum 金币数目
	 */
	void updateShowingGold(int goldNum);
	/**
	 * 更新显示的等级
	 * @param lv 等级
	 */
	void updateShowingLv(int lv);
	/**
	 * 更新显示的经验
	 * @param exp 当前经验
	 * @param maxExp 当前等级的最大经验
	 */
	void updateShowingExp(int exp, int maxExp);
private:
	MenuItemSprite* initializeOperationBtn(const string& text);
	//操作按钮回调函数
	void clickOperationBtnCallback(Object* sender);
	//存档回调
	void clickSaveBtnCallback(Object* sender);
	//显示作物状态
	void showCropInfo(Crop* crop);
	//隐藏作物状态
	void hideCropInfo();
	//仓库按钮回调函数
	void warehouseBtnCallback(Object* sender);
	//商店按钮回调函数
	void shopBtnCallback(Object* sender);
	//更新作物状态的时间
	void updateCropInfo();
	//显示 or 隐藏操作按钮
	void setVisibleOfOperationBtns(bool visible);
};
#endif
