#include "cocos2d.h"

class Background : public cocos2d::Node {
public:
    static Background* create();

    void initOptions();
    virtual void update(float delta);

private:
    cocos2d::Vec2 particleSpawnPosition;
};
