#include "HealthObject.hpp"

USING_NS_CC;

HealthObject::HealthObject(float maxObjectHealth)
    : health(maxObjectHealth),
      maxHealth(maxObjectHealth),
      active(true) {}

HealthObject::~HealthObject() {}

void HealthObject::update(float delta, Bullet *bullet) {
    if (!active) return;

    // Deduct the object's health if the object is hit by a bullet
    if (getBoundingBox().intersectsRect(bullet->getBoundingBox())) {

        // Mark the bullet as inactive to be later disposed by the GameScene
        bullet->deactivate();

        health -= bullet->getDamage();

        if (health <= 0) {
            setIsActive(false);
            onDestroyItem();
            return;
        }
    }

    updateItem(delta);
}

float HealthObject::getHealth() const {
    return health;
}

void HealthObject::setHealth(float newHealth) {
    health = newHealth;
}

bool HealthObject::isActive() const {
    return active;
}

void HealthObject::setIsActive(bool state) {
    active = state;
}


