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
    setProgress(1.0f);
}

float RadialSprite::getProgress() const {
    return progress;
}

void RadialSprite::setProgress(float newProgress) {
    // Don't update if the new progress is the same as the current progress
    // (with floating-point errors taken into account)
    if (abs(newProgress - progress) < 0.0001) return;

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
     * Calculate the triangleID based on the current progress value [0, 1].
     * Every triangle is created at an interval of 45 degrees with the ID
     * starting at 1. This means that 0-45 degrees is triangle 1, 45-90 is
     * triangle 2, etc.
     *
     * The progress value is the normalized degrees value.
     * To calculate the ID based on the progress value, the formula is used:
     * triangleID = (int) progress * TOTAL_TRIANGLES
     * TOTAL_TRIANGLES is 360 degrees divided by the interval in which triangles
     * are created.
     */
    int triangleID = (int) (progress * (360 / DEGREES_INTERVAL)) + 1;

    /*
     * Create a triangle every 45 degrees moving counter-clockwise.
     * This implies that the 0th triangle will be from 0 - 45 degrees CCW, and
     * the last triangle will be from 315 - 360 degrees
     */
    for (int i = 0; i < CIRCULAR_VERTICES; i++) {
        float degrees = i * DEGREES_INTERVAL;
 
        // Adjust the vertex position if the given degrees is within the range
        // of the triangle
        if (i == triangleID) {
           degrees = progress * 360.0f; 
        }

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

    // Set the amount of triangles to display based on the progress
    triangles.indexCount = 3 * triangleID;
    polygonInfo.setTriangles(triangles);

    setPolygonInfo(polygonInfo);
}

