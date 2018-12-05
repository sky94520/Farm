#include "DynamicData.h"
#include "Soil.h"
#include "Crop.h"
#include "Seed.h"
#include "Fruit.h"

//--------------------------------------------DynamicData---------------------------------------
DynamicData* DynamicData::s_pInstance = nullptr;

DynamicData* DynamicData::getInstance()
{
	if (s_pInstance == nullptr)
	{
		s_pInstance = new DynamicData();
		s_pInstance->init();
	}
	return s_pInstance;
}

void DynamicData::purge()
{
	SDL_SAFE_RELEASE_NULL(s_pInstance);
}

DynamicData::DynamicData()
	:m_bFirstGame(true)
	,m_nSaveDataIndex(0)
{
}

DynamicData::~DynamicData()
{
	for (auto it = m_bagGoodList.begin(); it != m_bagGoodList.end();)
	{
		auto good = *it;
		SDL_SAFE_RELEASE(good);

		it = m_bagGoodList.erase(it);
	}
}

bool DynamicData::init()
{
	//读取存档
	this->initializeSaveData(1);

	return true;
}

bool DynamicData::initializeSaveData(int idx)
{
	auto fileUtil = FileUtils::getInstance();
	//获取存档路径
	string path = fileUtil->getWritablePath();
	//对应的存档完整路径
	string filepath = m_filename = StringUtils::format("%ssave%d.plist", path.c_str(), idx);
	//不存在对应存档，则使用默认存档
	if ( !fileUtil->isFileExist(m_filename))
	{
		filepath = "data/default_data.plist";m_bFirstGame = true;
	}
	else
		m_bFirstGame = false;

	m_nSaveDataIndex = idx;
	//获得对应存档的键值对
	m_valueMap = fileUtil->getValueMapFromFile(filepath);
	//反序列化背包物品
	auto& goodList = m_valueMap.at("bag_good_list").asValueVector();
	for (auto& value : goodList)
	{
		auto vec = StringUtils::split(value.asString(), " ");

		string sType = vec[0].asString();
		string goodName = vec[1].asString();
		int number = vec[2].asInt();
		//创建并添加
		Good* good = this->produceGood(Good::toType(sType), goodName, number);
		SDL_SAFE_RETAIN(good);
		m_bagGoodList.push_back(good);
	}

	return true;
}

bool DynamicData::save()
{
	//回写存档
	bool ret = FileUtils::getInstance()->writeValueMapToFile(m_valueMap, m_filename);

	return ret;
}

Good* DynamicData::addGood(GoodType type, const string& goodName, int number)
{
	Good* good = nullptr;
	//是否存在该物品
	auto it = find_if(m_bagGoodList.begin(), m_bagGoodList.end(), [&goodName, &type](Good* good)
	{
		return good->getGoodName() == goodName
			&& good->getGoodType() == type;
	});
	//背包中存在该物品
	if (it != m_bagGoodList.end())
	{
		good = *it;
		
		good->setNumber(good->getNumber() + number);
	}//背包中不存在该物品，创建
	else
	{
		good = this->produceGood(type, goodName, number);
		SDL_SAFE_RETAIN(good);

		m_bagGoodList.push_back(good);
	}
	//添加成功，更新存档数据
	if (good != nullptr)
	{
		auto &goodList = m_valueMap["bag_good_list"].asValueVector();

		this->updateSaveData(goodList, good);
	}
	return good;
}

bool DynamicData::subGood(GoodType type, const string& goodName, int number)
{
	bool ret = false;
	//背包中是否存在该物品
	auto it = find_if(m_bagGoodList.begin(),m_bagGoodList.end(),[goodName, type](Good*good)
	{
		return good->getGoodName() == goodName
			&& good->getGoodType() == type;
	});
	//背包中不存在该物品
	if (it == m_bagGoodList.end())
	{
		return false;
	}
	auto good = *it;
	auto oldNumber = good->getNumber();
	SDL_SAFE_RETAIN(good);
	//存在足够的物品 扣除
	if (good->getNumber() > number)
	{
		good->setNumber(oldNumber - number);
		ret = true;
	}//直接删除
	else if (oldNumber == number)
	{
		good->setNumber(0);
		m_bagGoodList.erase(it);
		
		SDL_SAFE_RELEASE(good);
		ret = true;
	}
	//操作成功，才进行存档更新
	if (ret)
	{
		auto &goodList = m_valueMap["bag_good_list"].asValueVector();

		this->updateSaveData(goodList, good);
	}

	SDL_SAFE_RELEASE(good);
	return ret;
}

bool DynamicData::subGood(Good* good, int number)
{
	bool ret = false;
	auto goodNum = good->getNumber();
	SDL_SAFE_RETAIN(good);
	//个数足够
	if (goodNum > number)
	{
		good->setNumber(goodNum - number);
		ret = true;
	}
	else if (goodNum == number)
	{
		good->setNumber(goodNum - number);
		auto it = find_if(m_bagGoodList.begin(),m_bagGoodList.end(),[good](Good* g)
		{
			return good == g;
		});
		if (it != m_bagGoodList.end())
		{
			m_bagGoodList.erase(it);
			SDL_SAFE_RELEASE(good);

			ret = true;
		}
	}
	//操作成功，才进行存档更新
	if (ret)
	{
		auto &goodList = m_valueMap["bag_good_list"].asValueVector();

		this->updateSaveData(goodList, good);
	}
	SDL_SAFE_RELEASE(good);
	return ret;
}

Value* DynamicData::getValueOfKey(const string& key)
{
	Value* value = nullptr;
	//查找
	auto it = m_valueMap.find(key);

	if (it != m_valueMap.end())
	{
		value = &(it->second);
	}
	return value;
}

void DynamicData::setValueOfKey(const string& key, Value& value)
{
	auto it = m_valueMap.find(key);

	if (it != m_valueMap.end())
	{
		it->second = value;
	}
	else//直接插入
	{
		m_valueMap.insert(make_pair(key, value));
	}
}

bool DynamicData::removeValueOfKey(const string& key)
{
	auto it = m_valueMap.find(key);
	bool bRet = false;

	if (it != m_valueMap.end())
	{
		m_valueMap.erase(it);
		bRet = true;
	}
	return bRet;
}

void DynamicData::updateCrop(Crop* crop)
{
	//获取作物相关信息
	int cropID = crop->getCropID();
	int cropStart = crop->getStartTime();
	int harvestCount = crop->getHarvestCount();
	float cropRate = crop->getCropRate();
	//获取作物对应土壤
	auto soil = crop->getSoil();	
	auto soilID = soil->getSoilID();
	//获取对应存档valueMap
	auto& soilArr = m_valueMap["soils"].asValueVector();

	//找到对应的土壤，并更新
	for (auto& value : soilArr)
	{
		auto& dict = value.asValueMap();

		if (dict["soil_id"].asInt() == soilID)
		{
			dict["crop_start"] = Value(cropStart);
			dict["harvest_count"] = Value(harvestCount);
			dict["crop_rate"] = Value(cropRate);
			dict["crop_id"] = Value(cropID);
			break;
		}
	}
}

void DynamicData::updateSoil(Soil* soil)
{	
	//获取作物对应土壤
	auto soilID = soil->getSoilID();
	auto soilLv = soil->getSoilLv();
	//获取对应存档valueMap
	auto& soilArr = m_valueMap["soils"].asValueVector();
	ValueMap* map = nullptr;

	for (auto& value : soilArr)
	{
		auto& dict = value.asValueMap();

		if (dict["soil_id"].asInt() == soilID)
		{
			map = &dict;
			break;
		}
	}
	//没有该土壤则创建一个
	if (map == nullptr)
	{
		soilArr.push_back(Value(ValueMap()));
		map = &(soilArr.back().asValueMap());
	}
	map->insert(std::make_pair("soil_id", Value(soilID)));
	map->insert(std::make_pair("soil_lv", Value(soilLv)));
}

void DynamicData::shovelCrop(Crop* crop)
{
	//获取作物对应土壤
	auto soil = crop->getSoil();	
	auto soilID = soil->getSoilID();
	//获取对应存档valueMap
	auto& soilArr = m_valueMap["soils"].asValueVector();
	ValueMap* map = nullptr;

	for (auto& value : soilArr)
	{
		auto& dict = value.asValueMap();

		if (dict["soil_id"].asInt() == soilID)
		{
			map = &dict;
			break;
		}
	}
	//删除crop_id crop_start harvest_count crop_rate
	string strs[] = {"crop_id", "crop_start", "harvest_count", "crop_rate"};

	for (auto key : strs)
	{
		auto it = map->find(key);

		if (it != map->end())
		{
			map->erase(it);
		}
	}
}

int DynamicData::getFarmExpByLv(int lv)
{
	return lv * 200;
}

void DynamicData::updateSaveData(ValueVector& array, Good* good)
{
	auto goodName = good->getGoodName();
	auto number = good->getNumber();
	auto sType = Good::toString(good->getGoodType());

	ValueVector::iterator it;
	//获得对应的迭代器
	for (it = array.begin();it != array.end(); it++)
	{
		auto str = it->asString();
		//先按名称寻找
		auto index = str.find(goodName);
		//判断类型是否正确
		if (index != string::npos && str.find(sType) != string::npos)
		{
			break;
		}
	}
	//物品类型 物品ID 物品个数
	string	text = StringUtils::format("%s %s %d",sType.c_str(), goodName.c_str(), number);
	//找到对应字段，则进行覆盖
	if (it != array.end())
	{
		if (number > 0)
			array[it - array.begin()] = Value(text);
		else if (number == 0)
			array.erase(it);
	}
	else if (number > 0)//物品个数大于0，在后面添加
	{
		array.push_back(Value(text));
	}
}

Good* DynamicData::produceGood(GoodType type, const string& goodName, int number)
{
	Good* good = nullptr;
	switch (type)
	{
		case GoodType::Seed: good = Seed::create(atoi(goodName.c_str()), number); break;
		case GoodType::Fruit: good = Fruit::create(atoi(goodName.c_str()), number); break;

		default: LOG("not found the type %s\n", Good::toString(type).c_str());
	}
	return good;
}
