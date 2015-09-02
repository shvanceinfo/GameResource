#ifndef __BRAVE_SCENE_H__
#define __BRAVE_SCENE_H__

//http://cn.cocos2d-x.org/tutorial/lists?id=85


#include "cocos2d.h"
#include "Player.h"

USING_NS_CC;

class Progress;
class Background;

class Brave : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
	CREATE_FUNC(Brave);


	bool onTouchBegan(Touch *touch, Event *unused_event);

	void onTouchPause(Ref* sender);

	void OnTouchResume();

	void setPhysicsWorld(PhysicsWorld* physicsWorld){ _world = physicsWorld;}

	void toggleDebug(Ref* pSender);

	void onEnter();

	bool onContactBegin(const PhysicsContact& contact);

	void onContactSeperate(const PhysicsContact& contact);

	void clickEnemy(Ref* obj);

	void addRoles();

	void addUI();

	void addListener();

	void addObserver();

	void onExit();

	void gotoNextLevel(Ref* obj);

	void enemyDead(Ref* obj);

	void backgroundMoveEnd(Ref* obj);

	void showNextLevelItem();

	void addEnemy();

	void addEnemyByLevel(int level);

	void enemyMove(float dt);

	void updateHealth();

	void update(float dt);

	void initLevel();
	
	void addOneEnemy(Player::PlayerType type, const Vec2& pos);
private:
	EventListenerTouchOneByOne* _listener_touch;
	EventListenerPhysicsContact* _listener_contact;

	Player *player;
// 	Player *enemy1;
// 	Player *enemy2;

	Progress *process;
	Menu* _menu;
	PhysicsWorld* _world;

	Background* _background;
	Vector<Player*> _enemys;

	int _maxLevel;
	int _level;
	std::vector<std::vector<Player::PlayerType>> _enemyTypes;
	std::vector<std::vector<Vec2>> _enemyPositions;
};

#endif // __HELLOWORLD_SCENE_H__
