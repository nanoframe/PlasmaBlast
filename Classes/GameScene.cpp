#include "GameScene.hpp"

#include "Enemy.hpp"

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
    addChild(health, 1);

    setupTouchListener();
    createBulletParams();

    scheduleUpdate();

    return true;
}

void GameScene::update(float delta) {
    // Update touch events and spawn bullets if neccessary
    eventListener->update(delta);
    if (eventListener->canSpawnBullet()) {
        eventListener->resetSpawnTime();

        auto bullet = createBullet(eventListener->getBulletDirection());
        bullets.pushBack(bullet);
        addChild(bullet, 0);
    }

    // Update every game object
    for (HealthObject *object : objects) {
        object->update(delta);
    }

    // Check for bullet collisions
    Vector<Bullet*> deactivatedBullets;
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
        object->checkCollisions(bullet);

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

Bullet* GameScene::createBullet(const Vec2 direction) {
    // Define the BulletParams for future implementation on multiple types
    Bullet::BulletParams &params = normalBullet;

    // Calculate the offset to spawn bullets at the edge of the health ring
    float healthRadius = health->getContentSize().width / 2.0f;
    Vec2 offsetPosition = direction * healthRadius;
    Vec2 spawnPosition = Director::getInstance()->getVisibleSize() / 2.0f;
    spawnPosition += offsetPosition;

    Bullet *bullet = Bullet::create(params, direction);
    bullet->setPosition(spawnPosition);

    return bullet;
}

void GameScene::setupTouchListener() {
    eventListener = GameEventListener::create();
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventListener,
                                                                 this);
    eventListener->setBulletInterval(1.0f / 8.0f);
}

void GameScene::createBulletParams() {
    // Scale, velocity, damage

    normalBullet = {1.0f, 110.0f, 10.0f};
}

