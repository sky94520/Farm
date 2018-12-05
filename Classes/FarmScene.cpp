#include "FarmScene.h"
#include "SoilLayer.h"
#include "CropLayer.h"
#include "Soil.h"
#include "Crop.h"

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
	//test
	int soilIDs[] = {12, 13, 14, 15, 16, 17};
	auto currTime = time(NULL);

	for (int i = 0; i < 6; i++)
	{
		auto soil = m_pSoilLayer->addSoil(soilIDs[i], 1);

		int id = 101 + i;
		auto startTime = currTime - i * 3600;
		int harvestCount = 0;
		float rate = 0.f;

		auto crop = m_pCropLayer->addCrop(id, startTime, harvestCount, rate);
		crop->setPosition(soil->getPosition());
		crop->setSoil(soil);

		soil->setCrop(crop);
	}
}
