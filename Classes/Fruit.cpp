#include "Fruit.h"
#include "StaticData.h"

Fruit::Fruit()
	:m_nID(0)
	,m_nNumber(0)
{
}

Fruit::~Fruit()
{
}

Fruit* Fruit::create(int id, int number)
{
	auto fruit = new Fruit();

	if (fruit && fruit->init(id, number))
		fruit->autorelease();
	else
		SDL_SAFE_DELETE(fruit);

	return fruit;
}

bool Fruit::init(int id, int number)
{
	m_nID = id;
	m_nNumber = number;

	return true;
}

string Fruit::getGoodName() const
{
	return StringUtils::toString(m_nID);
}

SpriteFrame* Fruit::getIcon() const
{
	auto fruit_format = STATIC_DATA_STRING("fruit_filename_format");
	auto fruitName = StringUtils::format(fruit_format.c_str(), m_nID);
	auto frameCache = Director::getInstance()->getSpriteFrameCache();

	return frameCache->getSpriteFrameByName(fruitName);
}

string Fruit::getName() const
{
	auto cropSt = StaticData::getInstance()->getCropStructByID(m_nID);
	auto type = this->getType();

	string text = StringUtils::format("%s(%s)", cropSt->name.c_str(), type.c_str());
	return text;
}

int Fruit::getNumber() const
{
	return m_nNumber;
}

int Fruit::getCost() const
{
	auto cropSt = StaticData::getInstance()->getCropStructByID(m_nID);
	return cropSt->fruitValue;
}

string Fruit::getDescription() const
{
	auto cropSt = StaticData::getInstance()->getCropStructByID(m_nID);
	return cropSt->desc;
}

string Fruit::getType() const
{
	return STATIC_DATA_STRING("fruit_text");
}

void Fruit::setNumber(int number)
{
	m_nNumber = number;
}

void Fruit::execute(int userID, int targetID)
{
}

bool Fruit::isDeleption() const
{
	return false;
}

GoodType Fruit::getGoodType() const
{
	return GoodType::Fruit;
}
