#include "QuarterCircle.h"
#include "../config.h"

QuarterCircle::QuarterCircle(float x, float y, float radius, int segs, float startAngle, float endAngle){
    segments = segs;
    std::vector<float> vertices;
    vertices.push_back(x);
    vertices.push_back(y);

    for(int i = 0; i <= segments; i++){
        float angle = startAngle + (endAngle - startAngle) * (float)i / segments;
        vertices.push_back(x + radius * cos(angle));
        vertices.push_back(y + radius * sin(angle));
    }
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void QuarterCircle::draw(){
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, segments + 2);
    glBindVertexArray(0);
}

QuarterCircle::~QuarterCircle(){
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}