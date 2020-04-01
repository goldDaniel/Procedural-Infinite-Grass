#ifndef TERRAIN_GRASS_RENDERER_H
#define TERRAIN_GRASS_RENDERER_H

#include <glm/gtc/random.hpp>

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

#include "boundingVolume.h"

#include <vector>

namespace Renderer
{   

    struct Plane
    {
        glm::vec3 normal;
        float d;
    };

    ShapeRenderer* shapeRenderer;
    TerrainRenderer* terrainRenderer;
    SkyboxRenderer* skyboxRenderer;

    std::vector<TerrainChunk*> chunks;

    const int terrainSize = 64;

    float elapsed = 0;

    SDL_Window* window;

    void Dispose()
    {
    
    }

    void Init(SDL_Window* w)
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

        
        chunks = generateChunks(8);
    }

    void Draw(glm:: mat4 view)
    {
        int w,h;
        SDL_GetWindowSize(window, &w, &h);
        glViewport(0, 0, w, h);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        elapsed += 0.01f;

        glm::mat4 proj = glm::infinitePerspective(45.f, 1280.f/720.f, 0.01f);

        
        Frustum frustum(view, proj);

        std::vector<TerrainChunk*> chunksToRender;

        shapeRenderer->setProjectionMatrix(proj);

        shapeRenderer->begin(view);
        for(TerrainChunk* chunk : chunks)
        {
            BoundingBox box(chunk->getWorldMin(), chunk->getWorldMax());

            if(frustum.testIntersection(box) != BoundingVolume::TEST_OUTSIDE)
            {
                chunksToRender.push_back(chunk);
            }

            shapeRenderer->box(chunk->getWorldMin(), chunk->getWorldMax());
        }
        shapeRenderer->end();


        skyboxRenderer->draw(view);
        terrainRenderer->draw(view, chunksToRender);
        
    }    
}
#endif