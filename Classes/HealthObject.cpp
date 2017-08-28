#include "HealthObject.hpp"

USING_NS_CC;

HealthObject::HealthObject(float maxObjectHealth)
    : healthFrame(nullptr),
      healthBar(nullptr),
      health(maxObjectHealth),
      maxHealth(maxObjectHealth),
      active(true) {
    CCASSERT(maxObjectHealth > 0, "Object's maximum health must be > 0!");
}

HealthObject::~HealthObject() {}

void HealthObject::setupHealthBar() {
    const float Y_SPACING = 5.0f;

    // Create the object's health bar
    healthFrame = Sprite::create("health-frame.png");
    healthFrame->setPosition(getContentSize().width / 2.0f,
                             getContentSize().height + Y_SPACING);

    healthBar = Sprite::create("health-progress.png");
    healthBar->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    // Center the health progress bar in the frame and anchor its left edge
    float healthBarX = healthFrame->getContentSize().width / 2.0f -
                       healthBar->getContentSize().width / 2.0f;
    healthBar->setPosition(healthBarX,
                            healthFrame->getContentSize().height / 2.0f);

    // Start hidden
    healthFrame->setOpacity(0);
    healthBar->setOpacity(0);

    healthFrame->addChild(healthBar);
    addChild(healthFrame);
}

// TODO: Add impact animations
void HealthObject::update(float delta) {
    if (!active) return;

    updateItem(delta);
}

void HealthObject::checkCollisions(Bullet *bullet) {
    // Deduct the object's health if the object is hit by a bullet
    if (getBoundingBox().intersectsRect(bullet->getBoundingBox())) {

        // Mark the bullet as inactive to be later disposed by the GameScene
        bullet->deactivate();

        setHealth(health - bullet->getDamage());
        showHealthPopup();

        // Mark the object as inactive to be later disposed by the GameScene
        if (health <= 0) {
            setIsActive(false);
            onDestroyItem();
            return;
        }
    }
}

void HealthObject::showHealthPopup(float duration /*= 2.0f*/) {
    CCASSERT(healthFrame, "Call setupHealthBar() after initialization!");

    const int HEALTH_ACTION_TAG = 1;

    healthFrame->stopActionByTag(HEALTH_ACTION_TAG);
    healthBar->stopActionByTag(HEALTH_ACTION_TAG);
    healthFrame->setOpacity(255);
    healthBar->setOpacity(255);

    auto removeAction = Sequence::create(DelayTime::create(duration),
                                         FadeOut::create(1.5f),
                                         nullptr);
    removeAction->setTag(HEALTH_ACTION_TAG);
    healthFrame->runAction(removeAction);
    healthBar->runAction(removeAction->clone());
}

float HealthObject::getHealth() const {
    return health;
}

void HealthObject::setHealth(float newHealth) {
    health = newHealth;

    // Apply the current health to the health bar
    healthBar->setScaleX(newHealth / maxHealth);
}

bool HealthObject::isActive() const {
    return active;
}

void HealthObject::setIsActive(bool state) {
    active = state;
}


