#include "AltitudeLines.h"
#include "../config.h"

AltitudeLines::AltitudeLines(float startY, float endY, float interval, float lineLength, float textScale, TextRenderer &textRenderer)
    : startY(startY), endY(endY), interval(interval), lineLength(lineLength), textScale(textScale), textRenderer(textRenderer)
{
    generateLineVertices();
}

void AltitudeLines::generateLineVertices()
{
    for (float y = startY; y <= endY; y += interval)
    {
        // Add a smaller line in the middle of the interval
        float midY = y + interval / 2.0f;
        if (midY <= endY)
        {
            lineVertices.push_back(-lineLength / 4.0f); // x
            lineVertices.push_back(midY);               // y

            lineVertices.push_back(lineLength / 4.0f); // x
            lineVertices.push_back(midY);              // y
        }

        // Add the main line
        lineVertices.push_back(-lineLength / 2.0f); // x
        lineVertices.push_back(y);                  // y

        lineVertices.push_back(lineLength / 2.0f); // x
        lineVertices.push_back(y);                 // y
    }
}

void AltitudeLines::draw(Shader &lineShader, Shader &textShader)
{
    // Draw the lines
    lineShader.use();
    lineShader.setUniformVec4("uColor", glm::value_ptr(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f))); // White color

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, lineVertices.size() * sizeof(float), lineVertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(VAO);
    glLineWidth(2.0f);
    glDrawArrays(GL_LINES, 0, lineVertices.size() / 2);
    glLineWidth(1.0f);
    glBindVertexArray(0);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // Render the text
    glm::vec4 textColor = glm::vec4(1.0f, 1.0f,1.0f, 1.0f); // White color

    // Calculate the range of values (40 to -40)
    float maxValue = 40.0f;
    float step = 10.0f;

    // for (float value = maxValue; value >= -maxValue; value -= step)
    // {
    //     float y = (value / maxValue) * (endY - startY) / 2.0f;
    //     std::string text = std::to_string(std::abs((int)value));

    //     // Don't render 0
    //     if (value == 0)
    //         continue;

    //     // Render text on both sides
    //     textRenderer.renderText(textShader, text, -lineLength / 2.0f - 0.05f, y, textScale, textColor);
    //     textRenderer.renderText(textShader, text, lineLength / 2.0f + 0.05f, y, textScale, textColor);
    // }
}