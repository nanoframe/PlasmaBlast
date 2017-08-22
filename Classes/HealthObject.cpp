#include "HealthObject.hpp"

USING_NS_CC;

HealthObject::HealthObject(float maxObjectHealth)
    : health(maxObjectHealth),
      maxHealth(maxObjectHealth),
      active(true) {}

HealthObject::~HealthObject() {}

void HealthObject::update(float delta, Bullet *bullet) {
    
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


