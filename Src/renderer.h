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

    void draw(glm:: mat4 view)
    {        
        elapsed += 0.01f;

        glm::mat4 proj = glm::perspective(45.f, 1280.f/720.f, 2.f, 2048.f);

        
        for(size_t i = 0; i < modelsToDraw.size(); ++i)
        {
            modelsToDraw[i]->Draw(view, proj, modelMatrices[i]);
        }
        modelsToDraw.clear();
        modelMatrices.clear();
        

        terrainRenderer->draw(view, proj, chunks);
        skyboxRenderer->draw(view);
       
        shapeRenderer->setProjectionMatrix(proj);
        shapeRenderer->begin(view);
        for(TerrainChunk* chunk : chunks)
        {
            shapeRenderer->box(chunk->getWorldMin(), chunk->getWorldMax());
        }
        shapeRenderer->end();
    
    }    
};

#endif