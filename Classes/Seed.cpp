#include "Seed.h"
#include "StaticData.h"

Seed::Seed()
	:m_nID(0)
	,m_nNumber(0)
{
}

Seed::~Seed()
{
}

Seed* Seed::create(int id, int number)
{
	auto seed = new Seed();

	if (seed && seed->init(id, number))
		seed->autorelease();
	else
		SDL_SAFE_DELETE(seed);

	return seed;
}

bool Seed::init(int id, int number)
{
	m_nID = id;
	m_nNumber = number;

	return true;
}

string Seed::getGoodName() const
{
	return StringUtils::toString(m_nID);
}

SpriteFrame* Seed::getIcon() const
{
	auto fruit_format = STATIC_DATA_STRING("fruit_filename_format");
	auto fruitName = StringUtils::format(fruit_format.c_str(), m_nID);
	auto frameCache = Director::getInstance()->getSpriteFrameCache();

	return frameCache->getSpriteFrameByName(fruitName);
}

string Seed::getName() const
{
	auto cropSt = StaticData::getInstance()->getCropStructByID(m_nID);
	auto type = this->getType();

	string text = StringUtils::format("%s(%s)", cropSt->name.c_str(), type.c_str());
	return text;
}

int Seed::getNumber() const
{
	return m_nNumber;
}

int Seed::getCost() const
{
	auto cropSt = StaticData::getInstance()->getCropStructByID(m_nID);
	return cropSt->seedValue;
}

string Seed::getDescription() const
{
	auto format = STATIC_DATA_STRING("seed_desc_format");
	auto cropSt = StaticData::getInstance()->getCropStructByID(m_nID);

	//先生成种子属性
	auto text = StringUtils::format(format.c_str(), cropSt->level, cropSt->exp
			, cropSt->harvestCount, cropSt->number);
	//添加描述
	auto text2 = StringUtils::format("%s\n%s", text.c_str(), cropSt->desc.c_str());
	return text2;
}

string Seed::getType() const
{
	return STATIC_DATA_STRING("seed_text");
}

void Seed::setNumber(int number)
{
	m_nNumber = number;
}

void Seed::execute(int userID, int targetID)
{
}

bool Seed::isDeleption() const
{
	return false;
}

GoodType Seed::getGoodType() const
{
	return GoodType::Seed;
}
