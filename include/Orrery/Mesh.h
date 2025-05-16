#ifndef MESH_H
#define MESH_H

#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Orrery/Common.h"

class Mesh {
    private:
        std::vector<Graphics::Vertex> vertices_;
        unsigned int VBO_, VAO_;
        bool isInitialized_;

    public:
        Mesh();
        ~Mesh();
        void initialize();
        void setVertices(const std::vector<Graphics::Vertex>& vertices);
        void draw();
};

#endif // MESH_H