#include "Background.hpp"

USING_NS_CC;

Background* Background::create() {
    auto bg = new Background();

    if (bg->init()) {
        bg->initOptions();
        bg->autorelease();

        return bg;
    }

    CC_SAFE_DELETE(bg);
    return nullptr;
}

void Background::initOptions() {
    scheduleUpdate();
    particleSpawnPosition = Director::getInstance()->getVisibleSize() / 2.0f;
}

void Background::update(float delta) {

}
