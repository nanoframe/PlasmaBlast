#include "GameScene.hpp"

USING_NS_CC;

Scene* GameScene::createScene() {
    auto scene = Scene::create();
    auto layer = GameScene::create();

    scene->addChild(layer);
    return scene;
}

bool GameScene::init() {
    if (!LayerColor::initWithColor(Color4B(25, 25, 25, 255))) return false;

    auto screenSize = Director::getInstance()->getVisibleSize();

    screenBounds = Rect(Vec2::ZERO, screenSize);

    health = HealthBar::create();
    health->setPosition(screenSize / 2.0f);
    addChild(health);

    setupTouchListener();

    scheduleUpdate();

    return true;
}

void GameScene::update(float delta) {
    Vector<Bullet*> deactivatedBullets;

    // Update all of the game objects from every bullet in the game
    for (Bullet *bullet : bullets) {
        bullet->update(delta);
        updateComponents(delta, bullet);

        // Discard bullets off-screen
        if (!bullet->getBoundingBox().intersectsRect(screenBounds))
            bullet->deactivate();

        // Remove bullets that have made impact
        if (bullet->isDeactivated()) deactivatedBullets.pushBack(bullet);
    }

    for (Bullet *discarded : deactivatedBullets) {
        bullets.eraseObject(discarded);
        discarded->removeFromParentAndCleanup(true);
    }
}

void GameScene::updateComponents(float delta, Bullet *bullet) {
    Vector<HealthObject*> discardedComponents;

    for (HealthObject *object : objects) {
        object->update(delta, bullet);

        if (!object->isActive()) {
            discardedComponents.pushBack(object);
        }

        // Avoid unneccessary object updates from a deactivated bullet
        if (bullet->isDeactivated()) break;
    }

    // Remove unused objects
    for (HealthObject* discarded : discardedComponents) {
        objects.eraseObject(discarded);
    }
}

void GameScene::setupTouchListener() {
    eventListener = GameEventListener::create();
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventListener,
                                                                 this);
    eventListener->setBulletInterval(1.0f / 8.0f);
}

