#include "Bullet.hpp"

USING_NS_CC;

Bullet* Bullet::create(BulletParams params, Vec2 direction) {
    auto bullet = new Bullet(params, direction);

    // TODO: Add bullet Sprite
    if (bullet->init()) {
        bullet->autorelease();
        return bullet;
    }

    CC_SAFE_DELETE(bullet);
    return nullptr;
}

// TODO: Implement bullet movement
void Bullet::update(float delta) {
}

float Bullet::getDamage() const {
    return bulletParams.damage;
}

Bullet::Bullet(BulletParams params, Vec2 normalizedDirection)
    : bulletParams(params),
      direction(normalizedDirection) {
}

