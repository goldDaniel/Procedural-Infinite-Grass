#ifndef TERRAIN_GRASS_RENDERER_H
#define TERRAIN_GRASS_RENDERER_H

#include <glm/gtc/random.hpp>
#include <SDL2/SDL.h>
#include <vector>

#include "glad/glad.h"

#include "fastnoise/FastNoise.h"

#include "camera.h"
#include "shader.h"

#include "shapeRenderer.h"
#include "skyboxRenderer.h"
#include "terrainRenderer.h"
#include "terrainChunk.h"
#include "terrainChunkGenerator.h"

#include "model.h"

#include <queue>

struct Plane
{
    glm::vec3 normal;
    float d;
};

class Renderer
{
private:

    ShapeRenderer* shapeRenderer;
    TerrainRenderer* terrainRenderer;
    SkyboxRenderer* skyboxRenderer;


    std::vector<TerrainChunk*> chunks;
    float elapsed = 0;

    SDL_Window* window;

    std::vector<Model*> modelsToDraw;
    std::vector<glm::mat4> modelMatrices;

public:
    
    Renderer(SDL_Window* w)
    {
        window = w;

        glEnable(GL_DEPTH_TEST);
        
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        glEnable( GL_BLEND );
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        

        shapeRenderer = new ShapeRenderer();
        terrainRenderer = new TerrainRenderer();
        skyboxRenderer = new SkyboxRenderer();

    }

    void queueModel(Model* model, glm::mat4 transform)
    {
        modelsToDraw.push_back(model);
        modelMatrices.push_back(transform);
    }

    void setTerrain(std::vector<TerrainChunk*> chunks)
    {
        this->chunks = chunks;
    }

    ~Renderer()
    {

    }

    void setPointLightPos(glm::vec3 pos)
    {
        terrainRenderer->setPointLightPos(pos);
    }

    void draw(glm:: mat4 view)
    {        
        elapsed += 0.01f;

        glm::mat4 proj = glm::perspective(45.f, 1280.f/720.f, 4.f, 1024.f);

        
        for(size_t i = 0; i < modelsToDraw.size(); ++i)
        {
            modelsToDraw[i]->getShader()->use();
            modelsToDraw[i]->getShader()->setMat4("proj", proj);
            modelsToDraw[i]->getShader()->setMat4("view", view);
            modelsToDraw[i]->getShader()->setMat4("model", modelMatrices[i]);

            modelsToDraw[i]->getShader()->setVec3("dirLight.dir", glm::vec3(0, 0.5f, 1.f));
            modelsToDraw[i]->getShader()->setVec3("dirLight.color", glm::vec3(0.6f));

            for (auto mesh : modelsToDraw[i]->getMeshes()) 
            {
                modelsToDraw[i]->getShader()->setVec4("material.ambient",    mesh->material.ambient);
                modelsToDraw[i]->getShader()->setVec4("material.diffuse",    mesh->material.diffuse);
                modelsToDraw[i]->getShader()->setVec4("material.specular",   mesh->material.specular);    
                modelsToDraw[i]->getShader()->setFloat("material.shininess", mesh->material.shininess);

                glBindVertexArray(mesh->getVertexArray());
                glDrawElements(GL_TRIANGLES, mesh->numIndices, GL_UNSIGNED_INT, 0);
            }

        }
        modelsToDraw.clear();
        modelMatrices.clear();
        
        terrainRenderer->draw(view, proj, chunks);
        skyboxRenderer->draw(view);
    
    }    
};

#endif