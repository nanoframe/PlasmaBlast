#include "GameEventListener.hpp"

USING_NS_CC;

GameEventListener* GameEventListener::create() {
    auto listener = new GameEventListener();

    if (listener->initListener()) {
        listener->autorelease();
        return listener;
    }

    CC_SAFE_DELETE(listener);
    return nullptr;
}

bool GameEventListener::initListener() {
    if (!EventListenerTouchOneByOne::init()) return false;
    onTouchBegan = CC_CALLBACK_2(GameEventListener::touchBegan, this);
    onTouchMoved = CC_CALLBACK_2(GameEventListener::touchMoved, this);
    onTouchEnded = CC_CALLBACK_2(GameEventListener::touchEnded, this);

    return true;
}

bool GameEventListener::touchBegan(Touch *touch, Event *unused) {
    return true;
}

void GameEventListener::touchMoved(Touch *touch, Event *unused) {

}

void GameEventListener::touchEnded(Touch *touch, Event *unused) {

}

