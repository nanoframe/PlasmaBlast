#ifndef RadialSprite_hpp
#define RadialSprite_hpp

#include "cocos2d.h"

class RadialSprite : public cocos2d::Sprite {
public:
    static RadialSprite* create(const std::string &filename);

    ~RadialSprite();
     
    void initOptions();

    float getProgress() const;
    void setProgress(float newProgress);

private:
    float progress;

    cocos2d::PolygonInfo polygonInfo;
    cocos2d::TrianglesCommand::Triangles triangles;

    Sprite *progressMask;

    void initializeVertices();
    void updateVertices();
};

#endif // RadialSprite_hpp

