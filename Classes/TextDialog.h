#ifndef __TextDialog_H__
#define __TextDialog_H__

#include <string>
#include "SDL_Engine/SDL_Engine.h"

USING_NS_SDL;
using namespace std;

/**
 * 文本对话框
 * 需要：名称为fonts/1.fnt sprite/slider_dialog_ui_res.xml
 * 注意：Widget 优先级为-4 阻挡监听器优先级为-3
 */
class TextDialog : public Node
{
public:
	/**
	 * 当点击确认或者取消时回调该函数
	 * @param bool 确认/取消
	 */
	typedef function<void (bool)> textDialogCallback;
public:
	TextDialog();
	~TextDialog();
	CREATE_FUNC(TextDialog);
	bool init();
	/**
	 * 更新显示的标题
	 * @param title 标题
	 */
	void updateShowingTitle(const string& title);
	/**
	 * 更新显示的内容
	 * @param content 内容
	 */
	void updateShowingContent(const string& content);
	/**
	 * 设置回调函数 也可以负责隐藏此对话框
	 */
	void setCallback(const textDialogCallback& callback);

	bool isShowing() const;
	void setShowing(bool showing);
private:
	void clickBtnCallback(Object* sender, bool ok);
	bool onTouchBegan(Touch* touch, SDL_Event* event);
	void onTouchMoved(Touch* touch, SDL_Event* event);
	void onTouchEnded(Touch* touch, SDL_Event* event);
private:
	textDialogCallback m_callback;
	
	Sprite* m_pBackSprite;
	LabelBMFont* m_pTitleLabel;
	LabelBMFont* m_pContentLabel;
	
	ui::Button* m_pOKBtn;
	ui::Button* m_pCancelBtn;

	EventListenerTouchOneByOne* m_pListener;
	bool m_bShowing;
};
#endif
