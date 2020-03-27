#ifndef TERRAIN_RENDERER_H
#define TERRAIN_RENDERER_H

#include "fastnoise/FastNoise.h"

#include <iostream>

#include "glad/glad.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include "shader.h"

class TerrainRenderer
{
private:

    const char* vertexSource = 
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"

    "uniform mat4 proj;\n"
    "uniform mat4 view;\n"

    "out vec3 pos;"

    "void main()\n"
    "{\n"
        "gl_Position = proj * view * vec4(aPos, 1.0);\n"
        "pos = aPos;\n"
    "}\n";

    const char* fragmentSource = 
    "#version 330 core\n"

    "in vec3 pos;\n"
    "out vec4 FragColor;\n"

    "void main()\n"
    "{\n"             
        "FragColor = vec4(vec3(0.2, 0.7, 0.3) * mod(pos.yyy, 1.f), 1.f);\n"
    "}\n";

    unsigned int VAO;
    unsigned int positionBuffer;
    unsigned int normalBuffer;

    int numVertices;

    int terrainSize;

    Shader* terrainShader;

    float* vertices;

public:

    TerrainRenderer(int terrainSize) : terrainSize(terrainSize)
    {
        numVertices = 6 * terrainSize * terrainSize;

        vertices = new float[18 * terrainSize * terrainSize];


        FastNoise noise;

        float noiseHeightScale = 32.f;

        int vertexIndex = 0;
        for(int x = 0; x < terrainSize; ++x)
        {
            for(int z = 0; z < terrainSize; ++z)
            {
                vertices[vertexIndex++] = x;
                vertices[vertexIndex++] = (
                                           noise.GetValue(x, z)         + 
                                           noise.GetValue(x * 2, z * 2) +
                                           noise.GetValue(x * 3, z * 3) +
                                           noise.GetValue(x * 4, z * 4) 
                                           ) / 4.f * noiseHeightScale;
                vertices[vertexIndex++] = z;

                vertices[vertexIndex++] = x + 1;
                vertices[vertexIndex++] = (
                                           noise.GetValue((x + 1), z)         + 
                                           noise.GetValue((x + 1) * 2, z * 2) +
                                           noise.GetValue((x + 1) * 3, z * 3) +
                                           noise.GetValue((x + 1) * 4, z * 4) 
                                           ) / 4.f * noiseHeightScale;
                vertices[vertexIndex++] = z;

                vertices[vertexIndex++] = x + 1;
                vertices[vertexIndex++] = (
                                           noise.GetValue((x + 1),     (z + 1))         + 
                                           noise.GetValue((x + 1) * 2, (z + 1) * 2) +
                                           noise.GetValue((x + 1) * 3, (z + 1) * 3) +
                                           noise.GetValue((x + 1) * 4, (z + 1) * 4) 
                                           ) / 4.f * noiseHeightScale;
                vertices[vertexIndex++] = z + 1;

                

                vertices[vertexIndex++] = x;
                vertices[vertexIndex++] = (
                                           noise.GetValue(x, z)         + 
                                           noise.GetValue(x * 2, z * 2) +
                                           noise.GetValue(x * 3, z * 3) +
                                           noise.GetValue(x * 4, z * 4) 
                                           ) / 4.f * noiseHeightScale;
                vertices[vertexIndex++] = z;

                vertices[vertexIndex++] = x + 1;
                vertices[vertexIndex++] = (
                                           noise.GetValue((x + 1),     (z + 1))         + 
                                           noise.GetValue((x + 1) * 2, (z + 1) * 2) +
                                           noise.GetValue((x + 1) * 3, (z + 1) * 3) +
                                           noise.GetValue((x + 1) * 4, (z + 1) * 4) 
                                           ) / 4.f * noiseHeightScale;
                vertices[vertexIndex++] = z + 1;

                vertices[vertexIndex++] = x;
                vertices[vertexIndex++] = (
                                           noise.GetValue(x,     (z + 1))         + 
                                           noise.GetValue(x * 2, (z + 1) * 2) +
                                           noise.GetValue(x * 3, (z + 1) * 3) +
                                           noise.GetValue(x * 4, (z + 1) * 4) 
                                           ) / 4.f * noiseHeightScale;
                vertices[vertexIndex++] = z + 1;
            }
        }


        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &positionBuffer);
       
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
        glBufferData(GL_ARRAY_BUFFER, 18 * terrainSize * terrainSize * sizeof(float), &vertices[0], GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0); 

        terrainShader = new Shader(vertexSource, fragmentSource);

        delete vertices;
    }

    ~TerrainRenderer()
    {

    }

    void draw(glm::mat4 view)
    {
        terrainShader->use();
        glm::mat4 proj = glm::perspective(45.f, 1280.f/720.f, 0.01f, 10000.f);
        terrainShader->setMat4("proj", proj);
        terrainShader->setMat4("view", view);

        
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, numVertices);

        glBindVertexArray(0);
    }
};


#endif