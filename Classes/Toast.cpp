#include "Toast.h"

namespace Toast
{
	void makeText(Node* parent, const string& text, const Color3B& color, float duration)
	{
		auto visibleSize = Director::getInstance()->getVisibleSize();

		FadeIn* fadeIn = FadeIn::create(duration / 4);
		FadeOut* fadeOut = FadeOut::create(duration / 4);
		DelayTime* delayTime = DelayTime::create(duration / 2);
		RemoveSelf* removeSelf = RemoveSelf::create();
		auto seq = Sequence::create(fadeIn, delayTime, fadeOut, removeSelf, nullptr);

		LabelBMFont* label = LabelBMFont::create(text, "fonts/1.fnt");
		auto size = label->getContentSize();

		label->setColor(color);
		//创建背景
		auto bg = LayerColor::create(Color4B(0, 0, 0, 128), size.width, size.height);
		bg->setPosition((visibleSize.width - size.width) / 2
				, (visibleSize.height - size.height) / 2);
		bg->setCascadeOpacityEnabled(true);
		bg->addChild(label);
		bg->runAction(seq);

		label->setPosition(size.width / 2, size.height / 2);

		parent->addChild(bg);
	}
}
