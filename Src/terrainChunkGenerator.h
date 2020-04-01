#ifndef TERRAIN_CHUNK_GENERATOR_H
#define TERRAIN_CHUNK_GENERATOR_H


#include <thread>
#include <vector>
#include <future>

#include "fastnoise/FastNoise.h"
#include "terrainChunk.h"


std::vector<TerrainChunk*> generateChunks(int size)
{
    std::vector<TerrainChunk*> result;

    FastNoise noise;

    for(int x = -size; x < size; ++x)
    {
        for(int z = -size; z < size; ++z)
        {
            result.push_back(new TerrainChunk(noise, x, z));
        }
    }

    for(TerrainChunk* chunk : result)
    {
        chunk->createOnGPU();
    }

    return result;
}

#endif