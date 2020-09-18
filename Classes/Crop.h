#ifndef __Crop_H__
#define __Crop_H__
#include <time.h>
#include <string>

#include "SDL_Engine/SDL_Engine.h"
#include "Entity.h"

USING_NS_SDL;

class Soil;

class Crop : public Entity
{
	SDL_BOOL_SYNTHESIZE(m_bWitherred, Witherred);//是否是枯萎的 默认为false
private:
	//当前作物ID
	int m_cropID;
	//开始时间 秒数
	time_t m_startTime;
	//作物当前收货季数
	int m_harvestCount;
	//作物修正率[-1~1]
	float m_cropRate;
	//流逝时间 用于1秒更新作物贴图
	float m_elpased;
	//作物小时、分钟和秒数
	int m_hour;
	int m_minute;
	int m_second;
	//设置作物所在土壤
	Soil* m_pSoil;

	bool _first;
public:
	Crop();
	~Crop();

	static Crop* create(int id, int startTime, int harvestCount, float rate);
	bool init(int id, int startTime, int harvestCount, float rate);
	void update(float dt);
	
	Soil* getSoil();
	void setSoil(Soil* soil);
	
	//作物是否成熟
	bool isRipe() const;
	//获取到从a阶段到b阶段的总时间 a的值应小于b
	int getGrowingHour(int a, int b = -1);
	//收获 返回果实的个数，返回-1表示不可收获
	int harvest();
	//获取时间
	int getHour() const { return m_hour; }
	int getMinute() const { return m_minute; }
	int getSecond() const { return m_second; }
	//获取作物ID
	int getCropID() const { return m_cropID; }
	time_t getStartTime() const { return m_startTime; }
	int getHarvestCount() const { return m_harvestCount; }
	float getCropRate() const { return m_cropRate; }
private:
	void addOneSecond();
	//根据当前时间获取作物的贴图名
	string getSpriteFrameName();
	//获取作物的当前生长阶段
	int getGrowingStep();
};
#endif
