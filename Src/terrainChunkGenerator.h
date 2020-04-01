#ifndef TERRAIN_CHUNK_GENERATOR_H
#define TERRAIN_CHUNK_GENERATOR_H


#include <thread>
#include <vector>
#include <future>

#include "fastnoise/FastNoise.h"
#include "terrainChunk.h"

TerrainChunk* generateChunk(FastNoise noise, int x, int z)
{
    return new TerrainChunk(noise, x, z);
}

std::vector<TerrainChunk*> generateChunks(int size)
{
    std::vector<TerrainChunk*> result;

    FastNoise noise;

    std::vector<std::future<TerrainChunk*>> futures;

    for(int x = -size; x < size; ++x)
    {
        for(int z = -size; z < size; ++z)
        {
            futures.push_back(std::async(generateChunk, noise, x, z));
        }
    }

    for(size_t i = 0; i < futures.size(); ++i)
    {
        result.push_back(futures[i].get());
    }

    for(TerrainChunk* chunk : result)
    {
        chunk->createOnGPU();
    }

    return result;
}

#endif