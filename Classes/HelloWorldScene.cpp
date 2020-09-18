#include "HelloWorldScene.h"

HelloWorld::HelloWorld()
{
}

HelloWorld::~HelloWorld()
{
}

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    Director::getInstance()->getRenderer()->setDrawColor(Color4B(0,255,255));
    //add event listener
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = SDL_CALLBACK_2(HelloWorld::onTouchBegan, this);

    _eventDispatcher->addEventListener(listener, this);
    
	//¼ÓÔØlayer
	auto tiledMap = TMXTiledMap::create("farm_map/farm.tmx");
	this->addChild(tiledMap);

	auto label = LabelDotChar::create("Hello World", 3, Color3B(0, 0, 0));
    label->setTag(10);
    label->setPosition(200, 200);
    this->addChild(label);

    return true;
}

void HelloWorld::menuCloseCallback(Object* pSender)
{
}
bool HelloWorld::onTouchBegan(Touch* touch, SDL_Event* event)
{
	Point location = touch->getLocation();
	auto label = static_cast<LabelBMFont*>(this->getChildByTag(10));
	auto text = StringUtils::format("%d,%d", (int)location.x, (int)location.y);
	label->setString(text);

	return true;
}
