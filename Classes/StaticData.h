#ifndef __StaticData_H__
#define __StaticData_H__

#include <map>
#include <string>
#include <sstream>

#include "SDL_Engine/SDL_Engine.h"
USING_NS_SDL;
using namespace std;

class Crop;
//定义一些常用的宏
#define STATIC_DATA_PATH "data/static_data.plist"
/*简化使用*/
#define STATIC_DATA_STRING(key) (StaticData::getInstance()->getValueForKey(key)->asString())
#define STATIC_DATA_INT(key) (StaticData::getInstance()->getValueForKey(key)->asInt())
#define STATIC_DATA_FLOAT(key) (StaticData::getInstance()->getValueForKey(key)->asFloat())
#define STATIC_DATA_BOOLEAN(key) (StaticData::getInstance()->getValueForKey(key)->asBool())
#define STATIC_DATA_POINT(key) (StaticData::getInstance()->getPointForKey(key))
#define STATIC_DATA_SIZE(key) (StaticData::getInstance()->getSizeForKey(key))
#define STATIC_DATA_ARRAY(key) (StaticData::getInstance()->getValueForKey(key)->asValueVector())
//作物结构体
struct CropStruct
{
	string name;//作物名称
	string desc;//作物描述
	vector<int> growns;//作物生长期
	int harvestCount;//收货次数
	int seedValue;//种子价格
	int fruitValue;//果实价格
	int number;//果实理论个数
	int numberVar;//果实个数浮动值
	string absorb;//吸引 扩展接口
	int level;//需求等级
	int exp;//得到经验值
};
//土地需求等级和金钱
//当前id 表示扩展的第几块土地 (12-extensible_soil)
struct ExtensibleSoilStruct
{
	int value;//价值
	int lv;//等级
};

class StaticData
{
public:
	static StaticData* getInstance()
	{
		if (s_pInstance == nullptr)
		{
			s_pInstance = new StaticData();
			s_pInstance->init();
		}
		return s_pInstance;
	}
	static void purge()
	{
		SDL_SAFE_DELETE(s_pInstance);
	}
private:
	static StaticData* s_pInstance;
private:
	//键值对
	ValueMap m_valueMap;
	//保存作物配置文件中的数据
	map<int, CropStruct> m_cropMap;
	//保存可扩展土地需要的等级和金钱
	map<int, ExtensibleSoilStruct> m_extensibleSoilMap;
private:
	StaticData();
	~StaticData();

	bool init();
	bool loadCropConfigFile();
	bool loadSoilConfigFile();
	//加载csv文件
	bool loadCsvFile(const string& filename
		, const function<void (int, const Value&)>&, int skips = 0);
public:
	/** 
	 * 根据键获取值
	 * @param key 要查询的键
	 * @return 返回值，如果不存在对应的值，则返回nullptr
	*/
	Value* getValueForKey(const string& key);
	/**
	 * 获取键所对应的值，并转化为Point
	 * @param key 要查询的键
	 * @return 返回值，不存在返回Point::ZERO
	 */
	Point getPointForKey(const string& key);
	/**
	 * 获取键对应的值，并转化为Size
	 * @param key 要查询的键
	 * @return 返回值，不存在则返回Size::ZERO
	 */
	Size getSizeForKey(const string& key);
	//--------------------------------作物相关-----------------------------------
	//获取对应的农场作物结构体
	CropStruct* getCropStructByID(int id);
	//--------------------------------土壤相关-----------------------------------
	//获取对应的扩展土壤结构体
	ExtensibleSoilStruct* getExtensibleSoilStructByID(int id);
};
#endif
