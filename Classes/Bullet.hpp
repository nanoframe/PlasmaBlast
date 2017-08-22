#ifndef Bullet_hpp
#define Bullet_hpp

#include "cocos2d.h"

class Bullet : public cocos2d::Sprite {
public:
    struct BulletParams {
        float scale;
        float velocity; // Pixels per second
        float damage;
    };

    static Bullet* create(BulletParams params, cocos2d::Vec2 direction);

    void update(float delta);

    float getDamage() const;

    bool isDeactivated() const;
    void deactivate();

private:
    Bullet(BulletParams params, cocos2d::Vec2 normalizedDirection);

    BulletParams bulletParams;
    cocos2d::Vec2 direction;

    bool deactivated;
};

#endif // Bullet_hpp

