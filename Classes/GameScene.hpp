#ifndef GameScene_hpp
#define GameScene_hpp

#include "cocos2d.h"
#include "GameEventListener.hpp"
#include "HealthBar.hpp"

#include "Bullet.hpp"
#include "HealthObject.hpp"

class GameScene : public cocos2d::LayerColor {
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    virtual void update(float delta);

    void updateComponents(float delta, Bullet *bullet);

    CREATE_FUNC(GameScene);

private:
    GameEventListener *eventListener;

    cocos2d::Rect screenBounds;

    HealthBar *health;

    cocos2d::Vector<Bullet*> bullets;
    cocos2d::Vector<HealthObject*> objects;

    void setupTouchListener();
};

#endif // GameScene_hpp

