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
            hideHealthPopup();
            onDestroyItem();
            return;
        } else {
            // Animate the bullet impact
            const int IMPACT_ACTION_TAG = 2;
            const float SCALE_START = 1.2f;
            const float DURATION = 0.1f;

            stopActionByTag(IMPACT_ACTION_TAG);
            setScale(SCALE_START);

            auto shrinkAction = EaseIn::create(ScaleTo::create(DURATION,
                                                               1.0f),
                                               2.5f);
            shrinkAction->setTag(IMPACT_ACTION_TAG);

            runAction(shrinkAction);
        }
    }
}

void HealthObject::showHealthPopup(float duration /*= 2.0f*/) {
    CCASSERT(healthFrame, "Call setupHealthBar() after initialization!");

    // Apply the current health to the health bar
    healthBar->setScaleX(health / maxHealth);

    hideHealthPopup();
    healthFrame->setOpacity(255);
    healthBar->setOpacity(255);

    auto removeAction = Sequence::create(DelayTime::create(duration),
                                         FadeOut::create(1.5f),
                                         nullptr);
    removeAction->setTag(HEALTH_ACTION_TAG);
    healthFrame->runAction(removeAction);
    healthBar->runAction(removeAction->clone());
}

void HealthObject::hideHealthPopup() {
    healthFrame->stopActionByTag(HEALTH_ACTION_TAG);
    healthBar->stopActionByTag(HEALTH_ACTION_TAG);
    healthFrame->setOpacity(0);
    healthBar->setOpacity(0);
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


