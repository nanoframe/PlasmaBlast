#include "Enemy.hpp"

USING_NS_CC;

// Enemy implementation
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

// AttackerEnemy implementation

AttackerEnemy::AttackerEnemy(float maxHealth) : Enemy(maxHealth) {}

AttackerEnemy* AttackerEnemy::create(float maxHealth) {
    auto enemy = new AttackerEnemy(maxHealth);

    if (enemy->initWithFile("attacker.png")) {
        enemy->autorelease();
        
        enemy->setupHealthBar();

        auto glow = Sprite::create("attacker-glow.png");
        glow->setPosition(enemy->getContentSize() / 2.0f);
        enemy->addChild(glow);

        return enemy;
    }

    CC_SAFE_DELETE(enemy);
    return nullptr;
}

void AttackerEnemy::updateItem(float delta) {
    // Velocity of the enemy moving towards the target
    const float VELOCITY = 10.0f;

    // Calculate the direction to move to towards the target

    // Basic vector math
    Vec2 movementDelta = -getPosition() + getTarget();
    movementDelta.normalize();
    movementDelta *= VELOCITY;

    setPosition(getPosition() + movementDelta * delta);
}

void AttackerEnemy::onDestroyItem() {
}

