#include "Enemy.hpp"

USING_NS_CC;

// Enemy implementation
Enemy::Enemy(float maxHealth)
    : HealthObject(maxHealth),
      target(Circle()) {
}

const Circle& Enemy::getTarget() const {
    return target;
}

void Enemy::setTarget(Circle& targetCircle) {
    target = targetCircle;
}

// AttackerEnemy implementation

AttackerEnemy::AttackerEnemy(float maxHealth) : Enemy(maxHealth) {}

AttackerEnemy* AttackerEnemy::create(float maxHealth) {
    auto enemy = new AttackerEnemy(maxHealth);

    if (enemy->initWithFile("attacker.png")) {
        enemy->autorelease();

        enemy->initOptions();
        return enemy;
    }

    CC_SAFE_DELETE(enemy);
    return nullptr;
}

void AttackerEnemy::initOptions() {
    setupHealthBar();

    glow = Sprite::create("attacker-glow.png");
    glow->setPosition(getContentSize() / 2.0f);
    addChild(glow);
}

void AttackerEnemy::updateItem(float delta) {
    // Velocity of the enemy moving towards the target
    const float VELOCITY = 10.0f;

    // Calculate the direction to move to towards the target

    // Basic vector math
    Vec2 movementDelta = -getPosition() + getTarget().getCenter();
    movementDelta.normalize();
    movementDelta *= VELOCITY;

    setPosition(getPosition() + movementDelta * delta);
}

bool AttackerEnemy::checkForTargetCollisions() {
    return false;
}

void AttackerEnemy::onDestroyItem() {
    glow->removeFromParentAndCleanup(true);

    const float ACTION_DURATION = 1.2f;
    const float MAX_MOVEMENT = 20.0f;
    Vec2 movement = Vec2(random<float>(-MAX_MOVEMENT, MAX_MOVEMENT),
                         random<float>(-MAX_MOVEMENT, MAX_MOVEMENT));

    auto rotateAction = RotateBy::create(ACTION_DURATION,
                                         random<float>(180.0f, 540.0f));
    auto moveAction = MoveBy::create(ACTION_DURATION, movement);
    auto destroyAction = Spawn::create(EaseOut::create(rotateAction, 2.0f),
                                       EaseOut::create(moveAction, 2.0f),
                                       nullptr);
    auto fullAction = Sequence::create(destroyAction,
                                       DelayTime::create(0.4f),
                                       CallFunc::create([this]() {
                                           removeFromParentAndCleanup(true);
                                       }),
                                       nullptr);
    runAction(fullAction);
}

