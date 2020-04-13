#include "terrainChunk.h"

#include <glad/glad.h>

#include <glm/glm.hpp>

#include "fastnoise/FastNoise.h"

//#define ORIGINAL_TERRAIN_GENERATION

int TerrainChunk::TERRAIN_SIZE = 128;
int TerrainChunk::NOISE_HEIGHT_SCALE = 256;


float TerrainChunk::lerp(float a, float b, float t)
{
    return a * (1.f - t) + b * t;
}

glm::vec3 TerrainChunk::generateVertexPosition(FastNoise& noise, int x, int z)
{
    glm::vec3 result;
    result.x = x;
    result.z = z;


    float sample0 = ((noise.GetValue(x * 0.25f, z * 0.25f) + 1.f) / 2.f);
    float sample1 = ((noise.GetValue(x, z) + 1.f) / 2.f);
    

    result.y = sample0 *  0.8f  + 
               sample1 * 0.2f;

    result.y *= NOISE_HEIGHT_SCALE;

    return result;
}

glm::vec3 TerrainChunk::generateVertexNormal(glm::vec3 A, glm::vec3 B, glm::vec3 C)
{
    return glm::normalize(cross(B - A, C - A));
}

glm::vec3 TerrainChunk::generateVertexColor(glm::vec3 position)
{
    return glm::vec3(0.2f, 0.2f + position.y / NOISE_HEIGHT_SCALE, 0.4f);
}

void TerrainChunk::pushToBuffer(float* buffer, int& index, glm::vec3 values)
{
    buffer[index++] = values.x;
    buffer[index++] = values.y;
    buffer[index++] = values.z;
}

void TerrainChunk::generateChunkTerrain(FastNoise& noise)
{
    int vertexIndex = 0;
    int normalIndex = 0;
    int colorIndex = 0;
    for(int i = -1; i < TERRAIN_SIZE + 1; ++i)
    {
        for(int j = -1; j < TERRAIN_SIZE + 1; ++j)
        {
            float x = i + (chunkPosX * (TERRAIN_SIZE + 1));
            float z = j + (chunkPosZ * (TERRAIN_SIZE + 1));

            glm::vec3 posA = generateVertexPosition(noise, x, z);
            glm::vec3 posB = generateVertexPosition(noise, x + 1, z);
            glm::vec3 posC = generateVertexPosition(noise, x + 1, z + 1);

            glm::vec3 normal = generateVertexNormal(posA, posB, posC);
            glm::vec3 color = generateVertexColor(posA);

            pushToBuffer(positions, vertexIndex, posA);
            pushToBuffer(normals, normalIndex, normal);
            pushToBuffer(colors, colorIndex, color);
        }
    }

    size_t indicesIndex = 0;
    for(int i = 0; i < TERRAIN_SIZE + 1; ++i)
    {
        for(int j = 0; j < TERRAIN_SIZE + 1; ++j)
        {
            indices[indicesIndex++] = i * (TERRAIN_SIZE + 2) + j;			
            indices[indicesIndex++] = i * (TERRAIN_SIZE + 2) + j + 1;
            indices[indicesIndex++] = (i + 1) * (TERRAIN_SIZE + 2) + j + 1;

			indices[indicesIndex++] = i * (TERRAIN_SIZE + 2) + j;
			indices[indicesIndex++] = (i + 1) * (TERRAIN_SIZE + 2) + j + 1;
			indices[indicesIndex++] = (i + 1) * (TERRAIN_SIZE + 2) + j;
        }
    }
}

TerrainChunk::TerrainChunk(FastNoise& noise, int chunkPosX, int chunkPosZ)
{   
    this->chunkPosX = chunkPosX;
    this->chunkPosZ = chunkPosZ;

    this->worldPosMin.x = chunkPosX * TERRAIN_SIZE;
    this->worldPosMin.y = 0;
    this->worldPosMin.z = chunkPosZ * TERRAIN_SIZE;

    this->worldPosMax.x = (chunkPosX + 1) * TERRAIN_SIZE;
    this->worldPosMax.y = NOISE_HEIGHT_SCALE;
    this->worldPosMax.z = (chunkPosZ + 1) * TERRAIN_SIZE;
    

    numVertices = 3 * (TERRAIN_SIZE + 2) * (TERRAIN_SIZE + 2);

    numIndices = 6 * (TERRAIN_SIZE + 1) * (TERRAIN_SIZE + 1);

    //this can be quite large so create on heap
    //NOTE: these get deleted in createOnGPI
    positions = new float[numVertices];
    normals  = new float[numVertices];
    colors   = new float[numVertices];

    indices = new int[numIndices];

    generateChunkTerrain(noise);
}

TerrainChunk::~TerrainChunk()
{
    glDeleteBuffers(1, &positionBuffer);
    glDeleteVertexArrays(1, &VAO);

    delete positions;
    delete normals;
    delete colors;
}


void TerrainChunk::createOnGPU()
{
    if(createdOnGPU) return;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &positionBuffer);
    glGenBuffers(1, &normalBuffer);
    glGenBuffers(1, &colorBuffer);
    glGenBuffers(1, &indexBuffer);
        
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
    glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(GLfloat), &positions[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(GLfloat), &normals[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(GLfloat), &colors[0], GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0);

    createdOnGPU = true;

    
}