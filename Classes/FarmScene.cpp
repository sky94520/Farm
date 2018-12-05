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

	//初始化土壤和作物
	this->initializeSoilsAndCrops();
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

	if (soil != nullptr)
	{
		printf("clicked soil, the id is %d\n", soil->getSoilID());
	}
	return false;
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
		int harvestCount = 0;
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
		printf("cropID:%d\n", cropID);
		Crop* crop = m_pCropLayer->addCrop(cropID, startTime, harvestCount, rate);
		crop->setSoil(soil);
		soil->setCrop(crop);
		//设置位置
		crop->setPosition(soil->getPosition());
	}
}
