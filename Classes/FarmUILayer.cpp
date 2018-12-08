#include "FarmUILayer.h"
#include "Crop.h"
#include "StaticData.h"

FarmUILayer::FarmUILayer()
	:m_pWarehouseBtn(nullptr)
	,m_pShopBtn(nullptr)

	,m_pTagSprite(nullptr)

	,m_pMenu(nullptr)
	,m_pHarvestItem(nullptr)
	,m_pShovelItem(nullptr)
	,m_pFightItem(nullptr)

	,m_pDelegate(nullptr)
	,m_pOperatingCrop(nullptr)
	,m_pInfoCrop(nullptr)
	,m_nCropSecond(0)
	,m_pCropInfoPanel(nullptr)
{
}

FarmUILayer::~FarmUILayer()
{
	SDL_SAFE_RELEASE_NULL(m_pInfoCrop);
}

bool FarmUILayer::init()
{
	auto manager = ui::UIWidgetManager::getInstance();
	//加载仓库、商店、背包等控件
	auto node = manager->createWidgetsWithXml("scene/farm_layer.xml");
	m_pWarehouseBtn = node->getChildByName<ui::Button*>("warehouse_btn");
	m_pWarehouseBtn->addClickEventListener(
			SDL_CALLBACK_1(FarmUILayer::warehouseBtnCallback, this));

	m_pShopBtn = node->getChildByName<ui::Button*>("shop_btn");
	m_pShopBtn->addClickEventListener(SDL_CALLBACK_1(FarmUILayer::shopBtnCallback, this));
	//背景板
	m_pTagSprite = node->getChildByName("tag_bg");

	this->addChild(node);
	//保存按钮添加函数回调
	auto saveBtn = node->getChildByName<ui::Button*>("save_btn");
	saveBtn->addClickEventListener(SDL_CALLBACK_1(FarmUILayer::clickSaveBtnCallback, this));

	//创建按钮
	m_pHarvestItem = this->initializeOperationBtn(STATIC_DATA_STRING("harvest_text"));
	m_pShovelItem = this->initializeOperationBtn(STATIC_DATA_STRING("shovel_text"));
	m_pFightItem = this->initializeOperationBtn(STATIC_DATA_STRING("fight_text"));

	m_pMenu = Menu::create(m_pHarvestItem, m_pShovelItem, m_pFightItem, nullptr);
	this->addChild(m_pMenu);
	//设置不可用
	m_pMenu->setEnabled(false);

	m_pHarvestItem->setVisible(false);
	m_pShovelItem->setVisible(false);
	m_pFightItem->setVisible(false);

	m_pHarvestItem->setSwallowed(true);
	m_pShovelItem->setSwallowed(true);
	m_pFightItem->setSwallowed(true);

	//作物信息节点
	m_pCropInfoPanel = manager->createWidgetsWithXml("scene/farm_crop_info.xml");
	this->addChild(m_pCropInfoPanel);
	m_pCropInfoPanel->setVisible(false);

	return true;
}

void FarmUILayer::update(float dt)
{
	//面板作物不存在或已经枯萎或时间未到,不需刷新
	if (m_pInfoCrop == nullptr || m_pInfoCrop->isWitherred() 
	    || m_pInfoCrop->getSecond() == m_nCropSecond)
	{
		return;
	}
	this->updateCropInfo();
}

void FarmUILayer::showOperationBtns(Crop* crop)
{
	//已经显示
	if (m_pOperatingCrop == crop)
		return;

	SDL_SAFE_RETAIN(crop);
	SDL_SAFE_RELEASE(m_pOperatingCrop);
	m_pOperatingCrop = crop;

	this->setVisibleOfOperationBtns(true);
	//显示作物信息
	this->showCropInfo(crop);
}

void FarmUILayer::hideOperationBtns()
{
	if (m_pOperatingCrop == nullptr)
		return;

	this->setVisibleOfOperationBtns(false);

	//隐藏作物信息
	this->hideCropInfo();
	SDL_SAFE_RELEASE_NULL(m_pOperatingCrop);
}

void FarmUILayer::updateShowingGold(int goldNum)
{
	auto goldLabel = m_pTagSprite->getChildByName<LabelAtlas*>("gold");
	goldLabel->setString(StringUtils::toString(goldNum));
}

void FarmUILayer::updateShowingLv(int lv)
{
	auto lvLabel = m_pTagSprite->getChildByName<LabelAtlas*>("level");
	lvLabel->setString(StringUtils::toString(lv));
}

void FarmUILayer::updateShowingExp(int exp, int maxExp)
{
	//经验控件
	auto expLabel = m_pTagSprite->getChildByName<LabelAtlas*>("exp");
	auto progress = m_pTagSprite->getChildByName<ProgressTimer*>("exp_progress");

	string text = StringUtils::format("%d/%d", exp, maxExp);
	expLabel->setString(text);

	int percentage = int((float)exp / maxExp * 100);
	progress->setPercentage(percentage);
}

MenuItemSprite* FarmUILayer::initializeOperationBtn(const string& text)
{
        //创建新按钮
	LabelBMFont* label = LabelBMFont::create(text, "fonts/1.fnt");
	auto size = label->getContentSize();
	size.width *= 1.5f;
	size.height *= 1.5f;

	label->setPosition(size.width / 2, size.height / 2); 
	label->setAnchorPoint(Point(0.5f,0.5f));

	Scale9Sprite* normalSprite = Scale9Sprite::create(Sprite::createWithSpriteFrameName("bt6_1.png"), Rect(5, 5, 10, 10));
	normalSprite->setPreferredSize(size);

	Scale9Sprite* selectedSprite = Scale9Sprite::create(Sprite::createWithSpriteFrameName("bt6_2.png"), Rect(5, 5, 10, 10));
	selectedSprite->setPreferredSize(size);

	MenuItemSprite* item = MenuItemSprite::create(normalSprite, selectedSprite);

	item->addChild(label, 6); 
	item->setName(text);
	item->setCallback(SDL_CALLBACK_1(FarmUILayer::clickOperationBtnCallback, this));

	return item;
}

void FarmUILayer::clickOperationBtnCallback(Object* sender)
{
	if (m_pHarvestItem == sender)
		m_pDelegate->harvestCrop(m_pOperatingCrop);
	else if (m_pShovelItem == sender)
		m_pDelegate->shovelCrop(m_pOperatingCrop);
	else if (m_pFightItem == sender)
		m_pDelegate->fightCrop(m_pOperatingCrop);
}

void FarmUILayer::clickSaveBtnCallback(Object* sender)
{
	m_pDelegate->saveData();
}

void FarmUILayer::showCropInfo(Crop* crop)
{
	//设置面板显示的作物
	SDL_SAFE_RETAIN(crop);
	m_pInfoCrop = crop;
	m_nCropSecond = m_pInfoCrop->getSecond();

	int id = crop->getCropID();
	//获取作物名称
	auto pCropSt = StaticData::getInstance()->getCropStructByID(id);
	string name = pCropSt->name;
	//获取作物季数
	int harvestCount = crop->getHarvestCount();
	int totalHarvest = pCropSt->harvestCount;

	auto nameLabel = m_pCropInfoPanel->getChildByName<LabelBMFont*>("name_label");
	auto fruitSprite = m_pCropInfoPanel->getChildByName<Sprite*>("fruit_sprite");
	auto timePorgress = m_pCropInfoPanel->getChildByName<ProgressTimer*>("time_progress");
	auto ripeLabel = m_pCropInfoPanel->getChildByName<LabelBMFont*>("ripe_label");

	string nameText;
	bool bVisible = false;
	Size size = m_pCropInfoPanel->getContentSize();
	//判断作物是否枯萎,是则只显示名称控件
	if (crop->isWitherred())
	{
		auto name_format = STATIC_DATA_STRING("crop_name_witherred_format").c_str();
		bVisible = false;

		nameText = StringUtils::format(name_format, name.c_str());
	}
	else
	{
		auto name_format = STATIC_DATA_STRING("crop_name_format");
		auto time_format = STATIC_DATA_STRING("crop_time_format");
		bVisible = true;

		nameText = StringUtils::format(name_format.c_str(), name.c_str(), harvestCount, totalHarvest);
	}
	//显示状态面板
	m_pCropInfoPanel->setVisible(true);
	auto pos = crop->getPosition();
	pos.y -= crop->getContentSize().height * crop->getAnchorPoint().y + size.height / 2;
	m_pCropInfoPanel->setPosition(pos);

	//设置显示名称
	nameLabel->setString(nameText);
	//确定果实精灵的位置
	auto fruit_format = STATIC_DATA_STRING("fruit_filename_format");
	auto fruitName = StringUtils::format(fruit_format.c_str(), id);
	fruitSprite->setSpriteFrame(fruitName);

	pos = nameLabel->getPosition();
	auto nameSize = nameLabel->getContentSize();
	auto fruitSize = fruitSprite->getContentSize();

	pos.x = pos.x - nameSize.width * 0.5f - fruitSize.width * 0.5f;
	pos.y = timePorgress->getPositionY() - timePorgress->getContentSize().height / 2 - fruitSize.height * 0.5f;
	fruitSprite->setPosition(pos);

	timePorgress->setVisible(bVisible);
	ripeLabel->setVisible(bVisible);
	
	if (bVisible)
	{
		this->updateCropInfo();
	}
}

void FarmUILayer::hideCropInfo()
{
	SDL_SAFE_RELEASE_NULL(m_pInfoCrop);
	m_pCropInfoPanel->setVisible(false);
}

void FarmUILayer::warehouseBtnCallback(Object* sender)
{
	m_pDelegate->showWarehouse();
}

void FarmUILayer::shopBtnCallback(Object* sender)
{
	m_pDelegate->showShop();
}

void FarmUILayer::updateCropInfo()
{
	//获取作物当前时间和总时间
	time_t startTime = m_pInfoCrop->getStartTime();
	int totalHour = m_pInfoCrop->getGrowingHour(-1);
	time_t endTime = startTime + totalHour * 3600;
	time_t curTime = time(nullptr);

	auto timePorgress = m_pCropInfoPanel->getChildByName<ProgressTimer*>("time_progress");
	auto ripeLabel = m_pCropInfoPanel->getChildByName<LabelBMFont*>("ripe_label");

	string timeText;
	auto time_format = STATIC_DATA_STRING("crop_time_format");

	//判别时间
	if (curTime >= endTime)
	{
		timeText = STATIC_DATA_STRING("ripe_text");
	}
	else
	{
		auto deltaTime = endTime - curTime;
		int hour = deltaTime / 3600;
		int minute = (deltaTime - hour * 3600) / 60;
		int second = deltaTime - hour * 3600 - minute * 60;
		//刷新时间，使得可以一秒刷新一次
		m_nCropSecond = second;

		timeText = StringUtils::format(time_format.c_str(), hour, minute, second);
	}
	//prohress内部容错，当前并未控制取值范围
	float percentage = (curTime - startTime) / (totalHour * 36.f);
	timePorgress->setPercentage(percentage);
	ripeLabel->setString(timeText);
}

void FarmUILayer::setVisibleOfOperationBtns(bool visible)
{
	if (m_pOperatingCrop == nullptr)
	{
		LOG("error:m_pOperatingCrop == nullptr\n");
		return;
	}

	auto pos = m_pOperatingCrop->getPosition();
	auto size = m_pHarvestItem->getContentSize();
	//位置偏移
	vector<float> deltas;
	deltas.push_back( size.width);
	deltas.push_back(-size.width);

	float scale = visible ? 1.5f : 1.f;
	//菜单按钮
	vector<MenuItemSprite*> items;

	if (m_pOperatingCrop->isRipe())
		items.push_back(m_pHarvestItem);
	items.push_back(m_pShovelItem);

	m_pMenu->setEnabled(visible);

	for (size_t i = 0;i < items.size(); i++)
	{
		auto item = items[i];

		//结束位置
		auto toPos = Point(pos.x + scale * deltas[i], pos.y);
		ActionInterval* action = nullptr;
		auto move = MoveTo::create(0.1f, toPos);

		if (visible)
		{
			item->setPosition(pos.x + deltas[i], pos.y);
			action = move;
			item->setVisible(true);
		}
		else
		{
			auto hide = Hide::create();
			action = Sequence::createWithTwoActions(move, hide);
		}
		action->setTag(1);

		item->setEnabled(visible);

		item->stopActionByTag(1);
		item->runAction(action);
	}
}
