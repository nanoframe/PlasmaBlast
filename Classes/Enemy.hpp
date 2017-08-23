#ifndef Enemy_hpp
#define Enemy_hpp

#include "cocos2d.h"
#include "HealthObject.hpp"

class Enemy : public HealthObject {
public:
    static Enemy* create();

    Enemy(float maxHealth);

    virtual void updateItem(float delta) = 0;
    virtual void onDestroyItem() = 0;

    const cocos2d::Vec2& getTarget() const;
    void setTarget(cocos2d::Vec2 targetPosition);

private:
    cocos2d::Vec2 target;
};

#endif // Enemy_hpp

