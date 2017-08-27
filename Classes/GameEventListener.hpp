#ifndef GameEventListener_hpp
#define GameEventListener_hpp

#include "cocos2d.h"

class GameEventListener : public cocos2d::EventListenerTouchOneByOne {
public:
    static GameEventListener *create();
    bool initListener();

    /**
     * Updates the event listener.
     *
     * @param delta  The game's delta time
     */
    void update(float delta);

    /**
     * Returns a bool determining if a certain amount of time has passed in
     * order to spawn a bullet.
     *
     * @return  A bool determining if a bullet should be spawned
     */
    bool canSpawnBullet();

    /**
     * Resets the bullet spawning time.
     */
    void resetSpawnTime();

    /**
     * Returns the interval (in seconds) in which the bullets should be
     * spawned.
     *
     * @return  The interval (in seconds)
     */
    float getBulletInterval() const;

    /**
     * Sets the interval (in seconds) in which the bullets should be spawned.
     *
     * @param interval  The bullet spawning interval (in seconds)
     */
    void setBulletInterval(float interval);

    /**
     * Returns the normalized direction of the bullet.
     * 
     * The bullet's direction is based on the touch location subtracted by the
     * center position of the game.
     *
     * @return  The normalized direction
     */
    cocos2d::Vec2 getBulletDirection() const;

    // Touch event listeners
    bool touchBegan(cocos2d::Touch *touch, cocos2d::Event *unused);
    void touchMoved(cocos2d::Touch *touch, cocos2d::Event *unused);
    void touchEnded(cocos2d::Touch *touch, cocos2d::Event *unused);

private:
    bool isTouched = false;
    float currentTime = 0.0f;
    float bulletInterval = 0.0f;

    cocos2d::Vec2 touchLocation = cocos2d::Vec2::ZERO;
};

#endif // GameEventListener_hpp

