#ifndef TERRAIN_CHUNK_H
#define TERRAIN_CHUNK_H

#include <glad/glad.h>

#include "fastnoise/FastNoise.h"

class TerrainChunk
{
private:

    

    GLuint VAO;
    GLuint positionBuffer;
    GLuint normalBuffer;
    GLuint colorBuffer;

    GLuint numVertices;

    int chunkPosX;
    int chunkPosZ;

    glm::vec3 generateVertexPosition(FastNoise& noise, int x, int z)
    {
        glm::vec3 result;
        result.x = x;
        result.z = z;

        //we blend several weighted noise values together here
        result.y = NOISE_HEIGHT_SCALE * 
                    (    
                        (
                            noise.GetValue(x * 1.f, z * 1.f) +
                            noise.GetValue(x * 2.f, z * 2.f) +
                            noise.GetValue(x * 3.f, z * 3.f) +
                            noise.GetValue(x * 4.f, z * 4.f)
                        ) / 4.f + 1 / 2.f
                    );


        return result;
    }

    glm::vec3 generateVertexNormal(glm::vec3 A, glm::vec3 B, glm::vec3 C)
    {
        return glm::normalize(cross(B - A, C - A));
    }

    glm::vec3 generateVertexColor(glm::vec3 position)
    {
        glm::vec3 result;

        if(position.y < NOISE_HEIGHT_SCALE / 8.f)
        {
            result.r = 0.1f;
            result.g = 0.2f;
            result.b = 0.3f;
        }
        else if(position.y < NOISE_HEIGHT_SCALE / 2)
        {
            result.r = 0.2f * position.y / NOISE_HEIGHT_SCALE / 2.f;
            result.g = 0.7f * position.y / NOISE_HEIGHT_SCALE / 2.f;
            result.b = 0.3f * position.y / NOISE_HEIGHT_SCALE / 2.f;
        }
        else
        {
            result.r = 0.6f * position.y / NOISE_HEIGHT_SCALE;
            result.g = 0.8f * position.y / NOISE_HEIGHT_SCALE;
            result.b = 0.4f * position.y / NOISE_HEIGHT_SCALE;
        }


        return result;
    }

    void pushToBuffer(float* buffer, int& index, glm::vec3 values)
    {
        buffer[index++] = values.x;
        buffer[index++] = values.y;
        buffer[index++] = values.z;
    }

public:
    static int NOISE_HEIGHT_SCALE;
    static int TERRAIN_SIZE;

    TerrainChunk(FastNoise& noise, int chunkPosX, int chunkPosZ)
    {   
        this->chunkPosX = chunkPosX;
        this->chunkPosZ = chunkPosZ;

        numVertices = 6 * TERRAIN_SIZE * TERRAIN_SIZE;

        //this can be quite large so create on heap
        float* vertices = new float[18 * TERRAIN_SIZE * TERRAIN_SIZE];
        float* normals  = new float[18 * TERRAIN_SIZE * TERRAIN_SIZE];
        float* colors   = new float[18 * TERRAIN_SIZE * TERRAIN_SIZE];

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


        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &positionBuffer);
        glGenBuffers(1, &normalBuffer);
        glGenBuffers(1, &colorBuffer);
       
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
        glBufferData(GL_ARRAY_BUFFER, 18 * TERRAIN_SIZE * TERRAIN_SIZE * sizeof(float), &vertices[0], GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
        glBufferData(GL_ARRAY_BUFFER, 18 * TERRAIN_SIZE * TERRAIN_SIZE * sizeof(float), &normals[0], GL_STATIC_DRAW);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
        glBufferData(GL_ARRAY_BUFFER, 18 * TERRAIN_SIZE * TERRAIN_SIZE * sizeof(float), &colors[0], GL_STATIC_DRAW);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, 0); 

        glBindVertexArray(0);

        

        delete vertices;
        delete normals;
        delete colors;
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

    GLuint getVertexArray()
    {
        return VAO;
    }

    GLuint getNumVertices()
    {
        return numVertices;
    }
};

int TerrainChunk::TERRAIN_SIZE = 128;
int TerrainChunk::NOISE_HEIGHT_SCALE = 128;

#endif