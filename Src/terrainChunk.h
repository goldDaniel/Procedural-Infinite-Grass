#ifndef TERRAIN_CHUNK_H
#define TERRAIN_CHUNK_H

#include <glad/glad.h>

#include <glm/glm.hpp>

#include "fastnoise/FastNoise.h"

class TerrainChunk
{
private:
    
    static int VALUES_PER_VERTEX;
    
    
    GLuint VAO;
    GLuint positionBuffer;
    GLuint normalBuffer;
    GLuint colorBuffer;

    GLuint indexBuffer;

    GLuint numVertices;

    GLuint numIndices;

    int chunkPosX;
    int chunkPosZ;

    glm::vec3 worldPosMin;
    glm::vec3 worldPosMax;

    bool createdOnGPU = false;
    
    //we want to keep this data on the CPU for bullet physics mesh
    float* positions = nullptr;
    float* normals = nullptr;
    float* colors = nullptr;

    int* indices = nullptr;
    /////////////////////////////////////////////////////////////

    float lerp(float a, float b, float t);

    glm::vec3 generateVertexPosition(FastNoise& noise, int x, int z);
    glm::vec3 generateVertexNormal(glm::vec3 A, glm::vec3 B, glm::vec3 C);
    glm::vec3 generateVertexColor(glm::vec3 position);

    void pushToBuffer(float* buffer, int& index, glm::vec3 values);

    void generateChunkTerrain(FastNoise& noise);

    

public:
    static int NOISE_HEIGHT_SCALE;
    static int TERRAIN_SIZE;
    static int SPACE_BETWEEN_VERTICES;

    TerrainChunk(FastNoise& noise, int chunkPosX, int chunkPosZ);
    ~TerrainChunk();

    void createOnGPU();

    float* getPositionBuffer()
    {
        return positions;
    }

    int getChunkX()
    {
        return chunkPosX;
    }

    int getChunkZ()
    {
        return chunkPosZ;
    }

    glm::vec3 getWorldMin()
    {
        return worldPosMin;
    }

    glm::vec3 getWorldMax()
    {
        return worldPosMax;
    }

    GLuint getVertexArray()
    {
        return VAO;
    }

    GLuint getNumVertices()
    {
        return numVertices;
    }

    GLuint getNumIndices()
    {
        return numIndices;
    }
    
};



#endif