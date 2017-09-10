#include "Enemy.hpp"

USING_NS_CC;

// Enemy implementation
Enemy::Enemy(float maxHealth, float enemyDamage)
    : HealthObject(maxHealth),
      target(Circle()),
      damage(enemyDamage){
}

void Enemy::onDestroyItem() {
    spawnExplosionParticles(Vec2::ZERO);
    auto delayAction = DelayTime::create(2.5f);
    auto disposeAction = CallFunc::create([this]() {
        removeFromParentAndCleanup(true);
    });
    runAction(Sequence::create(delayAction,
                               disposeAction,
                               nullptr));
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

void Enemy::spawnExplosionParticles(const Vec2 &direction) {
    const float DISTANCE_MIN = 10.0f;
    const float DISTANCE_MAX = 50.0f;
    const float DURATION_MIN = 0.5f;
    const float DURATION_MAX = 1.7f;
    const int PARTICLE_COUNT = 30;
    const float SPAWN_OFFSET = 5.0f;
    const float DIRECTION_OFFSET = 18.0f;

    for (int i = 0; i < PARTICLE_COUNT; i++) {
        // Random spawn position
        Vec2 spawnPosition = Vec2(random<float>(-SPAWN_OFFSET,
                                                SPAWN_OFFSET),
                                  random<float>(-SPAWN_OFFSET,
                                                SPAWN_OFFSET));

        float duration = random<float>(DURATION_MIN, DURATION_MAX);
        Vec2 randomDirection = direction;

        // Random distance
        randomDirection *= random<float>(DISTANCE_MIN,
                                   DISTANCE_MAX);

        // Random movement
        randomDirection.x += random<float>(-DIRECTION_OFFSET, DIRECTION_OFFSET);
        randomDirection.y += random<float>(-DIRECTION_OFFSET, DIRECTION_OFFSET);

        // Create the particle
        auto particle = Sprite::create("attacker-particle.png");
        particle->setPosition(spawnPosition);
        addChild(particle);

        // Create particle actions
        auto movementAction = MoveBy::create(duration,
                                             randomDirection);
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
        // Disable bullet updates
        setIsActive(false);

        // Spawn explosion particles
        Vec2 direction = -getPosition() + getTarget().getCenter();
        direction.negate();
        direction.normalize();
        spawnExplosionParticles(direction);

        // Dispose after two seconds
        scheduleOnce([this](float) { removeFromParentAndCleanup(true); },
                     2.0f,
                     "cleanup");

        canUpdate = false;
    }

    return isColliding;
}

void AttackerEnemy::onDestroyItem() {
    // Hide the enemy
    getObjectImage()->setVisible(false);
    glow->setVisible(false);

    Enemy::onDestroyItem();
}

// ShooterEnemy implementation

Bullet::BulletParams ShooterEnemy::SHOOTER_BULLET_PARAMS = {
    0.5f,  // Scale
    70.0f, // Velocity
    5.0f   // Damage
};

ShooterEnemy::ShooterEnemy(float maxHealth)
    : Enemy(maxHealth, 2.0f) {

        bool reverse = random<int>(0, 1);
        // Rotate in a random direction
        degreeIncrement = ROTATION_INCREMENT * (reverse ? -1.0f : 1.0f);
}

ShooterEnemy* ShooterEnemy::create(float maxHealth) {
    auto enemy = new ShooterEnemy(maxHealth);

    if (enemy->init()) {
        enemy->autorelease();
        enemy->initOptions();
        return enemy;
    }

    CC_SAFE_DELETE(enemy);
    return nullptr;
}

void ShooterEnemy::initOptions() {
    auto image = Sprite::create("shooter.png");
    glow = Sprite::create("shooter-glow.png");
    glow->setPosition(image->getContentSize() / 2.0f);

    image->addChild(glow);

    setObjectImage(image);
}

void ShooterEnemy::updateItem(float delta) {
    updatePosition(delta);
    if (isRevolving) updateAttack(delta);
}

void ShooterEnemy::updatePosition(float delta) {
    // Direction of the enemy towards the player
    Vec2 playerDirection = -getPosition() + getTarget().getCenter();

    // Move the enemy towards the player
    if (playerDirection.lengthSquared() >
            SHOOTING_DISTANCE * SHOOTING_DISTANCE && !isRevolving) {
        playerDirection.normalize();

        const float SPEED = 10.0f;

        // Calculate the angle the Sprite should rotate to be facing towards
        // the player
        Vec2 upVector(0.0f, 1.0f);
        float angle = MATH_RAD_TO_DEG(acosf(upVector.dot(playerDirection)));
        if (playerDirection.x < 0) angle = -angle;

        playerDirection *= SPEED;

        getObjectImage()->setRotation(angle);
        setPosition(getPosition() + playerDirection * delta);

    } else { // Spin around the player

        // Determine which angle to start off with
        if (!isRevolving) {
            isRevolving = true;

            // Calculate the angle the enemy is from the right of the target
            Vec2 enemyTargetDelta = getPosition() -
                                    getTarget().getCenter();
            revolvingAngle = atan2f(enemyTargetDelta.y, enemyTargetDelta.x);
            revolvingAngle = MATH_RAD_TO_DEG(revolvingAngle);

        }

        revolvingAngle += degreeIncrement * delta;

        // Compute the enemy's x-position and y-position
        float angleRadians = MATH_DEG_TO_RAD(revolvingAngle);

        Vec2 enemyPosition(
                cosf(angleRadians) * SHOOTING_DISTANCE,
                sinf(angleRadians) * SHOOTING_DISTANCE);
        enemyPosition += getTarget().getCenter();

        // Convert degrees from 0-right CCW to 0-up CW
        getObjectImage()->setRotation(-revolvingAngle - 90.0f);

        setPosition(enemyPosition);
    }
}

void ShooterEnemy::updateAttack(float delta) {
    bulletTime += delta;
    // Spawn bullets if the time exceeds the threshold
    if (bulletTime >= SHOOTING_DELAY) {
        bulletTime = 0.0f;

        // Calculate the direction of the enemy towards the target
        Vec2 direction = getTarget().getCenter() - getPosition();
        direction.normalize();

        auto bullet = Bullet::create(SHOOTER_BULLET_PARAMS, direction);
        bullet->setPosition(getPosition());
        bullets.pushBack(bullet);
        getParent()->addChild(bullet);
    }

    // Update bullets
    for (Bullet *bullet : bullets) {
        bullet->update(delta);

        /*
         * There is a chance where the bullet will "magically" miss and shoot
         * outside of the screen. This isn't a big issue because although
         * there may be unneccessary updates, the bullet will be disposed
         * once the enemy is destroyed.
         */
    }

}

bool ShooterEnemy::checkForTargetCollisions() {
    bool isHit = false;
    Vector<Bullet*> disposedBullets;

    for (Bullet *bullet : bullets) {
        // The bullet hit the target
        if (getTarget().intersectsRect(bullet->getBoundingBox())) {
            disposedBullets.pushBack(bullet);
            isHit = true;
        }
    }

    // Remove disposed bullets
    for (Bullet *disposed : disposedBullets) {
        disposed->removeFromParentAndCleanup(true);
        bullets.eraseObject(disposed);
    }

    return isHit;
}

void ShooterEnemy::onDestroyItem() {
    for (Bullet *bullet : bullets) {
        bullet->removeFromParentAndCleanup(true);
    }
    bullets.clear();

    // Hide the enemy
    getObjectImage()->setVisible(false);
    glow->setVisible(false);

    Enemy::onDestroyItem();
}

