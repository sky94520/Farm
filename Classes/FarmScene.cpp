#include "FarmScene.h"
#include "SoilLayer.h"
#include "CropLayer.h"
#include "Soil.h"
#include "Crop.h"
#include "DynamicData.h"
#include "StaticData.h"

FarmScene::FarmScene()
	:m_pSoilLayer(nullptr)
	,m_pCropLayer(nullptr)
	,m_pFarmUILayer(nullptr)
{
}

FarmScene::~FarmScene()
{
}

bool FarmScene::init()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	this->preloadResources();

	//创建土壤层
	m_pSoilLayer = SoilLayer::create();
	this->addChild(m_pSoilLayer);
	//创建作物层
	m_pCropLayer = CropLayer::create();
	this->addChild(m_pCropLayer);
	//ui层
	m_pFarmUILayer = FarmUILayer::create();
	m_pFarmUILayer->setDelegate(this);
	this->addChild(m_pFarmUILayer);

	//初始化土壤和作物
	this->initializeSoilsAndCrops();
	//更新数据显示
	int gold = this->getValueOfKey(GOLD_KEY).asInt();
	int lv = this->getValueOfKey(FARM_LEVEL_KEY).asInt();
	int exp = this->getValueOfKey(FARM_EXP_KEY).asInt();
	int maxExp = DynamicData::getInstance()->getFarmExpByLv(lv);

	m_pFarmUILayer->updateShowingGold(gold);
	m_pFarmUILayer->updateShowingLv(lv);
	m_pFarmUILayer->updateShowingExp(exp, maxExp);
	//添加事件监听器
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = SDL_CALLBACK_2(FarmScene::handleTouchEvent, this);

	_eventDispatcher->addEventListener(listener, this);

	return true;
}

bool FarmScene::handleTouchEvent(Touch* touch, SDL_Event* event)
{
	auto location = touch->getLocation();
	//是否点击了土地
	auto soil = m_pSoilLayer->getClickingSoil(location);

	//点到了“空地”
	if (soil == nullptr)
	{
		m_pFarmUILayer->hideOperationBtns();
		return true;
	}
	//获取土壤对应的作物
	auto crop = soil->getCrop();

	//未种植作物
	if (crop == nullptr)
	{
	}
	else//存在作物，显示操作按钮
	{
		m_pFarmUILayer->showOperationBtns(crop);
	}
	return false;
}

void FarmScene::harvestCrop(Crop* crop)
{
	auto dynamicData = DynamicData::getInstance();
	//隐藏操作按钮
	m_pFarmUILayer->hideOperationBtns();

	//果实个数
	int number = crop->harvest();
	int id = crop->getCropID();
	//获取果实经验
	auto pCropSt = StaticData::getInstance()->getCropStructByID(id);
	//获取经验值和等级
	Value curExp = this->getValueOfKey(FARM_EXP_KEY);
	Value lv = this->getValueOfKey(FARM_LEVEL_KEY);
	int allExp = dynamicData->getFarmExpByLv(lv.asInt());

	curExp = pCropSt->exp + curExp.asInt();
	//是否升级
	if (curExp.asInt() >= allExp)
	{
		curExp = curExp.asInt() - allExp;
		lv = lv.asInt() + 1;
		allExp = dynamicData->getFarmExpByLv(lv.asInt());
		//更新控件
		m_pFarmUILayer->updateShowingLv(lv.asInt());
		//等级写入
		dynamicData->setValueOfKey(FARM_LEVEL_KEY, lv);
	}
	m_pFarmUILayer->updateShowingExp(curExp.asInt(), allExp);
	//果实写入
	dynamicData->addGood(GoodType::Fruit, StringUtils::toString(id), number);
	//经验写入
	dynamicData->setValueOfKey(FARM_EXP_KEY, curExp);
	//作物季数写入
	dynamicData->updateCrop(crop);
}

void FarmScene::shovelCrop(Crop* crop)
{
	//隐藏操作按钮
	m_pFarmUILayer->hideOperationBtns();
	SDL_SAFE_RETAIN(crop);
	m_pCropLayer->removeCrop(crop);
	DynamicData::getInstance()->shovelCrop(crop);
	//设置土壤
	auto soil = crop->getSoil();
	soil->setCrop(nullptr);
	crop->setSoil(nullptr);

	SDL_SAFE_RELEASE(crop);
}

void FarmScene::fightCrop(Crop* crop)
{
}

void FarmScene::showWarehouse()
{
}

void FarmScene::showShop()
{
}

void FarmScene::saveData()
{
	DynamicData::getInstance()->save();
	printf("save data success\n");
}

bool FarmScene::preloadResources()
{
	//加载资源
        auto spriteCache = Director::getInstance()->getSpriteFrameCache();

        spriteCache->addSpriteFramesWithFile("sprite/farm_crop_res.xml");
        spriteCache->addSpriteFramesWithFile("sprite/farm_ui_res.xml");
        spriteCache->addSpriteFramesWithFile("sprite/good_layer_ui_res.xml");
        spriteCache->addSpriteFramesWithFile("sprite/slider_dialog_ui_res.xml");

        return true;

}

void FarmScene::initializeSoilsAndCrops()
{
	//读取存档
	auto& farmValueVec = DynamicData::getInstance()->getValueOfKey("soils")->asValueVector();

	for (auto& value : farmValueVec)
	{
		int soilID = 0;
		int soilLv = 0;
		int cropID = 0;
		int startTime = 0;
		int harvestCount = 1;
		float rate = 0.f;
		auto& valueMap = value.asValueMap();

		for (auto it = valueMap.begin(); it != valueMap.end(); it++)
		{
			auto& name = it->first;
			auto& value = it->second;

			if (name == "soil_id")
				soilID = value.asInt();
			else if (name == "soil_lv")
				soilLv = value.asInt();
			else if (name == "crop_id")
				cropID = value.asInt();
			else if (name == "crop_start")
				startTime = value.asInt();
			else if (name == "harvest_count")
				harvestCount = value.asInt();
			else if (name == "crop_rate")
				rate = value.asFloat();
		}
		//生成土壤对象
		Soil* soil = m_pSoilLayer->addSoil(soilID, soilLv);
		//是否存在对应的作物ID
		CropStruct* pCropSt = StaticData::getInstance()->getCropStructByID(cropID);
		if (pCropSt == nullptr)
			continue;

		Crop* crop = m_pCropLayer->addCrop(cropID, startTime, harvestCount, rate);
		crop->setSoil(soil);
		soil->setCrop(crop);
		//设置位置
		crop->setPosition(soil->getPosition());
	}
}

Value FarmScene::getValueOfKey(const string& key)
{
	auto dynamicData = DynamicData::getInstance();
	Value* p = dynamicData->getValueOfKey(key);
	Value value;
	//不存在对应的键，自行设置
	if (p == nullptr)
	{
		if (key == FARM_LEVEL_KEY)
			value = Value(1);
		else if (key == FARM_EXP_KEY)
			value = Value(0);
		else if (key == GOLD_KEY)
			value = Value(0);
		//设置值
		dynamicData->setValueOfKey(key, value);
	}
	else
	{
		value = *p;
	}
	return value;
}
