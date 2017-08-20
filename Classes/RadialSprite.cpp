#include "RadialSprite.hpp"

USING_NS_CC;

RadialSprite* RadialSprite::create(const std::string &filename) {
    auto sprite = new RadialSprite();

    if (sprite->initWithFile(filename)) {
        sprite->autorelease();
        sprite->initOptions();
        return sprite;
    }

    CC_SAFE_DELETE(sprite);
    return nullptr;
}

RadialSprite::~RadialSprite() {
    delete[] triangles.verts;
    delete[] triangles.indices;
}

void RadialSprite::initOptions() {
    initializeVertices();
    updateVertices();
}

float RadialSprite::getProgress() const {
    return progress;
}

void RadialSprite::setProgress(float newProgress) {
    progress = newProgress;
    updateVertices();
}

void RadialSprite::initializeVertices() {
    auto spriteSize = getContentSize();    

    const int CIRCULAR_VERTICES = 9;

    triangles.vertCount = CIRCULAR_VERTICES + 1; // Include the center
    triangles.verts = new V3F_C4B_T2F[triangles.vertCount];

    for (int i = 0; i < CIRCULAR_VERTICES; i++) {
       triangles.verts[i].colors = Color4B::WHITE;
    }

    // Create the last vertices in the center of the Sprite
    int endIndex = triangles.vertCount - 1;
    triangles.verts[endIndex].vertices.x = spriteSize.width / 2.0f;
    triangles.verts[endIndex].vertices.y = spriteSize.height / 2.0f;
    triangles.verts[endIndex].vertices.z = 0.0f;
    triangles.verts[endIndex].texCoords.u = 0.5f;
    triangles.verts[endIndex].texCoords.v = 0.5f;
    triangles.verts[endIndex].colors = Color4B::WHITE;

    const int TRIANGLES_COUNT = 8;
    // 8 triangles where each triangle has 3 vertices
    triangles.indexCount = 3 * TRIANGLES_COUNT;
    triangles.indices = new GLushort[triangles.indexCount];
    for (int i = 0; i < TRIANGLES_COUNT; i++) {
        int vertexIndex = i * 3;
        triangles.indices[vertexIndex] = endIndex;
        triangles.indices[vertexIndex + 1] = i;
        triangles.indices[vertexIndex + 2] = i + 1;
    }
}

void RadialSprite::updateVertices() {
    auto spriteSize = getContentSize();    

    const int CIRCULAR_VERTICES = 9;
    const float DEGREES_INTERVAL = 45.0f;

    /*
     * Create a triangle every 45 degrees moving counter-clockwise.
     * This implies that the 0th triangle will be from 0 - 45 degrees CCW, and
     * the last triangle will be from 315 - 360 degrees
     */
    for (int i = 0; i < CIRCULAR_VERTICES; i++) {
        float degrees = i * DEGREES_INTERVAL;
        // Offset to make 0 degrees point upwards
        degrees -= 90.0f;
        float radians = MATH_DEG_TO_RAD(degrees);

        // Calculate the position of the edge of a given angle of the circle
        // with 0 degrees pointing up moving CCW.
        float positionX = -cosf(radians) * spriteSize.width / 2.0f;
        float positionY = sinf(radians) * spriteSize.height / 2.0f;

        // Vertices are based on a y-down coordinate and has its origin at the
        // top-left corner of the Sprite. The y-position will have to be flipped
        // and will be offset to match the requirements
        positionX += spriteSize.width / 2.0f;
        positionY -= spriteSize.height / 2.0f;
        positionY = -positionY;

        triangles.verts[i].vertices.x = positionX;
        triangles.verts[i].vertices.y = positionY;
        triangles.verts[i].vertices.z = 0.0f;

        // Calculate the UV coordinates of the texture to draw
        float u = positionX / spriteSize.width;
        float v = 1 - (positionY / spriteSize.height); // y-down to y-up
         
        triangles.verts[i].texCoords.u = u;
        triangles.verts[i].texCoords.v = v;
    }

    polygonInfo.setTriangles(triangles);

    setPolygonInfo(polygonInfo);
}

