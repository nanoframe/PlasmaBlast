#ifndef RadialSprite_hpp
#define RadialSprite_hpp

#include "cocos2d.h"

class RadialSprite : public cocos2d::Sprite {
public:
    static RadialSprite* create(const std::string &filename);

    float getProgress() const;
    void setProgress(float newProgress);

private:
    float progress;
};

#endif // RadialSprite_hpp

