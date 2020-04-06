#include "terrainChunkGenerator.h"

#include <iostream>
#include <vector>
#include <future>

#include <chrono>


static TerrainChunk* generateChunk(FastNoise noise, int x, int z)
{
    return new TerrainChunk(noise, x, z);
}

std::vector<TerrainChunk*> generateChunks(int size)
{
    // Get starting timepoint 
    auto start = std::chrono::high_resolution_clock::now(); 

    std::vector<TerrainChunk*> result;

    FastNoise noise;
    noise.SetSeed(start.time_since_epoch().count());

#define GENERATION_MULTITHREAD_PATH
#ifdef GENERATION_MULTITHREAD_PATH

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

#else

    for(int x = -size; x < size; ++x)
    {
        for(int z = -size; z < size; ++z)
        {
            result.push_back(generateChunk( noise, x, z));
        }
    }

    for(TerrainChunk* chunk : result)
    {
        chunk->createOnGPU();
    }

#endif

    
    

    return result;
}