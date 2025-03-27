#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "../config.h"

class Rectangle {
public:
    unsigned int VAO, VBO;
    Rectangle(float x, float y, float width, float height);
    void draw();
    ~Rectangle();
};

#endif