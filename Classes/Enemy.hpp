#ifndef Enemy_hpp
#define Enemy_hpp

#include "cocos2d.h"
#include "HealthObject.hpp"
#include "Circle.hpp"

class Enemy : public HealthObject {
public:
    Enemy(float maxHealth, float enemyDamage);

    virtual void updateItem(float delta) = 0;
    virtual bool checkForTargetCollisions() = 0;
    virtual void onDestroyItem() = 0;

    /**
     * Returns the amount of health the enemy will deduct from the player.
     *
     * @return  The damage amount
     */
    float getDamage() const;

    /**
     * Returns the target in which the enemy will try to attack to.
     *
     * @return  The Vec2 position of the target
     */
    const Circle& getTarget() const;

    /**
     * Sets the target for the enemy to attack to.
     *
     * @param target  The Circle object of the target
     */
    void setTarget(Circle &targetCircle);

private:
    Circle target;
    float damage;
};

class AttackerEnemy : public Enemy {
public:
    AttackerEnemy(float maxHealth);

    static AttackerEnemy* create(float maxHealth);

    void initOptions();

    virtual void updateItem(float delta);
    virtual bool checkForTargetCollisions();
    virtual void onDestroyItem();

private:
    cocos2d::Sprite *glow;
    bool canUpdate = true;

    /**
     * Spawn explosion particles moving away from the target.
     */
    void spawnExplosionParticles();
};

class ShooterEnemy : public Enemy {
public:
    const float SHOOTING_DISTANCE = 100.0f; // Distance away from the target to
                                            // begin shooting
    const float ROTATION_INCREMENT = 15.0f; // Degrees to increment every
                                            // second while shooting

    ShooterEnemy(float maxHealth);

    static ShooterEnemy* create(float maxHealth);

    void initOptions();

    virtual void updateItem(float delta);
    virtual bool checkForTargetCollisions();
    virtual void onDestroyItem();

private:
    cocos2d::Sprite *glow;

    bool isRevolving = false;
    float revolvingAngle = 0.0f;
    float degreeIncrement;
};

#endif // Enemy_hpp

