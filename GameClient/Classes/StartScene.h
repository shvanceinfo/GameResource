#ifndef __STARTSCENE_SCENE_H__
#define __STARTSCENE_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

class StartScene : public Layer
{
public:
	bool init();
	CREATE_FUNC(StartScene);
	static Scene* createScene();
	void onStart(Ref* obj);
private:
	EventListenerTouchOneByOne* _listener;
};
#endif // __HELLOWORLD_SCENE_H__
