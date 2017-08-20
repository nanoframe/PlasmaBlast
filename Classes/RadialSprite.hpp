#ifndef RadialSprite_hpp
#define RadialSprite_hpp

#include "cocos2d.h"

/**
 * A class for displaying a Sprite as a radial progress bar.
 *
 * It is required to add some padding to the images to prevent the edges of the
 * image from being clipped.
 */
class RadialSprite : public cocos2d::Sprite {
public:
    static RadialSprite* create(const std::string &filename);

    RadialSprite();
    ~RadialSprite();

    void initOptions();

    /**
     * Returns the current progress displayed by the Sprite.
     *
     * @return  the current progress
     */
    float getProgress() const;

    /**
     * Sets and updates the progress of the radial progress bar.
     *
     * @param newProgress  the progress to display between [0, 1]
     */
    void setProgress(float newProgress);

private:
    float progress = 1.0f;

    cocos2d::PolygonInfo polygonInfo;
    cocos2d::TrianglesCommand::Triangles triangles;

    Sprite *progressMask;

    /**
     * Generates an array of vertices and indices for use in displaying the
     * radial progress.
     */
    void initializeVertices();
    void updateVertices();
};

#endif // RadialSprite_hpp

