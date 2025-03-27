#ifndef QAURTERCIRCLE_H
#define QAURTERCIRCLE_H

#include "../config.h"

class QuarterCircle {
public:
    unsigned int VAO, VBO;
    int segments;
    QuarterCircle(float x, float y, float radius, int segs, float startAngle, float endAngle);
    void draw();
    ~QuarterCircle();
};

#endif