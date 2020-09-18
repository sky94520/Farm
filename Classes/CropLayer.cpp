#include "CropLayer.h"
#include "Crop.h"
#include "StaticData.h"

string CropLayer::CUSTOM_EVENT_STRING = "Crop Ripe";

CropLayer::CropLayer()
{
}

CropLayer::~CropLayer()
{
	for (auto it = m_cropVec.begin(); it != m_cropVec.end();)
	{
		auto crop = *it;
		SDL_SAFE_RELEASE(crop);

		it = m_cropVec.erase(it);
	}
}

bool CropLayer::init()
{
	return true;
}

void CropLayer::update(float dt)
{
	//仅仅通知成熟动画一次
	Crop* pCrop = nullptr;

	for (auto it = m_cropVec.begin(); it != m_cropVec.end(); it++)
	{
		auto crop = *it;
		//更新状态
		crop->update(dt);
		
		//如果有作物成熟
		if (crop->isRipe() && pCrop == nullptr)
		{
			pCrop = crop;
		}
	}
	_eventDispatcher->dispatchCustomEvent(CUSTOM_EVENT_STRING, pCrop);
}

Crop* CropLayer::addCrop(int id, int start, int harvestTime, float rate)
{
	Crop* crop = Crop::create(id, start, harvestTime, rate);
	
	this->addChild(crop);

	SDL_SAFE_RETAIN(crop);
	m_cropVec.push_back(crop);

	return crop;
}

void CropLayer::removeCrop(Crop* crop)
{
	//从容器中删除
	auto it = find(m_cropVec.begin(), m_cropVec.end(), crop);

	if (it != m_cropVec.end())
	{
		m_cropVec.erase(it);
		crop->removeFromParent();
		SDL_SAFE_RELEASE(crop);
	}
}
