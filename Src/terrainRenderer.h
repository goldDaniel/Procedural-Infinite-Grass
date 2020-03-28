#ifndef TERRAIN_RENDERER_H
#define TERRAIN_RENDERER_H

#include "fastnoise/FastNoise.h"

#include <iostream>

#include "glad/glad.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include "shader.h"
#include "terrainChunk.h"

class TerrainRenderer
{
private:

    const char* vertexSource = 
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aCol;\n"

    "uniform mat4 proj;\n"
    "uniform mat4 view;\n"

    "out vec3 color;\n"
    "out vec3 pos;\n"

    "void main()\n"
    "{\n"
        "gl_Position = proj * view * vec4(aPos, 1.0);\n"
        "pos = aPos;\n"
        "color = aCol;\n"  
    "}\n";

    const char* fragmentSource = 
    "#version 330 core\n"

    "in vec3 color;\n"
    "in vec3 pos;\n"
    "out vec4 FragColor;\n"

    "void main()\n"
    "{\n"           
        "FragColor = vec4(color, 1.f);\n"
    "}\n";

    Shader* terrainShader;


public:

    TerrainRenderer()
    {
        terrainShader = new Shader(vertexSource, fragmentSource);
    }

    ~TerrainRenderer()
    {

    }

    void draw(glm::mat4 view, std::vector<TerrainChunk*>& chunks)
    {
        terrainShader->use();
        glm::mat4 proj = glm::perspective(45.f, 1280.f/720.f, 0.01f, 10000.f);
        terrainShader->setMat4("proj", proj);
        terrainShader->setMat4("view", view);

        for(TerrainChunk* chunk : chunks)
        {
            glBindVertexArray(chunk->getVertexArray());
            glDrawArrays(GL_TRIANGLES, 0, chunk->getNumVertices());    
        }
        

        glBindVertexArray(0);
    }
};


#endif