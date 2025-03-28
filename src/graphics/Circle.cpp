#include "Circle.h"
#include "../config.h"
#include "Shader.h"


// Constructor
Circle::Circle(float x, float y, float radius, int segments)
    : x(x), y(y), radius(radius), segments(segments)
{
    // Generate vertices for the circle
    std::vector<float> vertices;
    vertices.push_back(x);
    vertices.push_back(y);
    for (int i = 0; i <= segments; i++)
    {
        float angle = 2.0f * M_PI * i / segments;
        vertices.push_back(x + radius * cos(angle));
        vertices.push_back(y + radius * sin(angle));
    }

    // Create VAO and VBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Bind VAO and VBO
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // Define vertex attributes
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // Unbind VBO and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

// Method to draw the circle
void Circle::draw()
{
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 360 + 2);
    glBindVertexArray(0);
}

// To draw the outline of the circle
void Circle::drawOutline()
{
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINE_LOOP, 1, 360 + 1);
    glBindVertexArray(0);
}

// To draw an inward overlay with a specified thickness and color
void Circle::drawInwardOverlay(float thickness, glm::vec4 color, Shader &shader)
{
    std::vector<float> vertices;

    // Generate vertices for the outer edge of the ring (matches the main circle's radius)
    for (int i = 0; i <= segments; i++)
    {
        float angle = 2.0f * M_PI * i / segments;

        // Outer edge
        vertices.push_back(x + radius * cos(angle));
        vertices.push_back(y + radius * sin(angle));

        // Inner edge
        vertices.push_back(x + (radius - thickness) * cos(angle));
        vertices.push_back(y + (radius - thickness) * sin(angle));
    }

    // Create VAO and VBO for the ring
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Bind VAO and VBO
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // Define vertex attributes
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // Draw the ring
    shader.use();
    shader.setUniformVec4("uColor", glm::value_ptr(color)); // Set the overlay color
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, (segments + 1) * 2); // Use GL_TRIANGLE_STRIP
    glBindVertexArray(0);

    // Clean up
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

// To draw overlay lines on the circle
void Circle::drawOverlayLines(float radius, int segments, Shader &shader, float thickness)
{
    std::vector<float> vertices;

    // Generate vertices for the lines
    for (int i = 0; i <= segments / 2; i++)
    {
        if (i == 3)
            continue;
        float angle = 2.0f * M_PI * i / segments;

        // Start point of the line (center of the circle)
        vertices.push_back((radius - thickness) * cos(angle)); // x
        vertices.push_back((radius - thickness) * sin(angle)); // y

        // End point of the line (on the circle's radius)
        vertices.push_back(radius * cos(angle)); // x
        vertices.push_back(radius * sin(angle)); // y
    }

    // Create VAO and VBO for the lines
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Bind VAO and VBO
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // Define vertex attributes
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // Set the color to white
    shader.setUniformVec4("uColor", glm::value_ptr(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));

    // Draw the lines
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, segments * 2); // Each line has 2 vertices
    glBindVertexArray(0);

    // Clean up
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // Draw the white triangle at the top
    float triangleHeight = thickness; // Height of the triangle
    float baseWidth = 0.07f;        // Width of the triangle's base

    std::vector<float> triangleVertices = {
        0.0f, radius - triangleHeight, // Top vertex
        -baseWidth / 2.0f, radius,     // Bottom-left vertex
        baseWidth / 2.0f, radius       // Bottom-right vertex
    };

    // Create VAO and VBO for the triangle
    unsigned int triangleVAO, triangleVBO;
    glGenVertexArrays(1, &triangleVAO);
    glGenBuffers(1, &triangleVBO);

    // Bind VAO and VBO
    glBindVertexArray(triangleVAO);
    glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
    glBufferData(GL_ARRAY_BUFFER, triangleVertices.size() * sizeof(float), triangleVertices.data(), GL_STATIC_DRAW);

    // Define vertex attributes
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // Set the color to yellow for the triangle
    shader.setUniformVec4("uColor", glm::value_ptr(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f))); // White color

    // Draw the triangle
    glBindVertexArray(triangleVAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);

    // Clean up
    glDeleteVertexArrays(1, &triangleVAO);
    glDeleteBuffers(1, &triangleVBO);
}

void Circle::drawTopTriangle(float radius, glm::vec4 color, Shader &shader, float thickness)
{
    float triangleHeight = 0.05f; // Height of the triangle
    float baseWidth = 0.03f;      // Width of the triangle's base

    // Vertices for the yellow triangle
    std::vector<float> yellowVertices = {
        0.0f, radius - thickness - 0.025f,                               // Top vertex (inner side of the overlay)
        -baseWidth / 2.0f, radius - thickness - triangleHeight - 0.025f, // Bottom-left vertex
        baseWidth / 2.0f, radius - thickness - triangleHeight - 0.025f   // Bottom-right vertex
    };

    // Vertices for the black border (slightly larger triangle)
    float borderOffset = 0.007f; // Offset to make the black border slightly larger
    std::vector<float> blackVertices = {
        0.0f, radius - thickness + borderOffset - 0.025f,                                              // Top vertex
        -baseWidth / 2.0f - borderOffset, radius - thickness - triangleHeight - borderOffset - 0.025f, // Bottom-left vertex
        baseWidth / 2.0f + borderOffset, radius - thickness - triangleHeight - borderOffset - 0.025f   // Bottom-right vertex
    };

    // Create VAO and VBO for the black border
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Draw the black border
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, blackVertices.size() * sizeof(float), blackVertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // Set the color to black
    shader.setUniformVec4("uColor", glm::value_ptr(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f))); // Black color
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3); // Draw the black border
    glBindVertexArray(0);

    // Draw the yellow triangle
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, yellowVertices.size() * sizeof(float), yellowVertices.data(), GL_STATIC_DRAW);

    // Set the color to yellow
    shader.setUniformVec4("uColor", glm::value_ptr(color)); // Yellow color
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3); // Draw the yellow triangle
    glBindVertexArray(0);

    // Clean up
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Circle::drawFork(float radius, glm::vec4 color, Shader &shader, float thickness)
{
    // Define the fork dimensions
    float trapeziumHeight = 0.1f;      // Height of the trapezium
    float topWidth = 0.05f;            // Width of the top edge
    float bottomWidth = 0.1f;          // Width of the bottom edge
    float lineLength = 0.3f;           // Length of the vertical line
    float horizontalLineWidth = 0.1f;  // Width of the horizontal line
    float verticalArmHeight = 0.05f;   // Height of the vertical arms
    float lineThickness = 0.015f;      // Thickness of the lines
    float blackLineThickness = 0.015f; // Thickness of the black lines
    float blackLineLength = 0.15f;     // Length of the black lines

    // Vertices for the fork (as a single shape)
    std::vector<float> forkVertices = {
        // Trapezium (2 triangles)
        -topWidth / 2.0f, -radius + thickness + trapeziumHeight, // Top-left
        topWidth / 2.0f, -radius + thickness + trapeziumHeight,  // Top-right
        -bottomWidth / 2.0f, -radius + thickness,                // Bottom-left

        topWidth / 2.0f, -radius + thickness + trapeziumHeight, // Top-right
        -bottomWidth / 2.0f, -radius + thickness,               // Bottom-left
        bottomWidth / 2.0f, -radius + thickness,                // Bottom-right

        // Vertical line (rectangle, 2 triangles)
        -lineThickness / 2.0f, -radius + thickness + trapeziumHeight, // Bottom-left
        lineThickness / 2.0f, -radius + thickness + trapeziumHeight,  // Bottom-right
        -lineThickness / 2.0f, -0.02,                                 // Top-left

        lineThickness / 2.0f, -radius + thickness + trapeziumHeight, // Bottom-right
        -lineThickness / 2.0f, -0.02,                                // Top-left
        lineThickness / 2.0f, -0.02,                                 // Top-right

        // Horizontal line (rectangle, 2 triangles)
        -horizontalLineWidth / 2.0f, -0.05f - 0.01f / 2.0f, // Bottom-left
        horizontalLineWidth / 2.0f, -0.05f - 0.01f / 2.0f,  // Bottom-right
        -horizontalLineWidth / 2.0f, -0.05f + 0.01f / 2.0f, // Top-left

        horizontalLineWidth / 2.0f, -0.05f - 0.01f / 2.0f,  // Bottom-right
        -horizontalLineWidth / 2.0f, -0.05f + 0.01f / 2.0f, // Top-left
        horizontalLineWidth / 2.0f, -0.05f + 0.01f / 2.0f,  // Top-right

        // Left vertical arm (rectangle, 2 triangles)
        -horizontalLineWidth / 2.0f - 0.015f / 2.0f, -0.05f - 0.01f / 2.0f, // Bottom-left
        -horizontalLineWidth / 2.0f + 0.015f / 2.0f, -0.05f - 0.01f / 2.0f, // Bottom-right
        -horizontalLineWidth / 2.0f - 0.015f / 2.0f, -0.05f + 0.025f,       // Top-left

        -horizontalLineWidth / 2.0f + 0.015f / 2.0f, -0.05f - 0.01f / 2.0f, // Bottom-right
        -horizontalLineWidth / 2.0f - 0.015f / 2.0f, -0.05f + 0.025f,       // Top-left
        -horizontalLineWidth / 2.0f + 0.015f / 2.0f, -0.05f + 0.025f,       // Top-right

        // Right vertical arm (rectangle, 2 triangles)
        horizontalLineWidth / 2.0f - 0.015f / 2.0f, -0.05f - 0.01f / 2.0f, // Bottom-left
        horizontalLineWidth / 2.0f + 0.015f / 2.0f, -0.05f - 0.01f / 2.0f, // Bottom-right
        horizontalLineWidth / 2.0f - 0.015f / 2.0f, -0.05f + 0.025f,       // Top-left

        horizontalLineWidth / 2.0f + 0.015f / 2.0f, -0.05f - 0.01f / 2.0f, // Bottom-right
        horizontalLineWidth / 2.0f - 0.015f / 2.0f, -0.05f + 0.025f,       // Top-left
        horizontalLineWidth / 2.0f + 0.015f / 2.0f, -0.05f + 0.025f        // Top-right

    };

    std::vector<float> blackLineVertices = {
        // Horizontal black line (rectangle, 2 triangles)
        -horizontalLineWidth / 2.0f - blackLineThickness / 2.0f - 0.01f, -0.05f - 0.01f / 2.0f - 0.02f / 2.0f, // Bottom-left
        horizontalLineWidth / 2.0f + blackLineThickness / 2.0f + 0.01f, -0.05f - 0.01f / 2.0f - 0.02f / 2.0f,  // Bottom-right
        -horizontalLineWidth / 2.0f - blackLineThickness / 2.0f - 0.01f, -0.05f - 0.01f / 2.0f,                // Top-left

        horizontalLineWidth / 2.0f + blackLineThickness / 2.0f + 0.01f, -0.05f - 0.01f / 2.0f - 0.02f / 2.0f, // Bottom-right
        -horizontalLineWidth / 2.0f - blackLineThickness / 2.0f - 0.01f, -0.05f - 0.01f / 2.0f,               // Top-left
        horizontalLineWidth / 2.0f + blackLineThickness / 2.0f + 0.01f, -0.05f - 0.01f / 2.0f,                // Top-right

        // Left vertical black arm (rectangle, 2 triangles)
        -horizontalLineWidth / 2.0f - blackLineThickness / 2.0f - 0.01f, -0.05f - 0.01f / 2.0f - blackLineThickness / 2.0f, // Bottom-left
        -horizontalLineWidth / 2.0f - 0.005f, -0.05f - 0.01f / 2.0f - blackLineThickness / 2.0f,                            // Bottom-right
        -horizontalLineWidth / 2.0f - blackLineThickness / 2.0f - 0.01f, -0.015f / 2,                                       // Top-left

        -horizontalLineWidth / 2.0f - 0.005f, -0.05f - 0.01f / 2.0f - blackLineThickness / 2.0f, // Bottom-right
        -horizontalLineWidth / 2.0f - blackLineThickness / 2.0f - 0.01f, -0.015f / 2,            // Top-left
        -horizontalLineWidth / 2.0f - 0.005f, -0.015f / 2,                                       // Top-right

        // Right vertical black arm (rectangle, 2 triangles)
        horizontalLineWidth / 2.0f + blackLineThickness / 2.0f + 0.01f, -0.05f - 0.01f / 2.0f - blackLineThickness / 2.0f, // Bottom-left
        horizontalLineWidth / 2.0f + 0.005f, -0.05f - 0.01f / 2.0f - blackLineThickness / 2.0f,                            // Bottom-right
        horizontalLineWidth / 2.0f + blackLineThickness / 2.0f + 0.01f, -0.015f / 2,                                       // Top-left

        horizontalLineWidth / 2.0f + 0.005f, -0.05f - 0.01f / 2.0f - blackLineThickness / 2.0f, // Bottom-right
        horizontalLineWidth / 2.0f + blackLineThickness / 2.0f + 0.01f, -0.015f / 2,            // Top-left
        horizontalLineWidth / 2.0f + 0.005f, -0.015f / 2,                                       // Top-right

        // Left horizontal black arm (rectangle, 2 triangles)
        -horizontalLineWidth / 2.0f - 0.1f, -0.005f - 0.01f,   // Bottom-left
        -horizontalLineWidth / 2.0f - 0.005f, -0.005f - 0.01f, // Bottom-right
        -horizontalLineWidth / 2.0f - 0.1f, -0.005f,           // Top-left

        -horizontalLineWidth / 2.0f - 0.005f, -0.005f - 0.01f, // Bottom-right
        -horizontalLineWidth / 2.0f - 0.1f, -0.005f,           // Top-left
        -horizontalLineWidth / 2.0f - 0.005f, -0.005f,         // Top-right

        // Right horizontal black arm (rectangle, 2 triangles)
        horizontalLineWidth / 2.0f + 0.1f, -0.005f - 0.01f,   // Bottom-left
        horizontalLineWidth / 2.0f + 0.005f, -0.005f - 0.01f, // Bottom-right
        horizontalLineWidth / 2.0f + 0.1f, -0.005f,           // Top-left

        horizontalLineWidth / 2.0f + 0.005f, -0.005f - 0.01f, // Bottom-right
        horizontalLineWidth / 2.0f + 0.1f, -0.005f,           // Top-left
        horizontalLineWidth / 2.0f + 0.005f, -0.005f          // Top-right

    };

    std::vector<float> whiteLineVertices = {
        // White line starting from the top of the left vertical red line
        -horizontalLineWidth / 2.0f - 0.015f / 2.0f, -0.05f - 0.01f / 2.0f + 0.025f, // Bottom-left
        -horizontalLineWidth / 2.0f + 0.015f / 2.0f, -0.05f - 0.01f / 2.0f + 0.025f, // Bottom-right
        -horizontalLineWidth / 2.0f - 0.015f / 2.0f, -0.05f + 0.025f + 0.025f,       // Top-left

        -horizontalLineWidth / 2.0f + 0.015f / 2.0f, -0.05f - 0.01f / 2.0f + 0.025f, // Bottom-right
        -horizontalLineWidth / 2.0f - 0.015f / 2.0f, -0.05f + 0.025f + 0.025f,       // Top-left
        -horizontalLineWidth / 2.0f + 0.015f / 2.0f, -0.05f + 0.025f + 0.025f,       // Top-right

        // White line starting from top of the right vertical red line
        horizontalLineWidth / 2.0f - 0.015f / 2.0f, -0.05f - 0.01f / 2.0f + 0.025f, // Bottom-left
        horizontalLineWidth / 2.0f + 0.015f / 2.0f, -0.05f - 0.01f / 2.0f + 0.025f, // Bottom-right
        horizontalLineWidth / 2.0f - 0.015f / 2.0f, -0.05f + 0.025f + 0.025f,       // Top-left

        horizontalLineWidth / 2.0f + 0.015f / 2.0f, -0.05f - 0.01f / 2.0f + 0.025f, // Bottom-right
        horizontalLineWidth / 2.0f - 0.015f / 2.0f, -0.05f + 0.025f + 0.025f,       // Top-left
        horizontalLineWidth / 2.0f + 0.015f / 2.0f, -0.05f + 0.025f + 0.025f,       // Top-right

        // Left horizontal white arm (rectangle, 2 triangles)
        -horizontalLineWidth / 2.0f - 0.1f, -0.005f,          // Bottom-left
        -horizontalLineWidth / 2.0f + 0.015f / 2.0f, -0.005f, // Bottom-right
        -horizontalLineWidth / 2.0f - 0.1f, 0.005f,           // Top-left

        -horizontalLineWidth / 2.0f + 0.015f / 2.0f, -0.005f, // Bottom-right
        -horizontalLineWidth / 2.0f - 0.1f, 0.005f,           // Top-left
        -horizontalLineWidth / 2.0f + 0.015f / 2.0f, 0.005f,  // Top-right

        // Right horizontal white arm (rectangle, 2 triangles)
        horizontalLineWidth / 2.0f + 0.1f, -0.005f,          // Bottom-left
        horizontalLineWidth / 2.0f - 0.015f / 2.0f, -0.005f, // Bottom-right
        horizontalLineWidth / 2.0f + 0.1f, 0.005f,           // Top-left

        horizontalLineWidth / 2.0f - 0.015f / 2.0f, -0.005f, // Bottom-right
        horizontalLineWidth / 2.0f + 0.1f, 0.005f,           // Top-left
        horizontalLineWidth / 2.0f - 0.015f / 2.0f, 0.005f   // Top-right

    };

    // Create VAO and VBO for the fork
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Bind VAO and VBO
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, forkVertices.size() * sizeof(float), forkVertices.data(), GL_STATIC_DRAW);

    // Define vertex attributes
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // Set the color
    shader.setUniformVec4("uColor", glm::value_ptr(color));

    // Draw the fork
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, forkVertices.size() / 2); // Draw the fork as triangles
    glBindVertexArray(0);

    // Draw the black line
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, blackLineVertices.size() * sizeof(float), blackLineVertices.data(), GL_STATIC_DRAW);

    // Define vertex attributes
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // Set the color for the black line
    shader.setUniformVec4("uColor", glm::value_ptr(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f))); // Black color
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, blackLineVertices.size() / 2); // Draw the black line
    glBindVertexArray(0);

    // Draw the white line
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, whiteLineVertices.size() * sizeof(float), whiteLineVertices.data(), GL_STATIC_DRAW);

    // Define vertex attributes
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // Set the color for the white line
    shader.setUniformVec4("uColor", glm::value_ptr(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f))); // White color
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, whiteLineVertices.size() / 2); // Draw the white line
    glBindVertexArray(0);

    // Clean up
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

// Destructor
Circle::~Circle()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}