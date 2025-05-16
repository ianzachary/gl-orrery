#include "Orrery/Mesh.h"

Mesh::Mesh() : isInitialized_(false), VAO_(0), VBO_(0) {}
Mesh::~Mesh() {
    if (isInitialized_) {
        glDeleteVertexArrays(1, &VAO_);
        glDeleteBuffers(1, &VBO_);
    }
}

void Mesh::setVertices(const std::vector<Graphics::Vertex>& vertices) {
    vertices_ = vertices;
}

void Mesh::initialize() {
    if (isInitialized_) {
        return;
    }

    glGenVertexArrays(1, &VAO_);
    glGenBuffers(1, &VBO_);

    glBindVertexArray(VAO_);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Graphics::Vertex), vertices_.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Graphics::Vertex), (void*)offsetof(Graphics::Vertex, position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Graphics::Vertex), (void*)offsetof(Graphics::Vertex, normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Graphics::Vertex), (void*)offsetof(Graphics::Vertex, texCoords));

    isInitialized_ = true;
}

void Mesh::draw() {
    glBindVertexArray(VAO_);
    glDrawArrays(GL_TRIANGLES, 0, vertices_.size());
    glBindVertexArray(0);
}