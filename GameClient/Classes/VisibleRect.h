#ifndef __VISIBLERECT_SCENE_H__
#define __VISIBLERECT_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

class VisibleRect
{
public:
	static cocos2d::Rect getVisibleRect();
	static cocos2d::Vec2 left();
	static cocos2d::Vec2 right();
	static cocos2d::Vec2 top();
	static cocos2d::Vec2 bottom();
	static cocos2d::Vec2 center();
	static cocos2d::Vec2 lefttop(); 
	static cocos2d::Vec2 righttop();
	static cocos2d::Vec2 leftbottom(); 
	static cocos2d::Vec2 rightbottom();
	
private:
	static void lazyInit();
	static cocos2d::Rect s_visibleRect;
};
#endif // __HELLOWORLD_SCENE_H__
