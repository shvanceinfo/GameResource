#include "StartScene.h"
#include "Brave.h"
#include "VisibleRect.h"
#include "CustomTool.h"

bool StartScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	log("StartLayer::init");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("role.plist", "role.pvr.ccz");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ui.plist", "ui.pvr.ccz");
	auto backgroud = Sprite::create("start-bg.jpg");
	backgroud->setPosition(VisibleRect::center());
	this->addChild(backgroud);

	auto item = CustomTool::createMenuItemImage("start1.png", "start2.png", CC_CALLBACK_1(StartScene::onStart, this));
	auto menu = Menu::createWithItem(item);
	this->addChild(menu);
	return true;
}

Scene* StartScene::createScene()
{
	auto scene = Scene::create();
	auto layer = StartScene::create();
	scene->addChild(layer);
	return scene;
}

void StartScene::onStart(Ref* obj)
{
	log("StartLayer::onStart");
	auto scene = Brave::createScene();
	Director::getInstance()->replaceScene(scene);
}