#ifndef __Toast_H__
#define __Toast_H__

#include <string>
#include "SDL_Engine/SDL_Engine.h"

using namespace std;
USING_NS_SDL;

namespace Toast
{
	/**
	 * 在屏幕中间显示文本
	 * @param parent 父节点
	 * @param text 显示的文本 需要fonts/1.fnt
	 * @param color 文本颜色
	 * @param duration 持续时间
	 */
	void makeText(Node* parent, const string& text, const Color3B& color, float duration);
}
#endif
