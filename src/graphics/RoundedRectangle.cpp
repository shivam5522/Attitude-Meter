#include "RoundedRectangle.h"
#include "../config.h"
#include "Rectangle.h"
#include "QuarterCircle.h"
#include "Shader.h"


// Constructor
RoundedRectangle::RoundedRectangle(float x, float y, float width, float height, float radius, int segments)
    : x(x), y(y), width(width), height(height), radius(radius), segments(segments)
    {}
    void RoundedRectangle::draw(Shader &shader) {
      std::vector<float> vertices;
  
      // Generate vertices for the center rectangle
      vertices.insert(vertices.end(), {
          x + radius, y + radius,                     // Bottom-left
          x + width - radius, y + radius,             // Bottom-right
          x + width - radius, y + height - radius,    // Top-right
          x + radius, y + height - radius             // Top-left
      });
  
      // Generate vertices for the 4 quarter circles
      for (int i = 0; i < 4; i++) {
          // Determine the center of the quarter circle
          float cx = (i == 0 || i == 3) ? x + radius : x + width - radius;
          float cy = (i == 0 || i == 1) ? y + radius : y + height - radius;
  
          // Calculate the starting angle for the quarter circle
          float startAngle = (i * 90.0f) * (M_PI / 180.0f);
  
          // Add the center of the quarter circle
          vertices.push_back(cx);
          vertices.push_back(cy);
  
          // Add the points along the arc
          for (int j = 0; j <= segments; j++) {
              float angle = startAngle + j * ((M_PI / 2.0f) / segments); // Increment angle
              float px = cx + cos(angle) * radius; // X-coordinate of the point on the arc
              float py = cy + sin(angle) * radius; // Y-coordinate of the point on the arc
              vertices.push_back(px);
              vertices.push_back(py);
          }
      }
  
      // Create VAO and VBO
      unsigned int VAO, VBO;
      glGenVertexArrays(1, &VAO);
      glGenBuffers(1, &VBO);
  
      // Bind VAO and VBO
      glBindVertexArray(VAO);
      glBindBuffer(GL_ARRAY_BUFFER, VBO);
      glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
  
      // Define vertex attributes
      glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
      glEnableVertexAttribArray(0);
  
      // Use the shader
      shader.use();
      shader.setUniformVec4("uColor", glm::value_ptr(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f))); // Set color
  
      // Draw the center rectangle as a triangle fan
      glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
  
      // Draw the 4 quarter circles
      int offset = 4; // Offset after the rectangle vertices
      for (int i = 0; i < 4; i++) {
          glDrawArrays(GL_TRIANGLE_FAN, offset, segments + 2); // +2 for center + arc points
          offset += segments + 2;
      }
  
      // Unbind VAO and VBO
      glBindVertexArray(0);
      glDeleteVertexArrays(1, &VAO);
      glDeleteBuffers(1, &VBO);
  }
