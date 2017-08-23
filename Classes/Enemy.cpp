#include "Enemy.hpp"

USING_NS_CC;

Enemy::Enemy(float maxHealth)
    : HealthObject(maxHealth),
      target(Vec2::ZERO) {
}

const Vec2& Enemy::getTarget() const {
    return target;
}

void Enemy::setTarget(Vec2 targetPosition) {
    target = targetPosition;
}

