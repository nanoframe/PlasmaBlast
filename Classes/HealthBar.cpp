#include "HealthBar.hpp"

USING_NS_CC;

HealthBar* HealthBar::create() {
    auto healthBar = new HealthBar();

    if (healthBar->init() && healthBar->initSprites()) {
        healthBar->autorelease();
        return healthBar;
    }

    CC_SAFE_DELETE(healthBar);
    return nullptr;
}

bool HealthBar::initSprites() {
    healthDisplay = RadialSprite::create("health-ring.png");
    outline = Sprite::create("ring-outline.png");
    outlineGlow = Sprite::create("ring-outline-glow.png");

    addChild(healthDisplay);
    addChild(outline);
    addChild(outlineGlow);

    return healthDisplay && outline && outlineGlow;
}

float HealthBar::getHealth() const {
    return health;
}

void HealthBar::setHealth(float value) {
    health = clampf(value, 0.0f, MAX_HEALTH);
    healthDisplay->setProgress(health / MAX_HEALTH);
}

