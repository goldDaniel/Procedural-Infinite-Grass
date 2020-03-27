#ifndef TERRAIN_CHUNK_H
#define TERRAIN_CHUNK_H

#include <glad/glad.h>

#include "fastnoise/FastNoise.h"

class TerrainChunk
{
private:
    static int TERRAIN_SIZE;

    GLuint VAO;
    GLuint positionBuffer;
    GLuint normalBuffer;

    GLuint numVertices;

public:
    TerrainChunk(FastNoise& noise, int chunkPosX, int chunkPosZ)
    {
        numVertices = 6 * TERRAIN_SIZE * TERRAIN_SIZE;

        //this can be quite large so create on heap
        float* vertices = new float[18 * TERRAIN_SIZE * TERRAIN_SIZE];


        float noiseHeightScale = 32.f;

        int vertexIndex = 0;
        for(int i = 0; i < TERRAIN_SIZE; ++i)
        {
            for(int j = 0; j < TERRAIN_SIZE; ++j)
            {

                float x = i + (chunkPosX * TERRAIN_SIZE);
                float z = j + (chunkPosZ * TERRAIN_SIZE);


                vertices[vertexIndex++] = x;
                vertices[vertexIndex++] = (
                                           noise.GetValue(x, z)         + 
                                           noise.GetValue(x * 2, z * 2) +
                                           noise.GetValue(x * 3, z * 3) +
                                           noise.GetValue(x * 4, z * 4) 
                                           ) / 4.f * noiseHeightScale;
                vertices[vertexIndex++] = z;

                vertices[vertexIndex++] = x + 1;
                vertices[vertexIndex++] = (
                                           noise.GetValue((x + 1), z)         + 
                                           noise.GetValue((x + 1) * 2, z * 2) +
                                           noise.GetValue((x + 1) * 3, z * 3) +
                                           noise.GetValue((x + 1) * 4, z * 4) 
                                           ) / 4.f * noiseHeightScale;
                vertices[vertexIndex++] = z;

                vertices[vertexIndex++] = x + 1;
                vertices[vertexIndex++] = (
                                           noise.GetValue((x + 1),     (z + 1))         + 
                                           noise.GetValue((x + 1) * 2, (z + 1) * 2) +
                                           noise.GetValue((x + 1) * 3, (z + 1) * 3) +
                                           noise.GetValue((x + 1) * 4, (z + 1) * 4) 
                                           ) / 4.f * noiseHeightScale;
                vertices[vertexIndex++] = z + 1;

                

                vertices[vertexIndex++] = x;
                vertices[vertexIndex++] = (
                                           noise.GetValue(x, z)         + 
                                           noise.GetValue(x * 2, z * 2) +
                                           noise.GetValue(x * 3, z * 3) +
                                           noise.GetValue(x * 4, z * 4) 
                                           ) / 4.f * noiseHeightScale;
                vertices[vertexIndex++] = z;

                vertices[vertexIndex++] = x + 1;
                vertices[vertexIndex++] = (
                                           noise.GetValue((x + 1),     (z + 1))         + 
                                           noise.GetValue((x + 1) * 2, (z + 1) * 2) +
                                           noise.GetValue((x + 1) * 3, (z + 1) * 3) +
                                           noise.GetValue((x + 1) * 4, (z + 1) * 4) 
                                           ) / 4.f * noiseHeightScale;
                vertices[vertexIndex++] = z + 1;

                vertices[vertexIndex++] = x;
                vertices[vertexIndex++] = (
                                           noise.GetValue(x,     (z + 1))         + 
                                           noise.GetValue(x * 2, (z + 1) * 2) +
                                           noise.GetValue(x * 3, (z + 1) * 3) +
                                           noise.GetValue(x * 4, (z + 1) * 4) 
                                           ) / 4.f * noiseHeightScale;
                vertices[vertexIndex++] = z + 1;
            }
        }


        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &positionBuffer);
       
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
        glBufferData(GL_ARRAY_BUFFER, 18 * TERRAIN_SIZE * TERRAIN_SIZE * sizeof(float), &vertices[0], GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0); 

        

        delete vertices;
    }

    ~TerrainChunk()
    {
        glDeleteBuffers(1, &positionBuffer);
        glDeleteVertexArrays(1, &VAO);
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

int TerrainChunk::TERRAIN_SIZE = 256;

#endif