#ifndef __CropLayer_H__
#define __CropLayer_H__
#include <vector>
#include <algorithm>
#include "SDL_Engine/SDL_Engine.h"

USING_NS_SDL;
using namespace std;

class Crop;

class CropLayer : public Layer
{
public:
	static string CUSTOM_EVENT_STRING;
private:
	vector<Crop*> m_cropVec;
public:
	CropLayer();
	~CropLayer();

	CREATE_FUNC(CropLayer);
	bool init();
	void update(float dt);
	//添加作物
	Crop* addCrop(int id, int start, int harvestCount, float rate);
	//删除作物
	void removeCrop(Crop* crop);
};
#endif
