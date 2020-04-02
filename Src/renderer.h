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

    void setTerrain(std::vector<TerrainChunk*> chunks)
    {
        this->chunks = chunks;
    }

    ~Renderer()
    {

    }

    void draw(glm:: mat4 view)
    {
        int w,h;
        SDL_GetWindowSize(window, &w, &h);
        glViewport(0, 0, w, h);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        elapsed += 0.01f;

        glm::mat4 proj = glm::perspective(45.f, 1280.f/720.f, 1.f, 1024.f);

        shapeRenderer->setProjectionMatrix(proj);

        shapeRenderer->begin(view);
        for(TerrainChunk* chunk : chunks)
        {
            shapeRenderer->box(chunk->getWorldMin(), chunk->getWorldMax());
        }
        shapeRenderer->end();


        skyboxRenderer->draw(view);
        terrainRenderer->draw(view, proj, chunks);
    }    
};

#endif