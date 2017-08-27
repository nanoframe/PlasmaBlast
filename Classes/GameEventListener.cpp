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

void GameEventListener::update(float delta) {
    currentTime += delta;
}

bool GameEventListener::canSpawnBullet() {
    return currentTime >= bulletInterval && isTouched;
}

void GameEventListener::resetSpawnTime() {
    currentTime = 0.0f;
}

float GameEventListener::getBulletInterval() const {
    return bulletInterval;
}

Vec2 GameEventListener::getBulletDirection() const {
    auto touchDelta = touchLocation -
                      Vec2(Director::getInstance()->getVisibleSize() / 2.0f);
    
    // Avoid division-by-0 if normalizing a zero vector
    if (touchDelta == Vec2::ZERO) return Vec2(0.0f, 1.0f);
    
    return touchDelta.getNormalized();
}

void GameEventListener::setBulletInterval(float interval) {
    bulletInterval = interval;
}

bool GameEventListener::touchBegan(Touch *touch, Event *unused) {
    isTouched = true;
    touchLocation = touch->getLocation();

    return true;
}

void GameEventListener::touchMoved(Touch *touch, Event *unused) {
    touchLocation = touch->getLocation();
}

void GameEventListener::touchEnded(Touch *touch, Event *unused) {
    isTouched = false;
}


