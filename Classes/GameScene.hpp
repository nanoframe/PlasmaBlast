#ifndef GameScene_hpp
#define GameScene_hpp

#include "cocos2d.h"

class GameScene : public cocos2d::LayerColor {
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    virtual void update(float delta);

    CREATE_FUNC(GameScene);

private:
};

#endif // GameScene_hpp

