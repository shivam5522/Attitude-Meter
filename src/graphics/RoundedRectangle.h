#ifndef ROUNDEDRECTANGLE_H
#define ROUNDEDRECTANGLE_H

#include "../config.h"
#include "Rectangle.h"
#include "QuarterCircle.h"
#include "Shader.h"

class RoundedRectangle {
public:
    // Member variables
    float x, y, width, height, radius;
    int segments;

    // Constructor
    RoundedRectangle(float x, float y, float width, float height, float radius, int segments);

    // Method to draw the rounded rectangle
    void draw(Shader& shader);
};

#endif