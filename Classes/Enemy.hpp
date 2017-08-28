#ifndef Enemy_hpp
#define Enemy_hpp

#include "cocos2d.h"
#include "HealthObject.hpp"

class Enemy : public HealthObject {
public:
    Enemy(float maxHealth);

    virtual void updateItem(float delta) = 0;
    virtual void onDestroyItem() = 0;

    /**
     * Returns the target in which the enemy will try to attack to.
     *
     * @return  The Vec2 position of the target
     */
    const cocos2d::Vec2& getTarget() const;

    /**
     * Sets the target for the enemy to attack to.
     *
     * @param targetPosition  The Vec2 position of the target
     */
    void setTarget(cocos2d::Vec2 targetPosition);

private:
    cocos2d::Vec2 target = cocos2d::Vec2::ZERO;
};

class AttackerEnemy : public Enemy {
public:
    AttackerEnemy(float maxHealth);

    static AttackerEnemy* create(float maxHealth);

    virtual void updateItem(float delta);
    virtual void onDestroyItem();
};

#endif // Enemy_hpp

