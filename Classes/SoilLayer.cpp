#include "SoilLayer.h"
#include "Soil.h"

SoilLayer::SoilLayer()
	:m_pTiledMap(nullptr)
{
}

SoilLayer::~SoilLayer()
{
}

bool SoilLayer::init()
{
	m_pTiledMap = TMXTiledMap::create("farm_map/farm.tmx");
	m_pTiledMap->setPosition(50, 150);

	this->addChild(m_pTiledMap);
	
	return true;
}

Soil* SoilLayer::getClickingSoil(const Point& loc)
{
	Soil* soil = nullptr;
	auto it = find_if(m_soilVec.begin(), m_soilVec.end(), [&loc](Soil* soil)
	{
		//菱形对角线大小
		auto size = soil->getContentSize();
		auto soilPos = soil->getPosition();
		//进行坐标的变换
		auto pos = loc - soilPos;
		//计算矩形的大小的一半
		auto halfOfArea = size.width * size.height / 4;
		//计算点击点的面积大小
		auto clickOfArea = fabs(pos.x * size.height * 0.5f) + fabs(pos.y * size.width * 0.5f);
		//判断是否在菱形内
		return clickOfArea <= halfOfArea;

	});

	if (it != m_soilVec.end())
		soil = *it;

	return soil;
}

Point SoilLayer::getSoilPositionByID(int soilID)
{
	//读取土壤层所有图块
	auto layer = m_pTiledMap->getLayer<TMXLayer*>("soil layer");
	auto mapSize = m_pTiledMap->getMapSize();
	int width = (int)mapSize.width;
	int x = soilID % width;
	int y = soilID / width;
	auto sprite = layer->getTileAt(Point(x, y));
	//layer->getChildByTag<Sprite*>(soilID);
	//位置转换
	auto pos = m_pTiledMap->convertToWorldSpace(sprite->getPosition());

	return pos;
}

Sprite* SoilLayer::updateSoil(int soilID, int soilLv)
{
	//读取土壤层所有土壤
	auto layer = m_pTiledMap->getLayer<TMXLayer*>("soil layer");

	//找到对应的土壤精灵
	auto mapSize = m_pTiledMap->getMapSize();
	int width = (int)mapSize.width;

	int x = soilID % width;
	int y = soilID / width;

	auto tileCoordinate = Point(x, y);
	//根据等级设置贴图
	int gid = 3 + soilLv;
	layer->setTileGID(gid, tileCoordinate);

	return layer->getTileAt(tileCoordinate);
}

Soil* SoilLayer::addSoil(int soilID, int soilLv)
{
	//更新并获得土壤精灵
	auto sprite = this->updateSoil(soilID, soilLv);
	Soil* soil = Soil::create(sprite, soilID, soilLv);

	m_soilVec.push_back(soil);
	this->addChild(soil);
	//设置位置 当前位置已经是世界坐标
	auto pos = m_pTiledMap->convertToWorldSpace(sprite->getPosition());

	soil->setPosition(pos);
	
	return soil;
}


