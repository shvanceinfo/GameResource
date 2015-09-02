#include "Brave.h"
#include "Player.h"
#include "Fsm.h"
#include "Progress.h"
#include "VisibleRect.h"
#include "CustomTool.h"
#include "PauseLayer.h"
#include "Background.h"
#include "GameOverLayer.h"

Scene* Brave::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
    // 'layer' is an autorelease object
	auto layer = Brave::create();
	layer->setPhysicsWorld(scene->getPhysicsWorld());
    // add layer as a child to scene
    scene->addChild(layer);

	//auto ff = fsm::FSM::create();

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool Brave::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ui.plist", "ui.pvr.ccz");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("role.plist", "role.pvr.ccz");

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

// 	Sprite * background = Sprite::create("background.png");
// 	background->setPosition(origin + visibleSize / 2);
// 	this->addChild(background);
	

// 	player = Player::create(Player::PlayerType::PLAYER);
// 	player->setPosition(origin.x + player->getContentSize().width / 2, origin.y + visibleSize.height / 2);
// 	this->addChild(player);
// 
// 	enemy1 = Player::create(Player::PlayerType::ENEMY1);
// 	enemy1->setPosition(origin.x + visibleSize.width - player->getContentSize().width / 2, origin.y + visibleSize.height / 2);
// 	this->addChild(enemy1);
// 
// 	enemy2 = Player::create(Player::PlayerType::ENEMY2);
// 	enemy2->setPosition(VisibleRect::right().x - player->getContentSize().width / 2, VisibleRect::top().y / 2);
// 	this->addChild(enemy2);

	//player->playAnimationForever(1);
	//enemy1->playAnimationForever(1);
	//enemy2->playAnimationForever(1);

// 	_listener_touch = EventListenerTouchOneByOne::create();
// 	_listener_touch->onTouchBegan = CC_CALLBACK_2(Brave::onTouchBegan, this);
// 	_eventDispatcher->addEventListenerWithSceneGraphPriority(_listener_touch, this);
// 
// 	//auto fsm = fsm::FSM::create("idle", [](){cocos2d::log("enter idle"); });
// 
// 	_listener_contact = EventListenerPhysicsContact::create();
// 	_listener_contact->onContactBegin = CC_CALLBACK_1(Brave::onContactBegin, this);
// 	_listener_contact->onContactSeperate = CC_CALLBACK_1(Brave::onContactSeperate, this);
// 	_eventDispatcher->addEventListenerWithFixedPriority(_listener_contact, 10);
// 
// 	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(Brave::clickEnemy), "clickEnemy", nullptr);


// 	process = Progress::create("player-progress-bg.png", "player-progress-fill.png");
// 	process->setPosition(VisibleRect::left().x + process->getContentSize().width /2, VisibleRect::top().y - process->getContentSize().height / 2);
// 	this->addChild(process);
// 
// 	auto pauseItem = CustomTool::createMenuItemImage("pause1.png", "pause2.png",CC_CALLBACK_1(Brave::onTouchPause,this));
// 	pauseItem->setPosition(VisibleRect::right().x - pauseItem->getContentSize().width / 2, VisibleRect::top().y - pauseItem->getContentSize().height / 2);
// 
// 	auto debugItem = MenuItemImage::create("CloseNormal.png", "CloseSelected.png", CC_CALLBACK_1(Brave::toggleDebug, this));
// 	debugItem->setScale(2.0);
// 	debugItem->setPosition(Vec2(VisibleRect::right().x - debugItem->getContentSize().width - pauseItem->getContentSize().width,
// 		VisibleRect::top().y - debugItem->getContentSize().height));
// 
// 	//_menu = Menu::create(pauseItem, NULL);
// 	_menu = Menu::create(pauseItem, debugItem, NULL);
// 
// 	_menu->setPosition(0, 0);
// 	this->addChild(_menu);
	initLevel();
	addRoles();
	addUI();
	addListener();
	addObserver();

	//this->schedule(schedule_selector(Brave::enemyMove), 1);
	this->schedule(schedule_selector(Brave::enemyMove, 3));
	this->scheduleUpdate();
    return true;
}

void Brave::initLevel()
{
	_level = 0;
	_maxLevel = 2;
	std::vector<Player::PlayerType> types;
	types.push_back(Player::ENEMY1);
	types.push_back(Player::ENEMY2);
	_enemyTypes.push_back(types);

	types.clear();
	types.push_back(Player::ENEMY1);
	types.push_back(Player::ENEMY2);
	types.push_back(Player::ENEMY2);
	_enemyTypes.push_back(types);

	types.clear();
	types.push_back(Player::ENEMY1);
	types.push_back(Player::ENEMY1);
	types.push_back(Player::ENEMY2);
	types.push_back(Player::BOSS);
	_enemyTypes.push_back(types);

	std::vector<Vec2> position;
	position.push_back(VisibleRect::center());
	position.push_back(VisibleRect::right() - Vec2(200, 0));
	_enemyPositions.push_back(position);

	position.clear();
}

void Brave::onEnter()
{
	Layer::onEnter();
	_world->setGravity(Vec2(0, 0));
}

void Brave::toggleDebug(Ref* pSender)
{
	if (_world->getDebugDrawMask()!=PhysicsWorld::DEBUGDRAW_NONE)
	{
		_world->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_NONE);
	}
	else
	{
		_world->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	}
}

bool Brave::onContactBegin(const PhysicsContact& contact)
{
// 	auto playerA = (Player*)contact.getShapeA()->getBody()->getNode();
// 	auto playerB = (Player*)contact.getShapeB()->getBody()->getNode();

	auto playerA = dynamic_cast<Player*>(contact.getShapeA()->getBody()->getNode());
	auto playerB = dynamic_cast<Player*>(contact.getShapeB()->getBody()->getNode());

	auto typeA = playerA->getPlayerType();
	auto typeB = playerB->getPlayerType();

	if (typeA == Player::PlayerType::PLAYER)
	{
		log("contact enmy!");
		playerB->setCanAttack(true);
		playerA->addAttacker(playerB);
	}

	if ( typeB==Player::PlayerType::PLAYER)
	{
		log("contact enemy!");
		playerB->setCanAttack(true);
		playerA->addAttacker(playerB);
	}
	
	return true;
}

void Brave::onContactSeperate(const PhysicsContact& contact)
{
	auto playerA = (Player*)contact.getShapeA()->getBody()->getNode();
	auto playerB = (Player*)contact.getShapeA()->getBody()->getNode();

	auto typeA = playerA->getPlayerType();
	auto typeB = playerB->getPlayerType();

	if (typeA==Player::PlayerType::PLAYER)
	{
		log("lave enemy!");
		playerB->setCanAttack(false);
		playerA->removeAttacker(playerB);
	}

	if (typeB == Player::PlayerType::PLAYER)
	{
		log("leave enemy!");
		playerA->setCanAttack(false);
		playerB->removeAttacker(playerA);
	}
}

void Brave::clickEnemy(Ref* obj)
{
	log("click enemy message received!");
	auto enemy = (Player*)obj;
	if (enemy==nullptr)
	{
		log("enemy null");
		return;
	}

	if (player == nullptr)
	{
		log("player null");
		return;
	}
	
	if (enemy->isCanAttack())
	{
// 		player->attack();
// 		enemy->beHit(player->getAttack());
		if (player->getState() != "attacking")
		{
			player->attack();
			enemy->beHit(player->getAttack());
		}
	}
	else
	{
		player->walkTo(enemy->getPosition());
	}
}

void Brave::addRoles()
{
	player = Player::create(Player::PlayerType::PLAYER);
	player->setPosition(VisibleRect::left().x + player->getContentSize().width / 2, VisibleRect::top().y / 2);
	this->addChild(player, 10);
	addEnemy();
}

void Brave::addEnemy()
{
	enemy1 = Player::create(Player::PlayerType::ENEMY1);
	enemy1->setPosition(VisibleRect::right().x - player->getContentSize().width / 2, VisibleRect::top().y / 2);
	this->addChild(enemy1, 10);
	_enemys.pushBack(enemy1);

	enemy2 = Player::create(Player::PlayerType::ENEMY2);
	enemy2->setPosition(VisibleRect::right().x*2/3 - player->getContentSize().width / 2, VisibleRect::top().y / 2);
	this->addChild(enemy2, 10);
	_enemys.pushBack(enemy2);
}

void Brave::addUI()
{
	_background = Background::create();
	_background->setPosition(0, 0);
	this->addChild(_background,-100);

	process = Progress::create("player-progress-bg.png", "player-progress-fill.png");
	process->setPosition(VisibleRect::left().x + process->getContentSize().width / 2, VisibleRect::top().y - process->getContentSize().height / 2);
	this->addChild(process);

	auto pauseItem = CustomTool::createMenuItemImage("pause1.png", "pause2.png", CC_CALLBACK_1(Brave::onTouchPause, this));
	pauseItem->setTag(1);
	pauseItem->setPosition(VisibleRect::right().x - pauseItem->getContentSize().width / 2, VisibleRect::top().y - pauseItem->getContentSize().height / 2);

	auto debugItem = MenuItemImage::create("CloseNormal.png", "CloseSelected.png", CC_CALLBACK_1(Brave::toggleDebug, this));
	debugItem->setScale(2.0);
	debugItem->setPosition(Vec2(
		VisibleRect::right().x - debugItem->getContentSize().width - pauseItem->getContentSize().width ,
		VisibleRect::top().y - debugItem->getContentSize().height
		));

	auto goItem = CustomTool::createMenuItemImage("go.png", "go.png", CC_CALLBACK_1(Brave::gotoNextLevel, this));
	goItem->setVisible(false);
	goItem->setTag(2);
	goItem->setPosition(VisibleRect::right().x - goItem->getContentSize().width / 2, VisibleRect::center().y);
	_menu = Menu::create(pauseItem, debugItem, goItem, NULL);
	_menu->setPosition(0, 0);
	this->addChild(_menu, 20);
}

void Brave::addListener()
{
	_listener_touch = EventListenerTouchOneByOne::create();
	_listener_touch->onTouchBegan = CC_CALLBACK_2(Brave::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_listener_touch,this);

	_listener_contact = EventListenerPhysicsContact::create();
	_listener_contact->onContactBegin = CC_CALLBACK_1(Brave::onContactBegin, this);
	_listener_contact->onContactSeperate = CC_CALLBACK_1(Brave::onContactSeperate, this);
	_eventDispatcher->addEventListenerWithFixedPriority(_listener_contact, 10);
}

void Brave::addObserver()
{
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(Brave::clickEnemy), "clickEnemy", nullptr);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(Brave::enemyDead), "enemyDead", nullptr);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(Brave::backgroundMoveEnd), "backgroundMoveEnd", nullptr);
}

void Brave::onExit()
{
	Layer::onExit();
	_eventDispatcher->removeEventListener(_listener_contact);
	_eventDispatcher->removeEventListener(_listener_touch);
	NotificationCenter::getInstance()->removeAllObservers(this);
}

void Brave::gotoNextLevel(Ref* obj)
{
	auto goItem = this->_menu->getChildByTag(2);
	goItem->setVisible(false);
	goItem->stopAllActions();

	_background->move("left", player);
}

void Brave::enemyDead(Ref* obj)
{
	auto _player = (Player*)obj;
// 	_enemys.eraseObject(_player, true);
// 	log("onEnemyDead:%d", _enemys.size());
// 	if (_enemys.size()==0)
// 	{
// 		showNextLevelItem();
// 	}

	if (Player::PlayerType::PLAYER == player->getPlayerType())
	{
		player = nullptr;
		auto layer = GameOverLayer::create();
		this->addChild(layer, 10000);
	}
	else
	{
		_enemys.eraseObject(player, true);
		log("onEnemyDead:%d", _enemys.size());
		if (_enemys.size()==0)
		{
			showNextLevelItem();
		}
	}
}

void Brave::backgroundMoveEnd(Ref* obj)
{
	addEnemy();
	log("adding enemy...");
}

void Brave::showNextLevelItem()
{
	auto goItem = this->_menu->getChildByTag(2);
	goItem->setVisible(true);
	goItem->runAction(RepeatForever::create(Blink::create(1, 1)));
}

void Brave::onTouchPause(Ref* sender)
{
// 	player->pause();
// 	enemy1->pause();
// 	enemy2->pause();

	Director::getInstance()->pause();

	auto layer = PauseLayer::create();
	this->addChild(layer, 10000);
}

void Brave::OnTouchResume()
{
// 	player->resume();
// 	enemy1->resume();
// 	enemy2->resume();
	Director::getInstance()->resume();
}

bool Brave::onTouchBegan(Touch *touch, Event *unused_event)
{
	Vec2 pos = this->convertToNodeSpace(touch->getLocation());
	//player->walkTo(pos);
	if (player)
	{
		player->walkTo(pos);
	}
	return true;
}

void Brave::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void Brave::enemyMove(float dt)
{
	for (auto enemy:_enemys)
	{
		if ("dead" !=enemy->getState()&&player&&"dead"!=player->getState())
		{
			if (player->isInRange(enemy))
			{
				if ("attacking"!=enemy->getState())
				{
					enemy->attack();
					player->beHit(enemy->getAttack());
					this->updateHealth();
				}
			}
			else
			{
				enemy->walkTo(player->getPosition());
			}
		}
	}
}

void Brave::updateHealth()
{
	if (player)
	{
		auto health = player->getHealth();
		auto maxHealth = player->getMaxHealth();
		process->setProgress((float)health/maxHealth*100.0);
	}
}

void Brave::update(float dt)
{
	if (player)
	{
		player->setZOrder(VisibleRect::top().y - player->getPosition().y);
	}

	for (auto enemy:_enemys)
	{
		enemy->setZOrder(VisibleRect::top().y - enemy->getPosition().y);
	}
}