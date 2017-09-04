#include "Enemy.hpp"

USING_NS_CC;

// Enemy implementation
Enemy::Enemy(float maxHealth, float enemyDamage)
    : HealthObject(maxHealth),
      target(Circle()),
      damage(enemyDamage){
}

float Enemy::getDamage() const {
    return damage;
}

const Circle& Enemy::getTarget() const {
    return target;
}

void Enemy::setTarget(Circle& targetCircle) {
    target = targetCircle;
}

// AttackerEnemy implementation

AttackerEnemy::AttackerEnemy(float maxHealth)
    : Enemy(maxHealth, 5.0f) {}

AttackerEnemy* AttackerEnemy::create(float maxHealth) {
    auto enemy = new AttackerEnemy(maxHealth);

    if (enemy->init()) {
        enemy->autorelease();

        enemy->initOptions();
        return enemy;
    }

    CC_SAFE_DELETE(enemy);
    return nullptr;
}

void AttackerEnemy::initOptions() {
    auto image = Sprite::create("attacker.png");
    glow = Sprite::create("attacker-glow.png");
    glow->setPosition(image->getContentSize() / 2.0f);

    image->addChild(glow);

    setObjectImage(image);
}

void AttackerEnemy::updateItem(float delta) {
    if (!canUpdate) return;

    // Velocity of the enemy moving towards the target
    const float VELOCITY = 10.0f;

    // Calculate the direction of the enemy relative to the player
    Vec2 movementDelta = -getPosition() + getTarget().getCenter();
    movementDelta.normalize();

    // Calculate the rotation of the enemy
    Vec2 upVector(0.0f, 1.0f);
    float angle = MATH_RAD_TO_DEG(acosf(upVector.dot(movementDelta)));
    if (movementDelta.x < 0) angle = -angle;
    getObjectImage()->setRotation(angle);

    // Move the enemy towards the player
    movementDelta *= VELOCITY;
    setPosition(getPosition() + movementDelta * delta);
}

bool AttackerEnemy::checkForTargetCollisions() {
    if (!canUpdate) return false;

    bool isColliding = getTarget().intersectsRect(getBoundingBox());
    if (isColliding) {
        // Remove the enemy from the screen by hiding it
        glow->removeFromParentAndCleanup(true);
        hideHealthPopup();
        getObjectImage()->setVisible(false);

        spawnExplosionParticles();

        // Dispose after two seconds
        scheduleOnce([this](float) { setIsActive(false); },
                     2.0f,
                     "cleanup");

        canUpdate = false;
    }

    return isColliding;
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

void AttackerEnemy::spawnExplosionParticles() {
    const float DISTANCE_MIN = 10.0f;
    const float DISTANCE_MAX = 50.0f;
    const float DURATION_MIN = 0.5f;
    const float DURATION_MAX = 1.7f;
    const float DIRECTION_OFFSET = 35.0f;
    const int PARTICLE_COUNT = 30.0f;
    const float SPAWN_OFFSET = 5.0f;

    for (int i = 0; i < PARTICLE_COUNT; i++) {
        // Spawn moving in a random direction
        Vec2 direction = -getPosition() + getTarget().getCenter();
        direction.negate();
        direction.x += random<float>(-DIRECTION_OFFSET, DIRECTION_OFFSET);
        direction.y += random<float>(-DIRECTION_OFFSET, DIRECTION_OFFSET);
        direction.normalize();

        // Random spawn position
        Vec2 spawnPosition = Vec2(random<float>(-SPAWN_OFFSET,
                                                SPAWN_OFFSET),
                                  random<float>(-SPAWN_OFFSET,
                                                SPAWN_OFFSET));

        float duration = random<float>(DURATION_MIN, DURATION_MAX);

        // Random distance
        direction *= random<float>(DISTANCE_MIN,
                                   DISTANCE_MAX);

        // Create the particle
        auto particle = Sprite::create("attacker-particle.png");
        particle->setPosition(spawnPosition);
        addChild(particle);

        // Create particle actions
        auto movementAction = MoveBy::create(duration,
                                             direction);
        auto fadeAction = FadeOut::create(0.5f);
        auto disposeAction = CallFunc::create([particle]() {
                particle->removeFromParentAndCleanup(true);
        });
        auto sequenceAction = Sequence::create(EaseOut::create(movementAction,
                                                               2.0f),
                                               fadeAction,
                                               disposeAction,
                                               nullptr);
        particle->runAction(sequenceAction);
    }
}

