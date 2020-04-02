#include "terrainChunkGenerator.h"

#include <vector>
#include <future>

static TerrainChunk* generateChunk(FastNoise noise, int x, int z)
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