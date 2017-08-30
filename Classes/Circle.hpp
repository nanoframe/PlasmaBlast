#ifndef Circle_hpp
#define Circle_hpp

#include "cocos2d.h"

class Circle {
public:
    Circle();
    Circle(float centerX, float centerY, float newRadius);
    Circle(const cocos2d::Vec2 &position, float newRadius);
    
    float getCenterX() const;
    void setCenterX(float centerX);

    float getCenterY() const;
    void setCenterY(float centerY);

    const cocos2d::Vec2& getCenter() const;
    void setCenter(float centerX, float centerY);
    void setCenter(const cocos2d::Vec2 &position);
    
    float getRadius() const;
    void setRadius(float newRadius);
 
    float getCircumference() const;
    float getArea() const;
   
    bool intersectsRect(cocos2d::Rect rect,
                        float rotation = 0.0f) const;
    float distanceFromRect(cocos2d::Rect rect,
                        float rotation = 0.0f) const;
    bool intersectsCircle(Circle &circle) const;
    float distanceFromCircle(Circle &circle) const;
    
private:
    cocos2d::Vec2 centerPosition;
    float radius;
};

#endif /* Circle_hpp */
