#include "StaticData.h"
#include "CSVLoader.h"

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

	//csv 解析器
	CSVLoader* loader = new CSVLoader();
	loader->loadStr(FileUtils::getInstance()->getDataFromFile("data/crops.csv"));
	//跳过前两行
	loader->skip(2);
	while (loader->hasNextLine())
	{
		id = loader->nextInt();
		cropSt.name = loader->nextStr();
		cropSt.desc = loader->nextStr();
		//生长周期
		string text = loader->nextStr();
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

		cropSt.harvestCount = loader->nextInt();
		cropSt.seedValue = loader->nextInt();
		cropSt.fruitValue = loader->nextInt();
		cropSt.number = loader->nextInt();
		cropSt.numberVar = loader->nextInt();
		cropSt.absorb = loader->nextStr();
		cropSt.level = loader->nextInt();
		cropSt.exp = loader->nextInt();

		//存入数据
		m_cropMap.insert(make_pair(id, cropSt));
		cropSt.growns.clear();
	}

	delete loader;

	return true;
}

bool StaticData::loadSoilConfigFile()
{
	int id = 0;
	ExtensibleSoilStruct soilSt;

	//csv 解析器
	CSVLoader* loader = new CSVLoader();
	loader->loadStr(FileUtils::getInstance()->getDataFromFile("data/soils.csv"));
	//跳过前两行
	loader->skip(2);
	while (loader->hasNextLine())
	{
		id = loader->nextInt();
		soilSt.value = loader->nextInt();
		soilSt.lv = loader->nextInt();

		m_extensibleSoilMap.insert(make_pair(id, soilSt));
	}
	delete loader;
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
