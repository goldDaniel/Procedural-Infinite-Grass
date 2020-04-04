#ifndef TERRAIN_RENDERER_H
#define TERRAIN_RENDERER_H

#include "fastnoise/FastNoise.h"

#include <iostream>

#include "glad/glad.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include "boundingVolume.h"
#include "shader.h"
#include "terrainChunk.h"

class TerrainRenderer
{
private:

    const char* vertexSource = 
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aNor;\n"
    "layout (location = 2) in vec3 aCol;\n"

    "uniform mat4 proj;\n"
    "uniform mat4 view;\n"

    "out vec3 fragPos;\n"
    "out vec3 pos;\n"
    "out vec3 nor;\n"
    "out vec3 color;\n"
    
    

    "void main()\n"
    "{\n"
        "gl_Position = proj * view * vec4(aPos, 1.0);\n"
        "fragPos = vec3(view * vec4(aPos, 1.0));\n"
        "pos = aPos;\n"
        "nor = aNor;\n"
        "color = aCol;\n"  
    "}\n";

    const char* fragmentSource = 
    "#version 330 core\n"
    
    "in vec3 fragPos;\n"
    "in vec3 pos;\n"
    "in vec3 nor;\n"
    "in vec3 color;\n"
    
    "out vec4 FragColor;\n"


    "void main()\n"
    "{\n"           
        "vec3 lightDir = vec3(0, -0.5, 1);\n"
        "lightDir = normalize(lightDir);\n"
        "float d = max(dot(normalize(nor), lightDir), 0.0);\n"
        "FragColor = vec4(0.6*d*color + 0.1*color, 1.f);\n"
    "}\n";

    Shader* terrainShader;


public:

    TerrainRenderer()
    {
        terrainShader = new Shader(vertexSource, fragmentSource);
    }

    ~TerrainRenderer()
    {
        delete terrainShader;
    }

    void draw(glm::mat4 view, glm::mat4 proj, std::vector<TerrainChunk*>& chunks)
    {
        glEnable(GL_DEPTH_TEST);


        Frustum frustum(view, proj);

        terrainShader->use();
        terrainShader->setMat4("proj", proj);
        terrainShader->setMat4("view", view);

        for(TerrainChunk* chunk : chunks)
        {
            BoundingBox box(chunk->getWorldMin(), chunk->getWorldMax());

            if(frustum.testIntersection(box) != BoundingVolume::TEST_OUTSIDE)
            {
                glBindVertexArray(chunk->getVertexArray());
                glDrawElements(GL_TRIANGLES, chunk->getNumIndices(), GL_UNSIGNED_INT, 0);
            }
            
        }
        glBindVertexArray(0);
    }
};


#endif