#ifndef __GAMEVOER_SCENE_H__
#define __GAMEVOER_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

class GameOverLayer : public Layer
{
public:
	bool init();
	CREATE_FUNC(GameOverLayer);
	void home(Ref* obj);
	void setText(const std::string &text);
private:
	LabelTTF* _label;
};
#endif // __HELLOWORLD_SCENE_H__
