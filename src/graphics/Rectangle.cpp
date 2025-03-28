#include "Rectangle.h"
#include "../config.h"

// Constructor
Rectangle::Rectangle(float x, float y, float width, float height){
    float vertices[] = {
        x - width / 2, y - height / 2, // Bottom-left
        x + width / 2, y - height / 2, // Bottom-right
        x + width / 2, y + height / 2, // Top-right
        x - width / 2, y + height / 2  // Top-left
    };

    // Generate VAO and VBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Bind VAO and VBO
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Define vertex attributes
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Unbind VBO and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

// Method to draw the rectangle
void Rectangle::draw(){
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glBindVertexArray(0);
}

// Destructor
Rectangle::~Rectangle(){
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}