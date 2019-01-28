#ifndef Bullet_hpp
#define Bullet_hpp

#include "cocos2d.h"

class Bullet : public cocos2d::Sprite {
public:
    /**
     * Defines the bullet properties.
     */
    struct BulletParams {
        float scale;
        float velocity; // Pixels per second
        float damage;
    };

    /**
     * Creates a bullet with the specified properties and direction.
     *
     * @param params     Properties of the bullet
     * @param direction  The bullet's movement direction (normalized)
     *
     * @return  The bullet instance
     */
    static Bullet* create(BulletParams &params, cocos2d::Vec2 direction);

    void update(float delta);

    /**
     * Returns the bullet's damage
     *
     * @return  The bullet's damage
     */
    float getDamage() const;

    /**
     * Returns a bool determining if the bullet is deactivated and should be
     * removed.
     *
     * @return  true if the bullet is deactivated, false if active
     */
    bool isDeactivated() const;

    /**
     * Marks the bullet as inactive to later be disposed of.
     */
    void deactivate();

private:
    Bullet(BulletParams &params, cocos2d::Vec2 normalizedDirection);

    BulletParams& bulletParams;
    cocos2d::Vec2 direction;

    bool deactivated;
};

#endif // Bullet_hpp

