#ifndef TERRAIN_CHUNK_H
#define TERRAIN_CHUNK_H

#include <glad/glad.h>

#include "fastnoise/FastNoise.h"

class TerrainChunk
{
private:

    static int NOISE_HEIGHT_SCALE;
    static int TERRAIN_SIZE;

    GLuint VAO;
    GLuint positionBuffer;
    GLuint colorBuffer;

    GLuint numVertices;

    glm::vec3 generateVertexPosition(FastNoise& noise, int x, int z)
    {
        glm::vec3 result;
        result.x = x;
        result.z = z;

        result.y = (    (
                            noise.GetValue(x, z)         + 
                            noise.GetValue(x * 2, z * 2) +
                            noise.GetValue(x * 4, z * 4)
                        ) / 3.f + 1 / 2.f
                    ) * NOISE_HEIGHT_SCALE;


        return result;
    }

public:
    TerrainChunk(FastNoise& noise, int chunkPosX, int chunkPosZ)
    {
        numVertices = 6 * TERRAIN_SIZE * TERRAIN_SIZE;

        //this can be quite large so create on heap
        float* vertices = new float[18 * TERRAIN_SIZE * TERRAIN_SIZE];
        float* colors   = new float[18 * TERRAIN_SIZE * TERRAIN_SIZE];

        int vertexIndex = 0;
        int colorIndex = 0;
        for(int i = 0; i < TERRAIN_SIZE; ++i)
        {
            for(int j = 0; j < TERRAIN_SIZE; ++j)
            {

                float x = i + (chunkPosX * TERRAIN_SIZE);
                float z = j + (chunkPosZ * TERRAIN_SIZE);

                glm::vec3 vertexPosition = generateVertexPosition(noise, x, z);
                vertices[vertexIndex++] = vertexPosition.x;
                vertices[vertexIndex++] = vertexPosition.y;
                vertices[vertexIndex++] = vertexPosition.z;
                if(vertexPosition.y < 8)
                {
                    colors[colorIndex++] = 0.1f * vertexPosition.y / 8.f;
                    colors[colorIndex++] = 0.4f * vertexPosition.y / 8.f;
                    colors[colorIndex++] = 0.8f * vertexPosition.y / 8.f;
                }
                else if(vertexPosition.y < 20)
                {
                    colors[colorIndex++] = 0.2f * vertexPosition.y / 20.f;
                    colors[colorIndex++] = 0.7f * vertexPosition.y / 20.f;
                    colors[colorIndex++] = 0.3f * vertexPosition.y / 20.f;
                }
                else
                {
                    colors[colorIndex++] = 0.6f * vertexPosition.y / 32.f;
                    colors[colorIndex++] = 0.8f * vertexPosition.y / 32.f;;
                    colors[colorIndex++] = 0.4f * vertexPosition.y / 32.f;;
                }

                


                vertexPosition = generateVertexPosition(noise, x + 1, z);
                vertices[vertexIndex++] = vertexPosition.x;
                vertices[vertexIndex++] = vertexPosition.y;
                vertices[vertexIndex++] = vertexPosition.z;
                if(vertexPosition.y < 8)
                {
                    colors[colorIndex++] = 0.1f * vertexPosition.y / 8.f;
                    colors[colorIndex++] = 0.4f * vertexPosition.y / 8.f;
                    colors[colorIndex++] = 0.8f * vertexPosition.y / 8.f;
                }
                else if(vertexPosition.y < 20)
                {
                    colors[colorIndex++] = 0.2f * vertexPosition.y / 20.f;
                    colors[colorIndex++] = 0.7f * vertexPosition.y / 20.f;
                    colors[colorIndex++] = 0.3f * vertexPosition.y / 20.f;
                }
                else
                {
                    colors[colorIndex++] = 0.6f * vertexPosition.y / 32.f;
                    colors[colorIndex++] = 0.8f * vertexPosition.y / 32.f;;
                    colors[colorIndex++] = 0.4f * vertexPosition.y / 32.f;;
                }

                vertexPosition = generateVertexPosition(noise, x + 1, z + 1);
                vertices[vertexIndex++] = vertexPosition.x;
                vertices[vertexIndex++] = vertexPosition.y;
                vertices[vertexIndex++] = vertexPosition.z;
                if(vertexPosition.y < 8)
                {
                    colors[colorIndex++] = 0.1f * vertexPosition.y / 8.f;
                    colors[colorIndex++] = 0.4f * vertexPosition.y / 8.f;
                    colors[colorIndex++] = 0.8f * vertexPosition.y / 8.f;
                }
                else if(vertexPosition.y < 20)
                {
                    colors[colorIndex++] = 0.2f * vertexPosition.y / 20.f;
                    colors[colorIndex++] = 0.7f * vertexPosition.y / 20.f;
                    colors[colorIndex++] = 0.3f * vertexPosition.y / 20.f;
                }
                else
                {
                    colors[colorIndex++] = 0.6f * vertexPosition.y / 32.f;
                    colors[colorIndex++] = 0.8f * vertexPosition.y / 32.f;;
                    colors[colorIndex++] = 0.4f * vertexPosition.y / 32.f;;
                }


                vertexPosition = generateVertexPosition(noise, x, z);
                vertices[vertexIndex++] = vertexPosition.x;
                vertices[vertexIndex++] = vertexPosition.y;
                vertices[vertexIndex++] = vertexPosition.z;
                if(vertexPosition.y < 8)
                {
                    colors[colorIndex++] = 0.1f * vertexPosition.y / 8.f;
                    colors[colorIndex++] = 0.4f * vertexPosition.y / 8.f;
                    colors[colorIndex++] = 0.8f * vertexPosition.y / 8.f;
                }
                else if(vertexPosition.y < 20)
                {
                    colors[colorIndex++] = 0.2f * vertexPosition.y / 20.f;
                    colors[colorIndex++] = 0.7f * vertexPosition.y / 20.f;
                    colors[colorIndex++] = 0.3f * vertexPosition.y / 20.f;
                }
                else
                {
                    colors[colorIndex++] = 0.6f * vertexPosition.y / 32.f;
                    colors[colorIndex++] = 0.8f * vertexPosition.y / 32.f;;
                    colors[colorIndex++] = 0.4f * vertexPosition.y / 32.f;;
                }

                vertexPosition = generateVertexPosition(noise, x + 1, z + 1);
                vertices[vertexIndex++] = vertexPosition.x;
                vertices[vertexIndex++] = vertexPosition.y;
                vertices[vertexIndex++] = vertexPosition.z;
                if(vertexPosition.y < 8)
                {
                    colors[colorIndex++] = 0.1f * vertexPosition.y / 8.f;
                    colors[colorIndex++] = 0.4f * vertexPosition.y / 8.f;
                    colors[colorIndex++] = 0.8f * vertexPosition.y / 8.f;
                }
                else if(vertexPosition.y < 20)
                {
                    colors[colorIndex++] = 0.2f * vertexPosition.y / 20.f;
                    colors[colorIndex++] = 0.7f * vertexPosition.y / 20.f;
                    colors[colorIndex++] = 0.3f * vertexPosition.y / 20.f;
                }
                else
                {
                    colors[colorIndex++] = 0.6f * vertexPosition.y / 32.f;
                    colors[colorIndex++] = 0.8f * vertexPosition.y / 32.f;;
                    colors[colorIndex++] = 0.4f * vertexPosition.y / 32.f;
                }


                vertexPosition = generateVertexPosition(noise, x, z + 1);
                vertices[vertexIndex++] = vertexPosition.x;
                vertices[vertexIndex++] = vertexPosition.y;
                vertices[vertexIndex++] = vertexPosition.z;
                if(vertexPosition.y < 8)
                {
                    colors[colorIndex++] = 0.1f * vertexPosition.y / 8.f;
                    colors[colorIndex++] = 0.4f * vertexPosition.y / 8.f;
                    colors[colorIndex++] = 0.8f * vertexPosition.y / 8.f;
                }
                else if(vertexPosition.y < 20)
                {
                    colors[colorIndex++] = 0.2f * vertexPosition.y / 20.f;
                    colors[colorIndex++] = 0.7f * vertexPosition.y / 20.f;
                    colors[colorIndex++] = 0.3f * vertexPosition.y / 20.f;
                }
                else
                {
                    colors[colorIndex++] = 0.6f * vertexPosition.y / 32.f;
                    colors[colorIndex++] = 0.8f * vertexPosition.y / 32.f;;
                    colors[colorIndex++] = 0.4f * vertexPosition.y / 32.f;;
                }
            }
        }


        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &positionBuffer);
        glGenBuffers(1, &colorBuffer);
       
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
        glBufferData(GL_ARRAY_BUFFER, 18 * TERRAIN_SIZE * TERRAIN_SIZE * sizeof(float), &vertices[0], GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
        glBufferData(GL_ARRAY_BUFFER, 18 * TERRAIN_SIZE * TERRAIN_SIZE * sizeof(float), &colors[0], GL_STATIC_DRAW);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0); 

        glBindVertexArray(0);

        

        delete vertices;
        delete colors;
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

int TerrainChunk::TERRAIN_SIZE = 128;
int TerrainChunk::NOISE_HEIGHT_SCALE = 32;

#endif