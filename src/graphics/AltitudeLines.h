#ifndef ALTITUDE_LINES_H
#define ALTITUDE_LINES_H

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include "Shader.h"

class AltitudeLines {
public:
    // Constructor
    AltitudeLines(float startY, float endY, float interval, float lineLength, float textScale);

    // Method to generate line vertices and render the text as well
    void draw(Shader &lineShader, Shader &textShader, float rollAngle, float pitchAngle);

// Member variables
private:
    float startY;
    float endY;
    float interval;
    float lineLength;
    float textScale;
    std::vector<float> lineVertices;
    void generateLineVertices();
};

#endif // ALTITUDE_LINES_H