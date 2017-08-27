#ifndef GameEventListener_hpp
#define GameEventListener_hpp

#include "cocos2d.h"

class GameEventListener : public cocos2d::EventListenerTouchOneByOne {
public:
    static GameEventListener *create();
    bool initListener();

    void update(float delta);
    bool canSpawnBullet();
    void resetSpawnTime();

    float getBulletInterval() const;
    void setBulletInterval(float interval);

    cocos2d::Vec2 getBulletDirection() const;

    // Touch event listener
    bool touchBegan(cocos2d::Touch *touch, cocos2d::Event *unused);
    void touchMoved(cocos2d::Touch *touch, cocos2d::Event *unused);
    void touchEnded(cocos2d::Touch *touch, cocos2d::Event *unused);

private:
    bool isTouched = false;
    float currentTime = 0.0f;
    float bulletInterval = 0.0f;

    cocos2d::Vec2 touchLocation = cocos2d::Vec2::ZERO;
};

#endif // GameEventListener_hpp

