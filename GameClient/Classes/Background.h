#ifndef __BACKGROUND_SCENE_H__
#define __BACKGROUND_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

class Background : public Layer
{
public:
	bool init();
	CREATE_FUNC(Background);
	void move(const char* direction, Sprite* withSprite);
	void onMoveEnd();

private:
	bool _isMoving;
	Sprite* _spriteA;
	Sprite* _spriteB;
};
#endif // __HELLOWORLD_SCENE_H__
