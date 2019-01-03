#include "TextDialog.h"

TextDialog::TextDialog()
	:m_callback(nullptr)
	,m_pBackSprite(nullptr)
	,m_pTitleLabel(nullptr)
	,m_pContentLabel(nullptr)
	,m_pOKBtn(nullptr)
	,m_pCancelBtn(nullptr)
	,m_pListener(nullptr)
	,m_bShowing(true)
{
}

TextDialog::~TextDialog()
{
}

bool TextDialog::init()
{
	auto manager = ui::UIWidgetManager::getInstance();
	auto node = manager->createWidgetsWithXml("scene/text_dialog.xml");
	this->addChild(node);
	//获取有用的控件
	m_pBackSprite = node->getChildByName<Sprite*>("bg");
	m_pTitleLabel = node->getChildByName<LabelBMFont*>("title_text");
	m_pContentLabel = node->getChildByName<LabelBMFont*>("content_text");

	//确认/取消按钮
	m_pOKBtn = node->getChildByName<ui::Button*>("ok_btn");
	m_pCancelBtn = node->getChildByName<ui::Button*>("cancel_btn");

	m_pOKBtn->addClickEventListener(SDL_CALLBACK_1(TextDialog::clickBtnCallback, this, true));
	m_pCancelBtn->addClickEventListener(SDL_CALLBACK_1(TextDialog::clickBtnCallback, this, false));
	//默认不显示
	this->setShowing(false);

	return true;
}

void TextDialog::updateShowingTitle(const string& title)
{
	m_pTitleLabel->setString(title);
}

void TextDialog::updateShowingContent(const string& content)
{
	m_pContentLabel->setString(content);
}

void TextDialog::setCallback(const textDialogCallback& callback)
{
	m_callback = callback;
}

bool TextDialog::isShowing() const
{
	return m_bShowing;
}

void TextDialog::setShowing(bool showing)
{
	if (showing == m_bShowing)
		return ;
	m_bShowing = showing;

	if (m_bShowing)
	{
		m_pListener = EventListenerTouchOneByOne::create();
		m_pListener->onTouchBegan = SDL_CALLBACK_2(TextDialog::onTouchBegan, this);
		m_pListener->onTouchMoved = SDL_CALLBACK_2(TextDialog::onTouchMoved, this);
		m_pListener->onTouchEnded = SDL_CALLBACK_2(TextDialog::onTouchEnded, this);

		m_pListener->setSwallowTouches(true);
		m_pListener->setPriority(-3);
		_eventDispatcher->addEventListener(m_pListener, this);
	}
	else if (m_pListener != nullptr)
	{
		_eventDispatcher->removeEventListener(m_pListener);
		m_pListener = nullptr;
	}
	m_pOKBtn->setTouchEnabled(m_bShowing);
	m_pCancelBtn->setTouchEnabled(m_bShowing);
}

void TextDialog::clickBtnCallback(Object* sender, bool ok)
{
	if (m_callback != nullptr)
		m_callback(ok);
}

bool TextDialog::onTouchBegan(Touch* touch, SDL_Event* event)
{
	return true;
}

void TextDialog::onTouchMoved(Touch* touch, SDL_Event* event)
{
}

void TextDialog::onTouchEnded(Touch* touch, SDL_Event* event)
{
}
