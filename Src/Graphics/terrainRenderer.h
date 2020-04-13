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
    Shader* terrainShader;

    glm::vec3 pointLightPos;

public:

    TerrainRenderer()
    {
        terrainShader = new Shader("Assets/Shaders/terrain.vert", 
                                   "Assets/Shaders/terrain.frag");
        
    }

    ~TerrainRenderer()
    {
        delete terrainShader;
    }

    void setPointLightPos(glm::vec3 pos)
    {
        pointLightPos = pos;
    }

    void draw(glm::mat4 view, glm::mat4 proj, std::vector<TerrainChunk*>& chunks)
    {
        glEnable(GL_DEPTH_TEST);


        Frustum frustum(view, proj);

        terrainShader->use();
        terrainShader->setMat4("proj", proj);
        terrainShader->setMat4("view", view);

        terrainShader->setVec3("dirLight.dir", glm::normalize(glm::vec3(0.f, 0.5f, 0.f)));
        terrainShader->setVec3("dirLight.ambient", glm::vec3(0.05f));
        terrainShader->setVec3("dirLight.diffuse", glm::vec3(0.1f));
        terrainShader->setVec3("dirLight.specular", glm::vec3(0.1f));	

        terrainShader->setVec3("pointLight.pos", pointLightPos);
        terrainShader->setVec3("pointLight.ambient", glm::vec3(0.8f));
        terrainShader->setVec3("pointLight.diffuse", glm::vec3(0.9f));
        terrainShader->setVec3("pointLight.specular", glm::vec3(0.3f));	
        terrainShader->setFloat("pointLight.constant", 1.f);
        terrainShader->setFloat("pointLight.linear",   0.027f);
        terrainShader->setFloat("pointLight.quadratic", 0.0028f);

        glm::vec3 pos;
        pos.x = view[3][0];
        pos.y = view[3][1];
        pos.z = view[3][2];
        terrainShader->setVec3("camPos", pos);

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