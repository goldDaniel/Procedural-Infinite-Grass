#include "mesh.h"

#include <iostream>

void Mesh::createOnGPU()
{

    glGenVertexArrays(1, &this->vertexArray);
    glBindVertexArray(this->vertexArray);
    glGenBuffers(1, &this->indexBuffer);
    glGenBuffers(4, &vertexBuffers[0]);

    GLuint* indices = &this->indices[0];
    GLfloat* vertices = &this->positions[0];
    GLfloat* normals = &this->normals[0];
    GLfloat* texCoords = &this->texCoords[0];
    GLfloat* tangents = &this->tangents[0];

    //indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->numIndices * sizeof(GLuint), indices, GL_STATIC_DRAW);

    //position
    glBindBuffer(GL_ARRAY_BUFFER, this->vertexBuffers[0]);
    glBufferData(GL_ARRAY_BUFFER, 3 * numVertices * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    //normal
    glBindBuffer(GL_ARRAY_BUFFER, this->vertexBuffers[1]);
    glBufferData(GL_ARRAY_BUFFER, 3 * numVertices * sizeof(GLfloat), normals, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    //texcoord
    glBindBuffer(GL_ARRAY_BUFFER, this->vertexBuffers[2]);
    glBufferData(GL_ARRAY_BUFFER, 2 * numVertices * sizeof(GLfloat), texCoords, GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(2);

    //tangent
    glBindBuffer(GL_ARRAY_BUFFER, this->vertexBuffers[3]);
    glBufferData(GL_ARRAY_BUFFER, 3 * numVertices * sizeof(GLfloat), tangents, GL_STATIC_DRAW);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(3);

    glBindVertexArray(0);
}