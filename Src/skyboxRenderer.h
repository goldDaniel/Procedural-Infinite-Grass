#ifndef SKYBOX_RENDERER_H
#define SKYBOX_RENDERER_H

#include "glad/glad.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "shader.h"

#include <vector>
#include <string>

#include <glm/gtx/transform.hpp>

class SkyboxRenderer
{
private: 

    float skyboxVertices[36*3] = 
    {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    const char* vertexSource = 
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"

    "out vec3 texCoords;\n"

    "uniform mat4 proj;\n"
    "uniform mat4 view;\n"

    "void main()\n"
    "{\n"
        "texCoords = aPos;\n"
        "vec4 pos = proj * view * vec4(aPos, 1.0);\n"
        "gl_Position = pos.xyww;\n"
    "}\n";

    const char* fragmentSource = 
    "#version 330 core\n"
    
    "in vec3 texCoords;\n"

    "uniform samplerCube cubemap;\n"

    "out vec4 FragColor;\n"

    "void main()\n"
    "{\n"             
        "FragColor = texture(cubemap, texCoords);\n"
    "}\n";

    unsigned int VAO;
    unsigned int VBO;

    unsigned int cubemapID;

    Shader* skyboxShader;

public:
    SkyboxRenderer()
    {
        glGenTextures(1, &cubemapID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapID);

        std::vector<std::string> faceFilepaths = 
        {
            "Assets/Skybox/posx.png",
            "Assets/Skybox/negx.png",
            "Assets/Skybox/posy.png",
            "Assets/Skybox/negy.png",
            "Assets/Skybox/posz.png",
            "Assets/Skybox/negz.png",
        };

        int width, height, nrChannels;
        unsigned char *data;  
        for(GLuint i = 0; i < faceFilepaths.size(); i++)
        {
            stbi_set_flip_vertically_on_load(false);
            data = stbi_load(faceFilepaths[i].c_str(), &width, &height, &nrChannels, 0);
            if(!data)
            {
                std::cout << "COULD NOT LOAD CUBEMAP TEX" << std::endl;
            }
            else
            {
                glTexImage2D(
                        GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
                        0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data
                );    
            }
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);  


        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

        skyboxShader = new Shader(vertexSource, fragmentSource);

        skyboxShader->use();
        skyboxShader->setInt("cubemap", 0);
    }

    ~SkyboxRenderer()
    {
        glDeleteTextures(1, &cubemapID);

        delete skyboxShader;
    }

    void draw(glm::mat4 view)
    {
        skyboxShader->use();
        glm::mat4 proj = glm::perspective(45.f, 1280.f/720.f, 0.01f, 10000.f);
        view = glm::mat4(glm::mat3(view)); // remove translation from the view matrix
        skyboxShader->setMat4("proj", proj);
        skyboxShader->setMat4("view", view);
        
        
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);  
        
        glBindVertexArray(VAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapID);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        
        glBindVertexArray(0);
        glDepthFunc(GL_LESS); 
    }
};

#endif