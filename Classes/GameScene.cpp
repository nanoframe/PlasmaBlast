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

   health = HealthBar::create();
   health->setPosition(screenSize / 2.0f);
   addChild(health);

   scheduleUpdate();

   return true;
}

void GameScene::update(float delta) {
}

