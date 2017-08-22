#ifndef HealthObject_hpp
#define HealthObject_hpp

#include "cocos2d.h"
#include "Bullet.hpp"

class HealthObject : public cocos2d::Sprite {
public:
    HealthObject(float maxObjectHealth);
    ~HealthObject();

    void update(float delta, Bullet *bullet);

    virtual void updateItem(float delta) = 0;
    virtual void onDestroyItem() = 0;

    void setupHealthBar();
    void showHealthPopup(float duration = 2.0f);

    float getHealth() const;
    void setHealth(float newHealth);

    bool isActive() const;
    void setIsActive(bool state);

private:
    cocos2d::Sprite *healthFrame;
    cocos2d::Sprite *healthBar;

    float health;
    const float maxHealth;

    bool active;
};

#endif

