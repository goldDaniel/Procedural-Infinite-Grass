#ifndef TERRAIN_CHUNK_H
#define TERRAIN_CHUNK_H

#include <glad/glad.h>

#include "fastnoise/FastNoise.h"

class TerrainChunk
{
private:
    
    static int VALUES_PER_VERTEX;
    
    
    GLuint VAO;
    GLuint positionBuffer;
    GLuint normalBuffer;
    GLuint colorBuffer;

    GLuint numVertices;

    int chunkPosX;
    int chunkPosZ;

    glm::vec3 worldPosMin;
    glm::vec3 worldPosMax;

    bool createdOnGPU = false;
    
    float* positions = nullptr;
    float* normals = nullptr;
    float* colors = nullptr;

    float lerp(float a, float b, float t)
    {
        return a * (1.f - t) + b * t;
    }

    glm::vec3 generateVertexPosition(FastNoise& noise, int x, int z)
    {
        glm::vec3 result;
        result.x = x;
        result.z = z;


        float sample0 = ((noise.GetValue(x * 0.25f, z * 0.25f) + 1.f) / 2.f);
        float sample1 = ((noise.GetValue(x * 2, z * 4) + 1.f) / 2.f);
        float sample2 = ((noise.GetValue(x * 3, z * 3) + 1.f) / 2.f);
        float sample3 = ((noise.GetValue(x * 4, z * 4) + 1.f) / 2.f);
        

        result.y = sample0 * 0.6f  + 
                   sample1 * 0.2f  + 
                   sample2 * 0.15f + 
                   sample3 * 0.05f;

        result.y *= NOISE_HEIGHT_SCALE;

        return result;
    }

    glm::vec3 generateVertexNormal(glm::vec3 A, glm::vec3 B, glm::vec3 C)
    {
        return glm::normalize(cross(B - A, C - A));
    }

    glm::vec3 generateVertexColor(glm::vec3 position)
    {
        return glm::vec3(0.2f, 0.2f + position.y / NOISE_HEIGHT_SCALE, 0.4f);
    }

    void pushToBuffer(float* buffer, int& index, glm::vec3 values)
    {
        buffer[index++] = values.x;
        buffer[index++] = values.y;
        buffer[index++] = values.z;
    }

    void generateChunkTerrain(FastNoise& noise, float* vertices, float* normals, float* colors)
    {
        int vertexIndex = 0;
        int normalIndex = 0;
        int colorIndex = 0;
        for(int i = 0; i < TERRAIN_SIZE; ++i)
        {
            for(int j = 0; j < TERRAIN_SIZE; ++j)
            {
                float x = i + (chunkPosX * TERRAIN_SIZE);
                float z = j + (chunkPosZ * TERRAIN_SIZE);

                
                glm::vec3 posA = generateVertexPosition(noise, x, z);
                glm::vec3 posB = generateVertexPosition(noise, x + 1, z + 1);
                glm::vec3 posC = generateVertexPosition(noise, x + 1, z);

                glm::vec3 colA = generateVertexColor(posA);
                glm::vec3 colB = generateVertexColor(posB);
                glm::vec3 colC = generateVertexColor(posC);
                //we use flat shading, so 1 normal value per triangle primitive
                glm::vec3 normal = generateVertexNormal(posA, posB, posC);

                pushToBuffer(vertices, vertexIndex, posA);
                pushToBuffer(normals, normalIndex, normal);
                pushToBuffer(colors, colorIndex, colA);

                pushToBuffer(vertices, vertexIndex, posB);
                pushToBuffer(normals, normalIndex, normal);
                pushToBuffer(colors, colorIndex, colB);

                pushToBuffer(vertices, vertexIndex, posC);
                pushToBuffer(normals, normalIndex, normal);
                pushToBuffer(colors, colorIndex, colC);


                posA = generateVertexPosition(noise, x, z);
                posB = generateVertexPosition(noise, x, z + 1);
                posC = generateVertexPosition(noise, x + 1, z + 1);

                colA = generateVertexColor(posA);
                colB = generateVertexColor(posB);
                colC = generateVertexColor(posC);
                //we use flat shading, so 1 normal value per triangle primitive
                normal = generateVertexNormal(posA, posB, posC);

                pushToBuffer(vertices, vertexIndex, posA);
                pushToBuffer(normals, normalIndex, normal);
                pushToBuffer(colors, colorIndex, colA);

                pushToBuffer(vertices, vertexIndex, posB);
                pushToBuffer(normals, normalIndex, normal);
                pushToBuffer(colors, colorIndex, colB);

                pushToBuffer(vertices, vertexIndex, posC);
                pushToBuffer(normals, normalIndex, normal);
                pushToBuffer(colors, colorIndex, colC);     
            }
        }
    }

    

public:
    static int NOISE_HEIGHT_SCALE;
    static int TERRAIN_SIZE;
    static int SPACE_BETWEEN_VERTICES;

    TerrainChunk(FastNoise& noise, int chunkPosX, int chunkPosZ)
    {   
        this->chunkPosX = chunkPosX;
        this->chunkPosZ = chunkPosZ;

        this->worldPosMin.x = chunkPosX * TERRAIN_SIZE;
        this->worldPosMin.y = 0;
        this->worldPosMin.z = chunkPosZ * TERRAIN_SIZE;

        this->worldPosMax.x = (chunkPosX + 1) * TERRAIN_SIZE;
        this->worldPosMax.y = NOISE_HEIGHT_SCALE;
        this->worldPosMax.z = (chunkPosZ + 1) * TERRAIN_SIZE;

        numVertices = 6 * TERRAIN_SIZE * TERRAIN_SIZE;

        //this can be quite large so create on heap
        //NOTE: these get deleted in createOnGPI
        positions = new float[VALUES_PER_VERTEX * TERRAIN_SIZE * TERRAIN_SIZE];
        normals  = new float[VALUES_PER_VERTEX * TERRAIN_SIZE * TERRAIN_SIZE];
        colors   = new float[VALUES_PER_VERTEX * TERRAIN_SIZE * TERRAIN_SIZE];

        generateChunkTerrain(noise, positions, normals, colors);
    }

    ~TerrainChunk()
    {
        glDeleteBuffers(1, &positionBuffer);
        glDeleteVertexArrays(1, &VAO);
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

    void createOnGPU()
    {
        if(createdOnGPU) return;

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &positionBuffer);
        glGenBuffers(1, &normalBuffer);
        glGenBuffers(1, &colorBuffer);
       
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
        glBufferData(GL_ARRAY_BUFFER, VALUES_PER_VERTEX * TERRAIN_SIZE * TERRAIN_SIZE * sizeof(float), &positions[0], GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
        glBufferData(GL_ARRAY_BUFFER, VALUES_PER_VERTEX * TERRAIN_SIZE * TERRAIN_SIZE * sizeof(float), &normals[0], GL_STATIC_DRAW);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
        glBufferData(GL_ARRAY_BUFFER, VALUES_PER_VERTEX * TERRAIN_SIZE * TERRAIN_SIZE * sizeof(float), &colors[0], GL_STATIC_DRAW);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, 0); 

        glBindVertexArray(0);

        createdOnGPU = true;

        delete positions;
        delete normals;
        delete colors;
    }
};

int TerrainChunk::TERRAIN_SIZE = 128;
int TerrainChunk::NOISE_HEIGHT_SCALE = 128;

int TerrainChunk::VALUES_PER_VERTEX = 18;

#endif