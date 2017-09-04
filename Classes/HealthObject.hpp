#ifndef HealthObject_hpp
#define HealthObject_hpp

#include "cocos2d.h"
#include "Bullet.hpp"

class HealthObject : public cocos2d::Node {
public:
    HealthObject(float maxObjectHealth);
    ~HealthObject();

    void update(float delta);

    /**
     * Checks if the object is colliding with a bullet.
     *
     * @param bullet  The bullet to check against
     */
    void checkCollisions(Bullet *bullet);

    /**
     * Updates the current game object.
     *
     * This method is called after the update(float) method.
     *
     * @param delta  The game's delta time
     */
    virtual void updateItem(float delta) = 0;

    /**
     * Removes the object from the game.
     *
     * Objects with a health of 0 will not be automatically removed; it is up
     * to the implementation to manually remove the object from the game.
     */
    virtual void onDestroyItem() = 0;

    /**
     * Display the object's health indicator for the specified amount of
     * seconds.
     *
     * @param duration  Amount of seconds before hiding the popup
     */
    void showHealthPopup(float duration = 2.0f);

    /**
     * Cancels all health UI actions and hides the health UI.
     */
    void hideHealthPopup();

    /**
     * Return the object's current health.
     *
     * @return  The object's health
     */
    float getHealth() const;

    /**
     * Set the object's health.
     *
     * Calling this method will update the health progress in the popup.
     *
     * @param newHealth  The object's new health
     */
    void setHealth(float newHealth);

    /**
     * Returns a bool determining if the object is currently alive.
     *
     * The active state is determined by the object's current health. If the
     * health is > 0, true will be returned. If the object is 0, false will
     * be returned.
     *
     * The state may be active if the object's health is 0 if
     * setIsActive(true) is called.
     *
     * @return  true if the object's health is > 0, false otherwise
     */
    bool isActive() const;

    /**
     * Set the current state of the object.
     *
     * @param state  The new state of the object
     */
    void setIsActive(bool state);

    virtual const cocos2d::Size& getContentSize() const;
    virtual cocos2d::Rect getBoundingBox() const;

protected:
    void setObjectImage(cocos2d::Sprite *image);

private:
    // Action identifier for the health UI
    const int HEALTH_ACTION_TAG = 1;

    cocos2d::Sprite *objectImage;
    cocos2d::Sprite *healthFrame;
    cocos2d::Sprite *healthBar;

    float health;
    const float maxHealth;

    bool active;

    /**
     * Initializes the health bar of the object.
     */
    void createHealthBar();

    void updateHealthBarPosition();
};

#endif

