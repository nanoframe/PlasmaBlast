#include "Bullet.hpp"

USING_NS_CC;

Bullet* Bullet::create(BulletParams &params, Vec2 direction) {
    auto bullet = new Bullet(params, direction);

    if (bullet->initWithFile("bullet.png")) {
        bullet->autorelease();

        // Calculate the bullet's rotation based on the direction
        auto upVector = Vec2(0.0f, 1.0f);
        float rotation = MATH_RAD_TO_DEG(acos(upVector.dot(direction)));
        if (direction.x < 0) rotation = -rotation;

        bullet->setRotation(rotation);

        return bullet;
    }

    CC_SAFE_DELETE(bullet);
    return nullptr;
}

void Bullet::update(float delta) {
    // Move the bullet
    auto movementDelta = direction * bulletParams.velocity * delta;
    setPosition(getPosition() + movementDelta);
}

float Bullet::getDamage() const {
    return bulletParams.damage;
}

bool Bullet::isDeactivated() const {
    return deactivated;
}

void Bullet::deactivate() {
    deactivated = true;
}

Bullet::Bullet(BulletParams &params, Vec2 normalizedDirection)
    : bulletParams(params),
      direction(normalizedDirection),
      deactivated(false) {
}

