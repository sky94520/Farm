#include "Crop.h"
#include "Soil.h"
#include "StaticData.h"

Crop::Crop()
	:m_bWitherred(false)
	,m_cropID(0)
	,m_startTime(0)
	,m_harvestCount(0)
	,m_cropRate(0.f)
	,m_elpased(0.f)
	,m_hour(0)
	,m_minute(0)
	,m_second(0)
	,m_pSoil(nullptr)
	,_first(true)
{
}

Crop::~Crop()
{
	SDL_SAFE_RELEASE_NULL(m_pSoil);
}

Crop* Crop::create(int id, int startTime, int harvestCount, float rate)
{
	Crop* crop = new Crop();

	if (crop != nullptr && crop->init(id, startTime, harvestCount, rate))
		crop->autorelease();
	else
		SDL_SAFE_DELETE(crop);

	return crop;
}

bool Crop::init(int id, int startTime, int harvestCount, float rate)
{
	//赋值
	m_cropID = id;
	m_startTime = startTime;
	m_harvestCount = harvestCount;
	m_cropRate = rate;
	//获取作物的秒数
	time_t now = time(nullptr);
	time_t deltaSec = now - startTime;
	//计算小时、分钟、和秒数
	m_hour = deltaSec / 3600;
	m_minute = (deltaSec - m_hour * 3600) / 60;
	m_second = deltaSec - m_hour * 3600 - m_minute * 60;

	string spriteName;
	//检测是否已经枯萎
	auto pCropSt = StaticData::getInstance()->getCropStructByID(m_cropID);
	int totalHarvestCount = pCropSt->harvestCount;

	if (m_harvestCount > totalHarvestCount)
	{
		m_bWitherred = true;
		spriteName = STATIC_DATA_STRING("crop_end_filename");
	}
	else
	{
		spriteName = this->getSpriteFrameName();
	}
	//设置贴图
	this->bindSpriteWithSpriteFrameName(spriteName);
	//设置锚点
	if(this->getGrowingStep() == 1)
	{
		this->setAnchorPoint(Point(0.5f, 0.5f));
	}
	else
	{
		this->setAnchorPoint(Point(0.5f, 0.8f));
	}
	return true;
}

void Crop::update(float dt)
{
	//TODO:已经枯萎
	if (m_bWitherred)
		return ;
	m_elpased += dt;
	//第一次直接更新 以后一秒更新一次
	if (m_elpased < 1.f && !_first)
		return;

	_first = false;
	m_elpased = m_elpased - 1.f > 0.f ? m_elpased - 1.f: 0.f;

	int beforeStep = this->getGrowingStep();
	//增加一秒时间
	this->addOneSecond();
	//阶段是否改变
	int afterStep = this->getGrowingStep();
	//贴图将要发生变化
	if (afterStep > beforeStep)
	{
		auto spriteName = this->getSpriteFrameName();

		this->bindSpriteWithSpriteFrameName(spriteName);
	}
}

Soil* Crop::getSoil()
{
	return m_pSoil;
}

void Crop::setSoil(Soil* soil)
{
	SDL_SAFE_RETAIN(soil);
	SDL_SAFE_RELEASE(m_pSoil);
	
	m_pSoil = soil;
}

bool Crop::isRipe() const
{
	auto pCropSt = StaticData::getInstance()->getCropStructByID(m_cropID);

	return pCropSt->growns.back() <= m_hour;
}


int Crop::getGrowingHour(int a, int b)
{
	if ( a > b)
		return -1;

	auto pCropSt = StaticData::getInstance()->getCropStructByID(m_cropID);
	auto& growns = pCropSt->growns;
	auto size = growns.size();

	if (a < 0)
		a = size + a;
	if (b < 0)
		b = size + b;
	//兼容判断
	if (a == b)
		return growns[a];
	else
		return growns[b] - growns[a];
}

int Crop::harvest()
{
	auto staticData = StaticData::getInstance();
	//不可收获，退出
	if ( !this->isRipe())
	{
		return 0;
	}
	string spriteName;
	//获取该作物的总季数
	auto pCropSt = staticData->getCropStructByID(m_cropID);
	int totalHarvestCount = pCropSt->harvestCount;
	//进行收获
	m_harvestCount++;
	//已经超过，则贴图变为枯萎的作物
	if (m_harvestCount > totalHarvestCount)
	{
		spriteName = STATIC_DATA_STRING("crop_end_filename");
		m_bWitherred = true;
	}
	else
	{
		//获取倒数第二个时间段的时间
		int hour = this->getGrowingHour(-2, -2);
		//设置时间
		m_startTime = time(NULL) - hour * 3600;
		m_hour = hour;
		m_minute = 0;
		m_second = 0;

		spriteName = this->getSpriteFrameName();
	}
	this->bindSpriteWithSpriteFrameName(spriteName);
	//获取个数和果实个数浮动值
	int number = pCropSt->number;
	int numberVar = pCropSt->numberVar;

	//获取随机值
	int randomVar = rand() % numberVar + 1;
	float scope = RANDOM_0_1();

	if (fabs(m_cropRate) < scope)
	{
		number += m_cropRate > 0 ? randomVar : -randomVar;
	}

	return number;
}

void Crop::addOneSecond()
{
	m_second++;

	if (m_second >= 60)
	{
		m_minute++;
		m_second -= 60;
	}
	if (m_minute >= 60)
	{
		m_hour++;
		m_minute -= 60;
	}
}

string Crop::getSpriteFrameName()
{
	auto staticData = StaticData::getInstance();
	auto pCropSt = staticData->getCropStructByID(m_cropID);
	string filename;

	auto& growns = pCropSt->growns;
	//获取贴图名称
	//第一阶段 种子
	if (m_hour < growns[0])
	{
		filename = staticData->getValueForKey("crop_start_filename")->asString();
	}
	else
	{
		size_t i = 0;
		while (i < growns.size())
		{
			if (m_hour >= growns[i])
				i++;
			else
				break;
		}
		auto format = staticData->getValueForKey("crop_filename_format")->asString();
		filename = StringUtils::format(format.c_str(), m_cropID, i);
	}
	return filename;
}

int Crop::getGrowingStep()
{
	auto pCropSt = StaticData::getInstance()->getCropStructByID(m_cropID);
	auto& growns = pCropSt->growns;
	auto len = growns.size();
	size_t i = 0;

	while (i < len)
	{
		if (m_hour < growns[i])
			break;
		i++;
	}

	return i + 1;
}
