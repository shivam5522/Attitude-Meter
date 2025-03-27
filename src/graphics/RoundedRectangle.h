#ifndef ROUNDEDRECTANGLE_H
#define ROUNDEDRECTANGLE_H

#include "../config.h"
#include "Rectangle.h"
#include "QuarterCircle.h"
#include "Shader.h"

class RoundedRectangle {
public:
    float x, y, width, height, radius;
    int segments;
    RoundedRectangle(float x, float y, float width, float height, float radius, int segments);
    void draw(Shader& shader);
};

#endif