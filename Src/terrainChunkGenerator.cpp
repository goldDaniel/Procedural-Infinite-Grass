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


#define MULTITHREAD_PATH
#ifdef MULTITHREAD_PATH

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

    
    // Get ending timepoint 
    auto stop = std::chrono::high_resolution_clock::now(); 
  
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start); 

    std::cout << "GEN TIME: " << duration.count() << std::endl;

    return result;
}