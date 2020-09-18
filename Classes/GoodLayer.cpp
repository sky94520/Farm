#include "GoodLayer.h"
#include "GoodInterface.h"

GoodLayer::GoodLayer()
	:m_bShowing(false)
	,m_pBagBG(nullptr)
	,m_pTitleSprite(nullptr)
	,m_pCloseBtn(nullptr)
	,m_pDescLabel(nullptr)

	,m_pUserBtn(nullptr)
	,m_pEquipBtn(nullptr)
	,m_pPrePageBtn(nullptr)
	,m_pNextPageBtn(nullptr)

	,m_pPageLabel(nullptr)
	,m_pGoldLabel(nullptr)
	,m_pGoodGroup(nullptr)
	,m_pDelegate(nullptr)
	,m_pListener(nullptr)
{
}

GoodLayer::~GoodLayer()
{
}

bool GoodLayer::init()
{
	auto manager = ui::UIWidgetManager::getInstance();
	auto node = manager->createWidgetsWithXml("scene/bag_good_layer.xml");
	this->addChild(node);
	//获取节点相应控件
	m_pBagBG = node->getChildByName<Sprite*>("bag_bg");
	m_pTitleSprite = node->getChildByName<Sprite*>("title_text");

	m_pCloseBtn = node->getChildByName<Button*>("close_btn");
	m_pCloseBtn->addClickEventListener(SDL_CALLBACK_1(GoodLayer::closeBtnCallback, this));

	m_pDescLabel = node->getChildByName<LabelBMFont*>("good_desc_label");

	m_pUserBtn = node->getChildByName<Button*>("use_btn");
	m_pUserBtn->addClickEventListener(SDL_CALLBACK_1(GoodLayer::useBtnCallback, this));

	m_pEquipBtn = node->getChildByName<Button*>("equip_btn");
	m_pEquipBtn->addClickEventListener(SDL_CALLBACK_1(GoodLayer::equipBtnCallback, this));

	m_pPrePageBtn = node->getChildByName<Button*>("pre_page_btn");
	m_pPrePageBtn->addClickEventListener(SDL_CALLBACK_1(GoodLayer::turnPageBtnCallback, this, -1));

	m_pNextPageBtn = node->getChildByName<Button*>("next_page_btn");
	m_pNextPageBtn->addClickEventListener(SDL_CALLBACK_1(GoodLayer::turnPageBtnCallback, this, 1));

	m_pPageLabel = node->getChildByName<LabelAtlas*>("page_label");
	m_pGoldLabel = node->getChildByName<LabelAtlas*>("gold_label");
	//创建物品组
	m_pGoodGroup = RadioButtonGroup::create();

	auto& children = node->getChildByName("bag_good_list")->getChildren();
	for (size_t i = 0; i < children.size(); i++)
	{
		auto radioBtn = static_cast<RadioButton*>(children.at(i));
		m_pGoodGroup->addRadioButton(radioBtn);
	}
	m_pGoodGroup->addEventListener(SDL_CALLBACK_3(GoodLayer::selectGoodCallback, this));
	this->addChild(m_pGoodGroup);

	m_bShowing = true;
	this->setShowing(false);

	return true;
}

bool GoodLayer::isShowing() const
{
	return m_bShowing;
}

void GoodLayer::setShowing(bool showing)
{
	if (showing == m_bShowing)
		return ;
	m_bShowing = showing;

	if (m_bShowing)
	{
		m_pListener = EventListenerTouchOneByOne::create();
		m_pListener->onTouchBegan = SDL_CALLBACK_2(GoodLayer::onTouchBegan, this);
		m_pListener->onTouchMoved = SDL_CALLBACK_2(GoodLayer::onTouchMoved, this);
		m_pListener->onTouchEnded = SDL_CALLBACK_2(GoodLayer::onTouchEnded, this);

		m_pListener->setSwallowTouches(true);
		m_pListener->setPriority(-1);
		_eventDispatcher->addEventListener(m_pListener, this);
	}
	else if (m_pListener != nullptr)
	{
		_eventDispatcher->removeEventListener(m_pListener);
		m_pListener = nullptr;
	}
	m_pUserBtn->setTouchEnabled(m_bShowing);
	m_pPrePageBtn->setTouchEnabled(m_bShowing);
	m_pNextPageBtn->setTouchEnabled(m_bShowing);
}

void GoodLayer::updateShowingGoods(vector<GoodInterface*>& vec)
{
	auto func = SDL_CALLBACK_2(GoodLayer::updateRadioButton, this);
	GoodLayer::updateRadioButtons<GoodInterface*>(m_pGoodGroup, vec, func);
	//如果为空，则按钮不可用
	if (vec.size() == 0)
	{
		m_pDescLabel->setString("");

		m_pUserBtn->setTouchEnabled(false);
		m_pPrePageBtn->setTouchEnabled(false);
		m_pNextPageBtn->setTouchEnabled(false);
	}
}

void GoodLayer::updateShowingTitle(const string& filename)
{
	m_pTitleSprite->setSpriteFrame(filename);
}

void GoodLayer::updateShowingBtn(BtnType type, const BtnParamSt& params)
{
	//获取当前要更新的按钮
	Button* btn = nullptr;

	switch (type)
	{
		case BtnType::Use: btn = m_pUserBtn; break;
		case BtnType::Equip: btn = m_pEquipBtn; break;
	}
	if (btn == nullptr)
	{
		LOG("erro:not found current button by type\n");
		return ;
	}
	btn->setVisible(params.visible);
	btn->setTouchEnabled(params.enable);

	//避免无意义的按钮改变
	if (!params.frameFilename.empty())
	{
		auto innerSprite = btn->getChildByName<Sprite*>("sprite");
		innerSprite->setSpriteFrame(params.frameFilename);
	}
}

void GoodLayer::updateShowingPage(int curPage, int totalPage)
{
	auto text = StringUtils::format("%d/%d", curPage, totalPage);
	m_pPageLabel->setString(text);
}

void GoodLayer::updateShowingGold(int goldNum)
{
	m_pGoldLabel->setString(StringUtils::toString(goldNum));
}

void GoodLayer::setDelegate(GoodLayerDelegate* pDelegate)
{
	m_pDelegate = pDelegate;
}

bool GoodLayer::onTouchBegan(Touch* touch, SDL_Event* event)
{
	//背包层隐藏，则不捕获事件
	if (!m_bShowing)
		return false;

	auto pos1 = touch->getLocation();
	
	auto pos2 = m_pBagBG->getPosition();
	pos2 = m_pBagBG->convertToWorldSpaceAR(pos2);
	auto size = m_pBagBG->getContentSize();
	
	//容错机制
	size.width += 20.f;
	size.height += 20.f;

	Rect rect = Rect(pos2, size);
	
	//点击了
	if (rect.containsPoint(pos1))
		return true;
	else
	{
		m_pDelegate->touchOutsideCallback(this);
		return true;
	}
}

void GoodLayer::onTouchMoved(Touch* touch, SDL_Event* event)
{
}

void GoodLayer::onTouchEnded(Touch* touch, SDL_Event* event)
{
}

void GoodLayer::updateRadioButton(RadioButton* radioBtn, GoodInterface* good)
{
	bool ret = (good != nullptr);

	radioBtn->setUserData(good);
	radioBtn->setVisible(ret);
	radioBtn->setTouchEnabled(ret);

	if (good == nullptr)
		return;

	auto iconFrame = good->getIcon();
	auto name = good->getName();
	auto number = good->getNumber();
	auto cost = good->getCost();
	//更新radio button的显示
	auto pIconSprite = radioBtn->getChildByName<Sprite*>("icon");
	auto pNameLabel = radioBtn->getChildByName<LabelBMFont*>("name");
	auto pCostLabel = radioBtn->getChildByName<LabelAtlas*>("cost");
	auto pNumberLabel = radioBtn->getChildByName<LabelAtlas*>("number");
	//更新
	pNameLabel->setString(name);
	pNumberLabel->setString(StringUtils::toString(number));
	pCostLabel->setString(StringUtils::toString(cost));
	//设置图标
	pIconSprite->setSpriteFrame(iconFrame);
	//图标大小固定
	auto size = pIconSprite->getContentSize();
	pIconSprite->setScale(24 / size.width, 24 / size.height);
}

//------------------------------一系列回调函数-------------------------------
void GoodLayer::turnPageBtnCallback(Object* sender, int value)
{
	//调用委托者
	if (m_pDelegate != nullptr)
	{
		m_pDelegate->pageBtnCallback(this, value);
	}
	else
	{
		LOG("error:m_pDelegate == nullptr\n");
	}
}

void GoodLayer::useBtnCallback(Object* sender)
{
	//调用委托者
	if (m_pDelegate != nullptr)
	{
		m_pDelegate->useBtnCallback(this);
	}
	else
	{
		LOG("error:m_pDelegate == nullptr\n");
	}
}

void GoodLayer::closeBtnCallback(Object* sender)
{
	//调用委托者
	if (m_pDelegate != nullptr)
	{
		m_pDelegate->closeBtnCallback(this);
	}
	else
	{
		LOG("error:m_pDelegate == nullptr\n");
	}
}

void GoodLayer::equipBtnCallback(Object* sender)
{
	//调用委托者
	if (m_pDelegate != nullptr)
	{
		m_pDelegate->equipBtnCallback(this);
	}
	else
	{
		LOG("error:m_pDelegate == nullptr\n");
	}

}

void GoodLayer::selectGoodCallback(RadioButton* radioBtn, int index, RadioButtonGroup::EventType)
{
	//获取该按钮对应的good
	auto good = static_cast<GoodInterface*>(radioBtn->getUserData());
	//获取物品描述
	auto desc = good->getDescription();
	//设置文本
	m_pDescLabel->setString(desc);
	//调用委托者
	if (m_pDelegate != nullptr)
	{
		m_pDelegate->selectGoodCallback(this, good);
	}
	else
	{
		LOG("error:m_pDelegate == nullptr\n");
	}
}


