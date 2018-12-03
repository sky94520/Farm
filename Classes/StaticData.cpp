#include "StaticData.h"

StaticData* StaticData::s_pInstance = nullptr;

StaticData::StaticData()
{
}

StaticData::~StaticData()
{
}

bool StaticData::init()
{
	//读取文件并保存键值对
	m_valueMap = FileUtils::getInstance()->getValueMapFromFile(STATIC_DATA_PATH);
	//读取配置文件
	this->loadCropConfigFile();
	this->loadSoilConfigFile();

	return true;
}

bool StaticData::loadCropConfigFile()
{
	int id = 0;
	CropStruct cropSt;

	auto callback = [&id, &cropSt, this](int col, const Value& value)
	{
		switch (col)
		{
			case 0: id = value.asInt(); break;
			case 1: cropSt.name = value.asString(); break;
			case 2: cropSt.desc = value.asString(); break;
			case 3: 
			{
				string text = value.asString();
				string sub;
				size_t begin = 1, end = 1;

				while (end != string::npos)
				{
					end = text.find(' ', begin);

					if (end == string::npos)
						sub = text.substr(begin, text.size() - begin - 1);
					else
					{
						sub = text.substr(begin, end - begin);
						begin = end + 1;
					}
					cropSt.growns.push_back(SDL_atoi(sub.c_str()));
				}
			} 
			break;
			case 4: cropSt.harvestCount = value.asInt(); break;
			case 5: cropSt.seedValue = value.asInt(); break;
			case 6: cropSt.fruitValue = value.asInt(); break;
			case 7: cropSt.number = value.asInt(); break;
			case 8: cropSt.numberVar = value.asInt(); break;
			case 9: cropSt.absorb = value.asString(); break;
			case 10: cropSt.level = value.asInt(); break;
			case 11: 
			{
				cropSt.exp = value.asInt(); 
				//存入数据
				m_cropMap.insert(make_pair(id, cropSt));
				break;
			}
		}
	};
	return this->loadCsvFile("data/crops.csv", callback, 2);
}

bool StaticData::loadSoilConfigFile()
{
	int id = 0;
	ExtensibleSoilStruct soilSt;

	auto callback = [&id, &soilSt, this](int col, const Value& value)
	{
		switch (col)
		{
			case 0: id = value.asInt(); break;
			case 1: soilSt.value = value.asInt(); break;
			case 2: 
				soilSt.lv = value.asInt(); 

				m_extensibleSoilMap.insert(make_pair(id, soilSt));
				break;
		}
	};

	return this->loadCsvFile("data/soils.csv", callback, 2);
}
	
bool StaticData::loadCsvFile(const string& filename
		, const function<void (int, const Value&)>& callback, int skips)
{
	//加载数据
	istringstream in(FileUtils::getInstance()->getDataFromFile(filename));
	string line;
	
	while (getline(in, line))
	{
		if (skips != 0)
		{
			skips--;
			continue;
		}
		//解析数据
		StringUtils::split(line, ",", callback);
	}

	return true;
}

Value* StaticData::getValueForKey(const string& key)
{
	auto iter = m_valueMap.find(key);

	if(iter != m_valueMap.end())
		return &iter->second;

	return nullptr;
}

Point StaticData::getPointForKey(const string& key)
{
	Point point;

	auto value = this->getValueForKey(key);

	if (value != nullptr)
	{
		point = PointFromString(value->asString());
	}
	return point;
}

Size StaticData::getSizeForKey(const string& key)
{
	Size size;

	auto value = this->getValueForKey(key);

	if (value != nullptr)
	{
		size = SizeFromString(value->asString());
	}
	return size;
}

CropStruct* StaticData::getCropStructByID(int id)
{
	auto it = m_cropMap.find(id);
	CropStruct* cropSt = nullptr;

	if (it != m_cropMap.end())
	{
		cropSt = &it->second;
	}

	return cropSt;
}

ExtensibleSoilStruct* StaticData::getExtensibleSoilStructByID(int id)
{
	auto it = m_extensibleSoilMap.find(id);

	if (it == m_extensibleSoilMap.end())
	{
		LOG("not found the soil of id:%d", id);
		return nullptr;
	}
	return &it->second;
}
