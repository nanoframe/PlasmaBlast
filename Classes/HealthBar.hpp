#ifndef HealthBar_hpp
#define HealthBar_hpp

#include "cocos2d.h"
#include "RadialSprite.hpp"

class HealthBar : public cocos2d::Node {
public:
    static HealthBar* create();

    bool initSprites();

    /**
     * Returns the current health previously set by the user.
     *
     * @return  the current health
     */
    float getHealth() const;

    /**
     * Sets the health to be displayed.
     *
     * The value is clamped within [0, MAX_HEALTH].
     *
     * @param value  the value to set
     */
    void setHealth(float value);

private:
    const float MAX_HEALTH = 100;
    float health = 0; // Between [0, MAX_HEALTH]
    
    RadialSprite *healthDisplay;
    cocos2d::Sprite *outline;
    cocos2d::Sprite *outlineGlow;
};

#endif

