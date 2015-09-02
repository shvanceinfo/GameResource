#ifndef __PROGRESS_SCENE_H__
#define __PROGRESS_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

class Progress : public Sprite
{
public:
	bool init(const char* background, const char* fillname);
	static Progress* create(const char* background, const char* fill);
	void setFill(ProgressTimer* fill){ _fill = fill; }
	void setProgress(float percentage){ _fill->setPercentage(percentage); }
private:
	ProgressTimer *_fill;
};
#endif // __HELLOWORLD_SCENE_H__
