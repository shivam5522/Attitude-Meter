#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "../config.h"

class Rectangle {
public:
    // Vertex data for a rectangle
    unsigned int VAO, VBO;

    // Constructor
    Rectangle(float x, float y, float width, float height);

    // Method to draw the rectangle
    void draw();

    // Destructor
    ~Rectangle();
};

#endif