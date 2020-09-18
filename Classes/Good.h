#ifndef __Good_H__
#define __Good_H__

#include <string>

#include "SDL_Engine/SDL_Engine.h"
#include "GoodInterface.h"
USING_NS_SDL;
using namespace std;

//物品类型
enum class GoodType
{
	Seed,//种子
	Fruit,//作物 果实
};

class Good : public Object, public GoodInterface
{
public:
	/*
	 * 获取物品名 如 101 或Stick
	 * 主要用于DynamicData中
	 */
	virtual string getGoodName() const = 0;
	//设置数目
	virtual void setNumber(int number) = 0;
	//执行函数
	virtual void execute(int userID, int targetID) = 0;
	//是否是消耗品
	virtual bool isDeleption() const = 0;
	//获取物品类型
	virtual GoodType getGoodType() const = 0;
	//获取类型对应字符串
	static string toString(GoodType type)
	{
		if (type == GoodType::Seed)
			return "Seed";
		else if (type == GoodType::Fruit)
			return "Fruit";
		return "";
	}
	static GoodType toType(const string& str)
	{
		auto type = GoodType::Seed;

		if (str == "Seed")
			type = GoodType::Seed;
		else if (str == "Fruit")
			type = GoodType::Fruit;
		return type;
	}
};
#endif
