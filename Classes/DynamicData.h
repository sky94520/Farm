#ifndef __DynamicData_H__
#define __DynamicData_H__
#include <map>
#include <cmath>
#include <string>
#include <vector>
#include <algorithm>

#include "SDL_Engine/SDL_Engine.h"
#include "Good.h"

using namespace std;
USING_NS_SDL;

class Crop;
class Soil;
class Good;
enum class GoodType;

//农场等级和农场经验
#define FARM_LEVEL_KEY "farm_level"
#define FARM_EXP_KEY "farm_exp"
#define GOLD_KEY "gold"
//--------------------------------------------DynamicData---------------------------------------
class DynamicData : public Object
{
private:
	static DynamicData* s_pInstance;
public:
	static DynamicData* getInstance();
	static void purge();
private:
	DynamicData();
	~DynamicData();
private:
	//存档
	ValueMap m_valueMap;
	//是否第一次进入游戏
	bool m_bFirstGame;
	//存档名称
	string m_filename;
	//存档索引
	int m_nSaveDataIndex;
	//背包物品列表
	vector<Good*> m_bagGoodList;
private:
	bool init();
public:
	/* 读取存档
	 * @param idx 对应索引的存档名称
	 */
	bool initializeSaveData(int idx);
	//保存数据
	bool save();
	/**
	 * @param type 物品类型 为扩展作准备
	 * @param goodName 物品名 对于作物 种子来说为ID字符串
	 * @param number 物品的添加个数
	 * @return 返回对应的Good
	*/
	Good* addGood(GoodType type, const string& goodName, int number);
	/**
	 * 减少物品
	 * @param: goodName 物品名
	 * @param: number 减少个数
	 * return: 存在足够的数目则返回true，否则返回false
	 */
	bool subGood(GoodType type, const string& goodName, int number);
	/* 减少物品
	 * @param good 物品对象
	 * @param number 减少物品个数
	 * @return 减少成功返回true,否则返回false
	 */
	bool subGood(Good* good, int number);
	vector<Good*>& getBagGoodList() { return m_bagGoodList; }
	//--------------------------数据库相关---------------------------
	//获取数据
	Value* getValueOfKey(const string& key);
	//设置数据
	void setValueOfKey(const string& key, Value& value);
	//移除数据
	bool removeValueOfKey(const string& key);
	//--------------------------农场相关---------------------------
	//更新作物
	void updateCrop(Crop* crop);
	//更新土壤
	void updateSoil(Soil* soil);
	//铲除作物
	void shovelCrop(Crop* crop);
	//获取对应等级需要的经验
	int getFarmExpByLv(int lv);
private:
	//更新物品存档
	void updateSaveData(ValueVector& array, Good* good);
	//根据类型和名称创建Good
	Good* produceGood(GoodType type, const string& goodName, int number);
};
#endif
