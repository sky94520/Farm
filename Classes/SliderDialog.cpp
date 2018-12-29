#include "SliderDialog.h"

SliderDialog::SliderDialog()
	:m_callback(nullptr)
	,m_pBackSprite(nullptr)
	,m_pTitleLabel(nullptr)
	,m_pSlider(nullptr)
	,m_pNumberLabel(nullptr)
	,m_pWorthLabel(nullptr)
	,m_pOKBtn(nullptr)
	,m_pCancelBtn(nullptr)
	,m_nPrice(0)
	,m_pListener(nullptr)
	,m_bShowing(true)
{
}

SliderDialog::~SliderDialog()
{
}

bool SliderDialog::init()
{
	auto manager = ui::UIWidgetManager::getInstance();
	auto node = manager->createWidgetsWithXml("scene/slider_dialog.xml");
	this->addChild(node);
	//获取有用的控件
	m_pBackSprite = node->getChildByName<Sprite*>("bg");
	m_pTitleLabel = node->getChildByName<LabelBMFont*>("title_text");
	m_pSlider = node->getChildByName<ui::Slider*>("number_slider");
	m_pSlider->addEventListener(SDL_CALLBACK_2(SliderDialog::sliderCallback, this));

	m_pNumberLabel = node->getChildByName<LabelAtlas*>("num_label");
	m_pWorthLabel = node->getChildByName<LabelAtlas*>("worth_label");

	//确认/取消按钮
	m_pOKBtn = node->getChildByName<ui::Button*>("ok_btn");
	m_pCancelBtn = node->getChildByName<ui::Button*>("cancel_btn");

	m_pOKBtn->addClickEventListener(SDL_CALLBACK_1(SliderDialog::clickBtnCallback, this, true));
	m_pCancelBtn->addClickEventListener(SDL_CALLBACK_1(SliderDialog::clickBtnCallback, this, false));
	//默认不显示
	this->setShowing(false);

	return true;
}

void SliderDialog::updateShowingTitle(const string& title)
{
	m_pTitleLabel->setString(title);
}

void SliderDialog::setMaxPercent(int maxPercent)
{
	m_pSlider->setMaxPercent(maxPercent);
	//根据当前进度来设置总价值
	int percent = (int)m_pSlider->getPercent();
	
	int value = percent * m_nPrice;

	m_pNumberLabel->setString(StringUtils::toString(percent));
	m_pWorthLabel->setString(StringUtils::toString(value));
}

void SliderDialog::setPercent(int percent)
{
	int curPercent = (int)m_pSlider->getPercent();
	if (curPercent == percent)
		return ;

	m_pSlider->setPercent(percent);
	//重新获取当前值
	curPercent = (int)m_pSlider->getPercent();
	int value = curPercent * m_nPrice;

	m_pNumberLabel->setString(StringUtils::toString(percent));
	m_pWorthLabel->setString(StringUtils::toString(value));
}

void SliderDialog::setPrice(int price)
{
	m_nPrice = price;
}

void SliderDialog::setCallback(const sliderDialogCallback& callback)
{
	m_callback = callback;
}

bool SliderDialog::isShowing() const
{
	return m_bShowing;
}

void SliderDialog::setShowing(bool showing)
{
	if (showing == m_bShowing)
		return ;
	m_bShowing = showing;

	if (m_bShowing)
	{
		m_pListener = EventListenerTouchOneByOne::create();
		m_pListener->onTouchBegan = SDL_CALLBACK_2(SliderDialog::onTouchBegan, this);
		m_pListener->onTouchMoved = SDL_CALLBACK_2(SliderDialog::onTouchMoved, this);
		m_pListener->onTouchEnded = SDL_CALLBACK_2(SliderDialog::onTouchEnded, this);

		m_pListener->setSwallowTouches(true);
		m_pListener->setPriority(-3);
		_eventDispatcher->addEventListener(m_pListener, this);
	}
	else if (m_pListener != nullptr)
	{
		_eventDispatcher->removeEventListener(m_pListener);
		m_pListener = nullptr;
	}
	m_pSlider->setTouchEnabled(m_bShowing);
	m_pOKBtn->setTouchEnabled(m_bShowing);
	m_pCancelBtn->setTouchEnabled(m_bShowing);
}

void SliderDialog::clickBtnCallback(Object* sender, bool ok)
{
	if (m_callback != nullptr)
	{
		int percent = (int)m_pSlider->getPercent();
		m_callback(ok, percent);
	}
}

void SliderDialog::sliderCallback(ui::Slider* slider, ui::Slider::EventType type)
{
	int percent = (int)slider->getPercent();
	int value = percent * m_nPrice;

	m_pNumberLabel->setString(StringUtils::toString(percent));
	m_pWorthLabel->setString(StringUtils::toString(value));
}

bool SliderDialog::onTouchBegan(Touch* touch, SDL_Event* event)
{
	return true;
}

void SliderDialog::onTouchMoved(Touch* touch, SDL_Event* event)
{
}

void SliderDialog::onTouchEnded(Touch* touch, SDL_Event* event)
{
}
