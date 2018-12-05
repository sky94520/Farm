#ifndef __GoodInterface_H__
#define __GoodInterface_H__

#include <string>
#include "SDL_Engine/SDL_Engine.h"

using namespace std;
USING_NS_SDL;
/**
 * GoodLayer所需要的抽象类
 */
class GoodInterface
{
public:
	/*获取icon*/
	virtual SpriteFrame* getIcon() const = 0;
	//物品名称
	virtual string getName() const = 0;
	//物品个数
	virtual int getNumber() const = 0;
	//物品价格
	virtual int getCost() const = 0;
	//物品描述
	virtual string getDescription() const = 0;
	//物品类型 string
	virtual string getType() const = 0;
};

#endif
