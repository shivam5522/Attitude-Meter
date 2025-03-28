#ifndef CIRCLE_H
#define CIRCLE_H

#include "../config.h"
#include "Shader.h"

class Circle {
public:
    // Vertex data for a circle
    unsigned int VAO, VBO;
    float x, y, radius;
    int segments;

    // Constructor
    Circle(float x, float y, float radius, int segments);

    // Method to draw the circle
    void draw();

    // Additional methods for drawing different styles
    void drawOutline();  // To draw the outline of the circle
    void drawInwardOverlay(float thickness, glm::vec4 color, Shader &shader); // To draw an inward overlay with a specified thickness and color
    void drawOverlayLines(float radius, int segments, Shader &shader, float thickness); // To draw overlay lines on the circle
    void drawTopTriangle(float radius, glm::vec4 color, Shader &shader, float thickness); // To draw a top triangle on the circle
    void drawFork(float radius, glm::vec4 color, Shader &shader, float thickness); // To draw a fork shape on the circle
    
    // Destructor
    ~Circle();
};

#endif