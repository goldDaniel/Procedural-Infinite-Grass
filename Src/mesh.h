#ifndef MESH_H
#define MESH_H

#include <iostream>
#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <assimp/scene.h>
#include <glad/glad.h>

struct Material
{
    glm::vec4 ambient;
    glm::vec4 diffuse;
    glm::vec4 specular;

    float shininess;
};

class Mesh
{
public:
    GLuint numVertices;
    GLuint numIndices;

    Material material;

    std::vector<GLuint> indices;

    std::vector<GLfloat> positions;
    std::vector<GLfloat> normals;
    std::vector<GLfloat> texCoords;
    std::vector<GLfloat> tangents;

    void createOnGPU();

    GLuint getVertexArray() { return vertexArray; }

private: 
    GLuint vertexArray;
    GLuint indexBuffer;
    GLuint vertexBuffers[4];
};





#endif