#ifndef __CUSTOMTOOL_SCENE_H__
#define __CUSTOMTOOL_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

class CustomTool : public Ref
{
public:
	static CustomTool* getInstance();

	static MenuItemImage* createMenuItemImage(const char* normal, const char* selected, ccMenuCallback callback);

private:
	static CustomTool *_tool;
};
#endif // __HELLOWORLD_SCENE_H__
