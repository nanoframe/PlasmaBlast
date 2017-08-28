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

        auto glow = Sprite::create("attacker-glow.png");
        glow->setPosition(enemy->getContentSize() / 2.0f);
        enemy->addChild(glow);

        return enemy;
    }

    CC_SAFE_DELETE(enemy);
    return nullptr;
}

void AttackerEnemy::updateItem(float delta) {

}

void AttackerEnemy::onDestroyItem() {

}

