#ifndef CIRCLE_H
#define CIRCLE_H

#include "../config.h"
#include "Shader.h"

class Circle {
public:
    unsigned int VAO, VBO;
    float x, y, radius;
    int segments;
    Circle(float x, float y, float radius, int segments);
    void draw();
    void drawOutline();
    void drawSplit(glm::vec4 color1, glm::vec4 color2, Shader &shader);
    void drawInwardOverlay(float thickness, glm::vec4 color, Shader &shader);
    void drawOverlayLines(float radius, int segments, Shader &shader, float thickness);
    void drawTopTriangle(float radius, glm::vec4 color, Shader &shader, float thickness);
    void drawFork(float radius, glm::vec4 color, Shader &shader, float thickness);
    ~Circle();
};

#endif