#include "Circle.hpp"

USING_NS_CC;

Circle::Circle() : Circle(0.0f, 0.0f, 0.0f){
}

Circle::Circle(float centerX, float centerY, float newRadius) {
    centerPosition = Vec2(centerX, centerY);
    radius = newRadius;
}

Circle::Circle(const cocos2d::Vec2 &position, float newRadius) {
    centerPosition = Vec2(position);
    radius = newRadius;
}


float Circle::getCenterX() const {
    return centerPosition.x;
}

void Circle::setCenterX(float centerX) {
    centerPosition.x = centerX;
}

float Circle::getCenterY() const {
    return centerPosition.y;
}

void Circle::setCenterY(float centerY) {
    centerPosition.y = centerY;
}

const Vec2& Circle::getCenter() const {
    return centerPosition;
}

void Circle::setCenter(float centerX, float centerY) {
    centerPosition.set(centerX, centerY);
}

void Circle::setCenter(const cocos2d::Vec2 &position) {
    centerPosition = position;
}

float Circle::getRadius() const {
    return radius;
}

void Circle::setRadius(float newRadius) {
    radius = newRadius;
}

float Circle::getCircumference() const {
    return 2 * M_PI * radius;
}

float Circle::getArea() const {
    return M_PI * radius * radius;
}

bool Circle::intersectsRect(Rect rect,
                            float rotation/* = 0.0f*/) const {
    return distanceFromRect(rect, rotation) < radius;
}

float Circle::distanceFromRect(Rect rect,
                               float rotation/* = 0.0f*/) const {
    Vec2 centerRect = Vec2(rect.getMidX(), rect.getMidY());
    Vec2 centerPosition = getCenter(); // The center position of the circle,
                                       // rotated the opposite direction of
                                       // the rect
    centerPosition = centerPosition.rotateByAngle(centerRect, -rotation);
    
    // The point where the ball might intersect the rectangle
    Vec2 intersectPoint = Vec2(
                               clampf(centerPosition.x, rect.getMinX(), rect.getMaxX()),
                               clampf(centerPosition.y, rect.getMinY(), rect.getMaxY())
                               );
    
    
    return (centerPosition - intersectPoint).getLength();
}

bool Circle::intersectsCircle(Circle &circle) const {
    return distanceFromCircle(circle) < circle.getRadius() + radius;
}

float Circle::distanceFromCircle(Circle &circle) const {
    return getCenter().getDistance(circle.getCenter());
}
