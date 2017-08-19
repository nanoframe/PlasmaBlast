#include "RadialSprite.hpp"

RadialSprite* RadialSprite::create(const std::string &filename) {
    auto sprite = new RadialSprite();

    if (sprite->initWithFile(filename)) {
        sprite->autorelease();
        return sprite;
    }

    CC_SAFE_DELETE(sprite);
    return nullptr;
}

float RadialSprite::getProgress() const {
    return progress;
}

void RadialSprite::setProgress(float newProgress) {
    progress = newProgress;
}

