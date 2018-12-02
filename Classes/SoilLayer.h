#ifndef __SoilLayer_H__
#define __SoilLayer_H__
#include <vector>
#include <iostream>
#include "SDL_Engine/SDL_Engine.h"

USING_NS_SDL;
using namespace std;

class Soil;

class SoilLayer : public Layer
{
public:
	SoilLayer();
	~SoilLayer();

	CREATE_FUNC(SoilLayer);
	bool init();
	/* 获取点击的在m_soilVec数组中的土壤对象
	 * @param pos 世界位置
	 * @return pos对应的土壤对象或nullptr
	 */
	Soil* getClickingSoil(const Point& pos);
	/* 根据id获取土壤精灵的坐标 搜寻的是tiledMap中的土壤精灵
	 * @param z tiled地图对应id
	 * @return 对应瓦片的世界位置
	 */
	Point getSoilPositionByID(int z);
	/* 更新并获得土壤精灵
	 * @param soilLv 土壤ID
	 * @param soilLv 土壤等级
	 * @return soilID对应的精灵
	 */
	Sprite* updateSoil(int soilID, int soilLv);
	/* 根据id和等级生成土壤并返回
	 * @param soilID 土壤id
	 * @param soilLv 土壤等级
	 * @return 初始化完成的土壤对象
	 */
	Soil* addSoil(int soilID, int soilLv);
private:
	//地图
	TMXTiledMap* m_pTiledMap;
	//保存土壤对象
	vector<Soil*> m_soilVec;
};
#endif
