#ifndef GameEventListener_hpp
#define GameEventListener_hpp

#include "cocos2d.h"

class GameEventListener : public cocos2d::EventListenerTouchOneByOne {
public:
    static GameEventListener *create();
    bool initListener();

    // Touch event listener
    bool touchBegan(cocos2d::Touch *touch, cocos2d::Event *unused);
    void touchMoved(cocos2d::Touch *touch, cocos2d::Event *unused);
    void touchEnded(cocos2d::Touch *touch, cocos2d::Event *unused);
};

#endif // GameEventListener_hpp

