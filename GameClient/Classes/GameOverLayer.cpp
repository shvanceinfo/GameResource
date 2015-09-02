#include "GameOverLayer.h"
#include "VisibleRect.h"
#include "CustomTool.h"
#include "StartScene.h"

bool GameOverLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Sprite* background = Sprite::createWithSpriteFrameName("pause-bg.png");
	background->setPosition(VisibleRect::center());
	this->addChild(background);

	auto size = background->getContentSize();
	auto title = Sprite::createWithSpriteFrameName("gameover-title.png");
	title->setPosition(size.width / 2, size.height);
	background->addChild(title);

	auto homeItem = CustomTool::createMenuItemImage("home-1", "home-2.png", CC_CALLBACK_1(GameOverLayer::home, this));
	homeItem->setPosition(size.width / 2, size.height / 2);

	auto menu = Menu::create(homeItem, NULL);
	menu->setPosition(VisibleRect::leftbottom());

	_label = LabelTTF::create("", "fonts/Marker Felt.ttf", 334);
	_label->setPosition(size.width / 2, size.height);
	background->addChild(_label);

	background->addChild(menu);

	return true;
}

void GameOverLayer::home(Ref* obj)
{
	this->removeFromParentAndCleanup(true);
	auto start = StartScene::createScene();
	Director::getInstance()->replaceScene(start);
}

void GameOverLayer::setText(const std::string &text)
{
	_label->setString(text);
}