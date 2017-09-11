#include "GameScene.hpp"

#include "EventType.hpp"
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

    playerCircle = Circle(Vec2(screenSize / 2.0f),
                          health->getContentSize().width / 2.0f);

    bulletDirectionIndicator = Sprite::create("direction-indicator.png");
    bulletDirectionIndicator->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    // Start with the indicator pointing up
    bulletDirectionIndicator->setPosition(playerCircle.getCenterX(),
                                          playerCircle.getCenterY() +
                                                playerCircle.getRadius());
    addChild(bulletDirectionIndicator, 1);

    score = GameScore::create();
    score->setPosition(playerCircle.getCenter());
    addChild(score, 1);
    subject.addObserver(score);

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

    Vector<HealthObject*> deactivatedObjects;
    // Update every game object
    for (HealthObject *object : objects) {
        object->update(delta);

        // Object-specific updates
        if (Enemy *enemy = dynamic_cast<Enemy*>(object)) {
            // The enemy successfully attacked the player
            if (enemy->checkForTargetCollisions()) {
                // Deduct health
                health->setHealth(health->getHealth() - enemy->getDamage());

                subject.notify(ActionEvent::ENEMY_HIT_PLAYER);
            }
        }

        // Discard inactive objects
        if (!object->isActive()) {
            deactivatedObjects.pushBack(object);
        }
    }
    for (HealthObject *object : deactivatedObjects) {
        objects.eraseObject(object);
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
            subject.notify(ActionEvent::ENEMY_DESTROYED);
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

    // Calculate the angle between the up vector and the direction
    float dotProduct = Vec2(0.0f, 1.0f).dot(direction);
    float angle = MATH_RAD_TO_DEG(acosf(dotProduct));
    if (direction.x < 0) angle = -angle;
    bulletDirectionIndicator->setRotation(angle);
    bulletDirectionIndicator->setPosition(spawnPosition);

    subject.notify(ActionEvent::FIRE_BULLET);

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

// GameScore implementation

GameScore* GameScore::create() {
    auto gameScore = new GameScore();
    
    if (gameScore->initWithTTF("0",
                               "SquaresBold.ttf",
                               FONT_SIZE)) {
        gameScore->autorelease();
        return gameScore;
    }

    CC_SAFE_DELETE(gameScore);
    return nullptr;
}

GameScore::GameScore() : score(0) {
}

void GameScore::setScore(int newScore) {
    score = newScore;
    setString(std::to_string(newScore));
}

int GameScore::getScore() const {
    return score;
}

void GameScore::onNotify(ActionEvent action, int data) {
    if (action == ActionEvent::ENEMY_DESTROYED) {

    }
}

