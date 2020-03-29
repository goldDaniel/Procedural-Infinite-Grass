#ifndef SHAPE_RENDERER_H
#define SHAPE_RENDERER_H

#include <cassert>
#include <glm/glm.hpp>
#include <string.h> // for memset
#include "glad/glad.h"

class ShapeRenderer
{
private:

    const char* vertexSource = 
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"

    "out vec3 texCoords;\n"

    "uniform mat4 proj;\n"
    "uniform mat4 view;\n"

    "void main()\n"
    "{\n"
        "texCoords = aPos;\n"
        "vec4 pos = proj * view * vec4(aPos, 1.0);\n"
        "gl_Position = pos.xyww;\n"
    "}\n";

    const char* fragmentSource = 
    "#version 330 core\n"
    
    "in vec3 texCoords;\n"

    "uniform samplerCube cubemap;\n"

    "out vec4 FragColor;\n"

    "void main()\n"
    "{\n"             
        "FragColor = texture(cubemap, texCoords);\n"
    "}\n";

    static const int MAX_VERTICES = 100000;
    static const int VALUES_PER_VERTEX = 6;//xyz,rgb

    glm::vec3 currentColor;

    glm::mat4 projection;
    glm::mat4 view;

    float* buffer;
    size_t currentIndex;

    GLuint VAO;
    GLuint VBO;
    GLuint numVertices;


    bool running = false;

public:

    ShapeRenderer()
    {
        buffer = new float[MAX_VERTICES * VALUES_PER_VERTEX];
        memset(buffer, 0, MAX_VERTICES * VALUES_PER_VERTEX);

        currentColor = glm::vec3(1.f);

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(3*sizeof(float)));
        glEnableVertexAttribArray(1);
        
        glBindBuffer(GL_ARRAY_BUFFER, 0); 

        glBindVertexArray(0);

    }

    ~ShapeRenderer()
    {
        delete buffer;
        glDeleteBuffers(1, &VBO);
        glDeleteVertexArrays(1, &VAO);
    }

    void setProjectionMatrix(glm::mat4 proj)
    {
        this->projection = proj;
    }

    void begin(glm::mat4 view)
    {
        assert(!running);
        running = true;

        this->view = view;

        currentIndex = 0;
        memset(buffer, 0, MAX_VERTICES * VALUES_PER_VERTEX);
    }

    void line(float x0, float y0, float z0, float x1, float y1, float z1)
    {
        buffer[currentIndex++] = x0;
        buffer[currentIndex++] = y0;
        buffer[currentIndex++] = z0;
        buffer[currentIndex++] = currentColor.r;
        buffer[currentIndex++] = currentColor.g;
        buffer[currentIndex++] = currentColor.b;

        buffer[currentIndex++] = x1;
        buffer[currentIndex++] = y1;
        buffer[currentIndex++] = z1;
        buffer[currentIndex++] = currentColor.r;
        buffer[currentIndex++] = currentColor.g;
        buffer[currentIndex++] = currentColor.b;
    }

    void end()
    {
        assert(running);
        running = false;

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, MAX_VERTICES * VALUES_PER_VERTEX * sizeof(float), &buffer[0], GL_DYNAMIC_DRAW);

        glDrawArrays(GL_LINES, 0, currentIndex / VALUES_PER_VERTEX);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void setColor(glm::vec3 color)
    {
        this->currentColor = color;
    }
};

#endif