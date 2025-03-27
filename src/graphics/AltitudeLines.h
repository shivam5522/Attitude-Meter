#ifndef ALTITUDE_LINES_H
#define ALTITUDE_LINES_H

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include "Shader.h"
#include "TextRenderer.h"

class AltitudeLines {
public:
    AltitudeLines(float startY, float endY, float interval, float lineLength, float textScale, TextRenderer &textRenderer);
    void draw(Shader &lineShader, Shader &textShader);

private:
    float startY;
    float endY;
    float interval;
    float lineLength;
    float textScale;
    TextRenderer &textRenderer;
    std::vector<float> lineVertices;
    void generateLineVertices();
};

#endif // ALTITUDE_LINES_H