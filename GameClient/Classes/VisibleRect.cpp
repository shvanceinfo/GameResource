#include "VisibleRect.h"

Rect VisibleRect::s_visibleRect;

void VisibleRect::lazyInit()
{
	s_visibleRect = Director::getInstance()->getOpenGLView()->getVisibleRect();
}

Rect VisibleRect::getVisibleRect()
{
	lazyInit();
	return s_visibleRect;
}

Vec2 VisibleRect::left()
{
	lazyInit();
	return Vec2(s_visibleRect.origin.x, s_visibleRect.origin.y + s_visibleRect.size.height / 2);
}

Vec2 VisibleRect::right()
{
	lazyInit();
	return Vec2(s_visibleRect.origin.x + s_visibleRect.size.width, s_visibleRect.origin.y + s_visibleRect.size.height / 2);
}

Vec2 VisibleRect::top()
{
	lazyInit();
	return Vec2(s_visibleRect.origin.x + s_visibleRect.size.width / 2, s_visibleRect.origin.y + s_visibleRect.size.height);
}

Vec2 VisibleRect::bottom()
{
	lazyInit();
	return Vec2(s_visibleRect.origin.x + s_visibleRect.size.width / 2, s_visibleRect.origin.y);
}

Vec2 VisibleRect::center()
{
	lazyInit();
	return Vec2(s_visibleRect.origin.x + s_visibleRect.size.width / 2, s_visibleRect.origin.y + s_visibleRect.size.height / 2);
}

Vec2 VisibleRect::lefttop()
{
	lazyInit();
	return Vec2(s_visibleRect.origin.x, s_visibleRect.origin.y + s_visibleRect.size.height);
}

Vec2 VisibleRect::righttop()
{
	lazyInit();
	return Vec2(s_visibleRect.origin.x + s_visibleRect.size.width, s_visibleRect.origin.y + s_visibleRect.size.height);
}

Vec2 VisibleRect::leftbottom()
{
	lazyInit();
	return Vec2(s_visibleRect.origin);
}

Vec2 VisibleRect::rightbottom()
{
	lazyInit();
	return Vec2(s_visibleRect.origin.x+ s_visibleRect.size.width, s_visibleRect.origin.y);
}

