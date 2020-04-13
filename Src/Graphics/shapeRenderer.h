#ifndef SHAPE_RENDERER_H
#define SHAPE_RENDERER_H

#include <cassert>
#include <glm/glm.hpp>
#include <string.h> // for memset
#include "glad/glad.h"

#include "shader.h"

class ShapeRenderer
{
private:

    size_t MAX_VERTICES;
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

    Shader* shader;
    
    void flush()
    {
        if(currentIndex == 0) return;

        assert(running);
        assert(currentIndex % 6 == 0);
        assert(currentIndex <= MAX_VERTICES * VALUES_PER_VERTEX);

        glEnable(GL_DEPTH_TEST);

        shader->use();
        shader->setMat4("proj", projection);
        shader->setMat4("view", view);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, MAX_VERTICES * VALUES_PER_VERTEX * sizeof(float), &buffer[0], GL_DYNAMIC_DRAW);

        glDrawArrays(GL_LINES, 0, currentIndex / VALUES_PER_VERTEX);

        
        memset(buffer, 0, MAX_VERTICES * VALUES_PER_VERTEX);
        currentIndex = 0;
    }

public:

    ShapeRenderer() : ShapeRenderer(10000)
    {

    }
    
    ShapeRenderer(int maxVertices)
    {
        MAX_VERTICES = maxVertices;

        buffer = new float[MAX_VERTICES * VALUES_PER_VERTEX];
        memset(buffer, 0, MAX_VERTICES * VALUES_PER_VERTEX);

        currentColor = glm::vec3(1.f);

        shader = new Shader("Assets/Shaders/shapeRenderer.vert", 
                            "Assets/Shaders/shapeRenderer.frag");

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
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
        assert(!running);
        
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

    void box(glm::vec3 min, glm::vec3 max)
    {
        assert(running);

        //LINES ALONG Z////////////////////////////////////////
        line(min.x, min.y, min.z, 
             min.x, min.y, max.z);

        line(max.x, min.y, min.z, 
             max.x, min.y, max.z);

        line(min.x, max.y, min.z, 
             min.x, max.y, max.z);

        line(max.x, max.y, min.z, 
             max.x, max.y, max.z);

        //LINES ALONG X//////////////////////////////////////////
        line(min.x, min.y, min.z,
             max.x, min.y, min.z);

        line(min.x, min.y, max.z,
             max.x, min.y, max.z);

        line(min.x, max.y, min.z,
             max.x, max.y, min.z);

        line(min.x, max.y, max.z,
             max.x, max.y, max.z);

        //LINES ALONG Y//////////////////////////////////////////
        line(min.x, min.y, min.z,
             min.x, max.y, min.z);

        line(max.x, min.y, min.z,
             max.x, max.y, min.z);

        line(min.x, min.y, max.z,
             min.x, max.y, max.z);

        line(max.x, min.y, max.z,
             max.x, max.y, max.z);

            
        
    }

    void line(float x0, float y0, float z0, float x1, float y1, float z1)
    {
        assert(running);

        if(currentIndex >= MAX_VERTICES * VALUES_PER_VERTEX)
        {
            assert(currentIndex == MAX_VERTICES * VALUES_PER_VERTEX);
            flush();
        }

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
        
        flush();

        running = false;
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void setColor(glm::vec3 color)
    {
        this->currentColor = color;
    }
};

#endif