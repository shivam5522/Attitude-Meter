#ifndef QAURTERCIRCLE_H
#define QAURTERCIRCLE_H

#include "../config.h"

class QuarterCircle {
public:
    // Vertex data for a quarter circle
    unsigned int VAO, VBO;
    int segments;

    // Constructor
    QuarterCircle(float x, float y, float radius, int segs, float startAngle, float endAngle);

    // Method to draw the quarter circle
    void draw();

    // Destructor
    ~QuarterCircle();
};

#endif