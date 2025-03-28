#include "AltitudeLines.h"
#include "../config.h"

// Constructor
AltitudeLines::AltitudeLines(float startY, float endY, float interval, float lineLength, float textScale)
    : startY(startY), endY(endY), interval(interval), lineLength(lineLength), textScale(textScale)
{
    generateLineVertices();
}

// Generate line vertices for the altitude lines
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

// Function to render text using stb_easy_font
void renderText(Shader &textShader, const glm::mat4 &baseTransform, float x, float y, const std::string &text, float scale, glm::vec3 color) {
    char buffer[99999]; // Large enough buffer for vertex data
    int numQuads = stb_easy_font_print(0, 0, const_cast<char *>(text.c_str()), NULL, buffer, sizeof(buffer));

    // Create a VAO and VBO for the text
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Bind the VAO and VBO
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, numQuads * 4 * 16, buffer, GL_STATIC_DRAW);

    // Define vertex attributes
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 16, (void *)0);
    glEnableVertexAttribArray(0);

    // Use the text shader
    textShader.use();

    // Set the text color
    textShader.setUniformVec3("uTextColor", glm::value_ptr(color));

    // Create the final transformation matrix
    glm::mat4 transform = baseTransform;
    transform = glm::translate(transform, glm::vec3(x, y, 0.0f)); // Translate to position
    transform = glm::scale(transform, glm::vec3(scale, -scale, 1.0f)); // Scale and flip Y-axis
    textShader.setUniformMat4("uTransform", glm::value_ptr(transform));

    // Render the text
    glBindVertexArray(VAO);
    glDrawArrays(GL_QUADS, 0, numQuads * 4);

    // Cleanup
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

// Draw the altitude lines and text
void AltitudeLines::draw(Shader &lineShader, Shader &textShader, float rollAngle, float pitchAngle)
{
    // Draw the lines
    lineShader.use();
    lineShader.setUniformVec4("uColor", glm::value_ptr(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f))); // White color

    // Create a VAO and VBO for the lines
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Bind the VAO and VBO
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, lineVertices.size() * sizeof(float), lineVertices.data(), GL_STATIC_DRAW);

    // Define vertex attributes
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // Draw the lines
    glBindVertexArray(VAO);
    glLineWidth(2.0f);
    glDrawArrays(GL_LINES, 0, lineVertices.size() / 2);
    glLineWidth(1.0f);
    glBindVertexArray(0);

    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // Render the text
    glm::vec4 textColor = glm::vec4(1.0f, 1.0f,1.0f, 1.0f); // White color

    // Calculate the range of values (40 to -40)
    float maxValue = 40.0f;
    float step = 10.0f;

    // Render the text on both sides of the lines for each main line
    for (float value = maxValue; value >= -maxValue; value -= step)
    {
        float y = (value / maxValue) * (endY - startY) / 2.0f;
        std::string text = std::to_string(std::abs((int)value));

        // Don't render 0
        if (value == 0)
            continue;

         // Calculate pitch and roll transformations
         glm::mat4 transform = glm::mat4(1.0f);
         float pitchX = pitchAngle * 0.01f * sin(glm::radians(rollAngle)); // X-component of pitch
         float pitchY = pitchAngle * 0.01f * cos(glm::radians(rollAngle)); // Y-component of pitch
 
         // Apply pitch translation
         transform = glm::translate(transform, glm::vec3(-pitchX, pitchY, 0.0f));
 
         // Apply roll rotation
         transform = glm::rotate(transform, glm::radians(rollAngle), glm::vec3(0.0f, 0.0f, 1.0f));
 
         // Render text on both sides
         renderText(textShader, transform, -lineLength / 2.0f - 0.07f, y + 0.02f, text, textScale, glm::vec3(1.0f, 1.0f, 1.0f));
         renderText(textShader, transform, lineLength / 2.0f + 0.02f, y + 0.02f, text, textScale, glm::vec3(1.0f, 1.0f, 1.0f));
    }

    // Reset the transformation matrix to identity
    glm::mat4 identity = glm::mat4(1.0f);
    lineShader.use();
    lineShader.setUniformMat4("uTransform", glm::value_ptr(identity));
}