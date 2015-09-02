#include "PauseLayer.h"
#include "VisibleRect.h"
#include "CustomTool.h"
#include "Brave.h"
#include "StartScene.h"

bool PauseLayer::init()
{
	if (!LayerColor::init())
	{
		return false;
	}

	this->initWithColor(Color4B(162, 162, 162, 128));
	addUI();
	addTouch();
	return true;
}

void PauseLayer::addUI()
{
	auto background = Sprite::createWithSpriteFrameName("pause-bg.png");
	background->setPosition(VisibleRect::center());
	this->addChild(background);

	auto homeItem = CustomTool::createMenuItemImage("home-1.png", "home-2.png",CC_CALLBACK_1(PauseLayer::home,this));
	auto resumItem = CustomTool::createMenuItemImage("continue-1.png", "continue-2.png", CC_CALLBACK_1(PauseLayer::back, this));
	auto bgsize = background->getContentSize();
	homeItem->setPosition(bgsize.width / 3, bgsize.height / 2);
	resumItem->setPosition(bgsize.width * 2 / 3, bgsize.height / 2);
	auto menu = Menu::create(homeItem, resumItem, NULL);
	menu->setPosition(VisibleRect::leftbottom());
	background->addChild(menu);
}

void PauseLayer::addTouch()
{
	_listener = EventListenerTouchOneByOne::create();
	_listener->onTouchBegan = [&](Touch *touch, Event* event)
	{
		log("PauseLayer::addTouch");
		return true;
	};
	_listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, this);
}

void PauseLayer::home(Ref *obj)
{
	_eventDispatcher->removeEventListener(_listener);
	auto main = (Brave*)this->getParent();
	this->removeFromParentAndCleanup(true);
	main->OnTouchResume();
	auto start = StartScene::createScene();
	Director::getInstance()->replaceScene(start);
}

void PauseLayer::back(Ref *obj)
{
	_eventDispatcher->removeEventListener(_listener);
	auto main = (Brave*)this->getParent();
	this->removeFromParentAndCleanup(true);
	main->OnTouchResume();
}