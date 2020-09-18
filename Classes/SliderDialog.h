#ifndef __SliderDialog_H__
#define __SliderDialog_H__

#include <string>
#include <functional>

#include "SDL_Engine/SDL_Engine.h"
USING_NS_SDL;
using namespace std;

/**
 * 滑动条对话框
 * 需要：名称为fonts/1.fnt sprite/slider_dialog_ui_res.xml
 * 注意：Widget优先级为-4 阻挡监听器优先级为-3
 */
class SliderDialog : public Node
{
public:
	/**
	 * 当点击确认或者取消时回调该函数
	 * @param bool 确认/取消
	 * @param int 当前选择的个数
	 */
	typedef function<void (bool, int)> sliderDialogCallback;
public:
	SliderDialog();
	virtual ~SliderDialog();
	CREATE_FUNC(SliderDialog);
	bool init();

	/**
	 * 更新显示的标题
	 * @param title 标题
	 */
	void updateShowingTitle(const string& title);
	/**
	 * 设置最大值
	 * @param percent 滑动条最大值
	 */
	void setMaxPercent(int percent);

	/**
	 * 设置当前进度
	 * @param percent 当前进度
	 */
	void setPercent(int percent);
	/**
	 * 设置显示的单价
	 * @param price 
	 */
	void setPrice(int price);
	/**
	 * 设置回调函数 也可以负责隐藏此对话框
	 */
	void setCallback(const sliderDialogCallback& callback);

	bool isShowing() const;
	void setShowing(bool showing);
private:
	void clickBtnCallback(Object* sender, bool ok);
	void sliderCallback(ui::Slider* slider, ui::Slider::EventType type);
	bool onTouchBegan(Touch* touch, SDL_Event* event);
	void onTouchMoved(Touch* touch, SDL_Event* event);
	void onTouchEnded(Touch* touch, SDL_Event* event);
private:
	sliderDialogCallback m_callback;
	Sprite* m_pBackSprite;
	//该控件使用到了fonts/1.fnt
	LabelBMFont* m_pTitleLabel;
	ui::Slider* m_pSlider;

	LabelAtlas* m_pNumberLabel;
	LabelAtlas* m_pWorthLabel;

	ui::Button* m_pOKBtn;
	ui::Button* m_pCancelBtn;

	int m_nPrice;
	//吞并事件监听器
	EventListenerTouchOneByOne* m_pListener;
	bool m_bShowing;
};
#endif
