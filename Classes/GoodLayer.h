#ifndef __GoodLayer_H__
#define __GoodLayer_H__

#include <vector>
#include <string>
#include <functional>

#include "SDL_Engine/SDL_Engine.h"

USING_NS_SDL;
using namespace std;
using namespace SDL::ui;

class GoodInterface;
class GoodLayer;
/**
 * 事件回调委托类
 */
class GoodLayerDelegate
{
public:
	GoodLayerDelegate(){}
	virtual ~GoodLayerDelegate(){}
	/**
	 * 点击上一页按钮回调函数
	 * @param goodLayer 对应的物品层
	 * @param value 下一页+1 上一页-1
	 */
	virtual void pageBtnCallback(GoodLayer* goodLayer, int value) = 0;
	/**
	 * 使用按钮回调函数
	 * @param goodLayer 对应的物品层
	 */
	virtual void useBtnCallback(GoodLayer* goodLayer) = 0;
	/**
	 * 装备按钮回调函数
	 * @param goodLayer 对应的物品层
	 */
	virtual void equipBtnCallback(GoodLayer* goodLayer) = 0;
	/**
	 * 关闭按钮回调函数
	 * @param goodLayer 对应的物品层
	 */
	virtual void closeBtnCallback(GoodLayer* goodLayer) = 0;
	/**
	 * 点击了GoodLayer外部回调函数 默认回调closeBtnCallback()
	 */
	virtual void touchOutsideCallback(GoodLayer* goodLayer)
	{
		closeBtnCallback(goodLayer);
	}
	/**
	 * 选中物品回调函数
	 * @param goodLayer 对应的物品层
	 * @param good 对应的物品
	 */
	virtual void selectGoodCallback(GoodLayer* goodLayer, GoodInterface* good) = 0;
};

/**
 * 按钮类型
 */
enum class BtnType
{
	Use,//使用按钮
	Equip,//装备按钮
};
/**
 * 按钮设置参数
 * 主要负责参数填充
 */
struct BtnParamSt
{
	bool visible;
	bool enable;
	string frameFilename;
public:
	BtnParamSt(bool v = true, bool e = true, const string& filename = "")
		:visible(v)
		,enable(e)
		,frameFilename(filename){}
};
/**
 * author sky
 * date 2018-11-12
 * desc 物品层，需要配套的资源 good_layer_ui_res.* 和fonts/1.fnt
 * 内置一个优先级为-1的事件监听器 负责捕获事件
 * 同时Widget控件的优先级为-2
 */
class GoodLayer : public Layer
{
	//当前物品层是否打开（逻辑上标识）
	//SDL_BOOL_SYNTHESIZE(m_bShowing, Showing);
private:
	bool m_bShowing;
	//保存XML文件中常用的控件
	//背景
	Sprite* m_pBagBG;
	//标题
	Sprite* m_pTitleSprite;
	//关闭按钮
	Button* m_pCloseBtn;
	//物品描述文本
	LabelBMFont* m_pDescLabel;
	//使用按钮
	Button* m_pUserBtn;
	//装备按钮
	Button* m_pEquipBtn;
	//上一页按钮
	Button* m_pPrePageBtn;
	//下一页按钮
	Button* m_pNextPageBtn;
	//页面标签
	LabelAtlas* m_pPageLabel;
	//金币标签
	LabelAtlas* m_pGoldLabel;
	//物品组
	RadioButtonGroup* m_pGoodGroup;
	//委托
	GoodLayerDelegate* m_pDelegate;
	EventListenerTouchOneByOne* m_pListener;
public:
	GoodLayer();
	~GoodLayer();
	CREATE_FUNC(GoodLayer);
	bool init();

	bool isShowing() const;
	void setShowing(bool showing);
	/**
	 * 根据物品数组更新对应单选按钮，如果单选按钮个数少于物品数组，将会报错
	 * @param vec 物品数组
	 */
	void updateShowingGoods(vector<GoodInterface*>& vec);
	/**
	 * 更新显示的标题
	 * @param filename 标题对应的帧文件名
	 */
	void updateShowingTitle(const string& filename);
	/**
	 * 更新显示按钮
	 * @param type 当前要设置的按钮 目前仅仅有Use Equip
	 * @param params 参数结构体
	 */
	void updateShowingBtn(BtnType type, const BtnParamSt& params);
	/**
	 * 更新显示页面索引
	 * @param curPage 当前页面索引
	 * @param totalPage 总索引
	 */
	void updateShowingPage(int curPage, int totalPage);
	/**
	 * 更新显示金币
	 * @param goldNum 要显示的金币个数
	 */
	void updateShowingGold(int goldNum);
	/**
	 * 设置委托
	 */
	void setDelegate(GoodLayerDelegate* pDelegate);
private:
	void updateRadioButton(RadioButton* radioBtn, GoodInterface* good);
	bool onTouchBegan(Touch* touch, SDL_Event* event);
	void onTouchMoved(Touch* touch, SDL_Event* event);
	void onTouchEnded(Touch* touch, SDL_Event* event);
private:
	//------------------------------一系列回调函数-------------------------------
	//上/下一个回调函数
	void turnPageBtnCallback(Object* sender, int value);
	//使用按钮回调函数
	void useBtnCallback(Object* sender);
	//关闭按钮回调函数
	void closeBtnCallback(Object* sender);
	//装备按钮回调函数
	void equipBtnCallback(Object* sender);
	//选中物品回调函数
	void selectGoodCallback(RadioButton* radioBtn, int index, RadioButtonGroup::EventType);
public:
	//更新物品显示
	template<typename T>
	static void updateRadioButtons(RadioButtonGroup* group, vector<T>& vec
			,const function<void (RadioButton*, T)>& updateRadioBtn)
	{
		auto number = group->getRadioButtonList().size();
		//是否应该更新 即重新选中按钮
		auto selectedIndex = group->getSelectedIndex();

		RadioButton* selectedBtn = nullptr;
		T selectedItem = nullptr;

		bool bShouldUpdate = false;

		if (selectedIndex == -1)
			selectedIndex = 0;
		else
			selectedBtn = group->getRadioButtonByIndex(selectedIndex);

		if (selectedBtn != nullptr)
			selectedItem = static_cast<T>(selectedBtn->getUserData());
		
		//没有选中项或者物品个数<=索引或者不匹配 则应该更新
		if (selectedItem == nullptr 
			|| (int)vec.size() <= selectedIndex
			|| selectedItem != vec[selectedIndex])
		{
			bShouldUpdate = true;
		}

		for (int i = number - 1;i >= 0 ;i--)
		{
			auto radioBtn = group->getRadioButtonByIndex(i);

			T item = nullptr;
			
			if (i < (int)vec.size())
			{
				item = vec.at(i);
			}
			//更新对应的单选按钮
			updateRadioBtn(radioBtn, item);
			//按钮是选中项 或者应该更新还没更新
			if (selectedIndex >= i && bShouldUpdate)
			{
				//当前是选中项，先取消选中
				if (selectedBtn == radioBtn)
					group->unselectedButton();
				//重新设置选中
				if (item != nullptr)
				{
					bShouldUpdate = false;
					group->setSelectedButton(radioBtn);
				}
			}
		}
	}
};
#endif
