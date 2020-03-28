#ifndef TERRAIN_GRASS_RENDERER_H
#define TERRAIN_GRASS_RENDERER_H

#include "glad/glad.h"

#include "fastnoise/FastNoise.h"

#include "camera.h"
#include "shader.h"

#include "skyboxRenderer.h"
#include "terrainRenderer.h"
#include "terrainChunk.h"

#include "model.h"

namespace Renderer
{
    static bool initialized = false;

    const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aNormal;\n"
    "layout (location = 2) in vec2 aTexCoord;\n"
    "layout (location = 3) in vec3 aTangent;\n"

    "out vec3 modelPos;\n"
    "out vec3 fragPos;\n"
    "out vec2 texCoord;\n"
    "out vec3 normal;\n"
    "out mat3 TBNviewspace;\n"

    "uniform mat4 proj;\n"
    "uniform mat4 view;\n"
    "uniform mat4 model;\n"
    
    "void main(void)\n"
    "{\n"
        
        //world to viewspace
        "vec3 T = normalize(vec3(model * vec4(aTangent,   0.0)));\n"
        "vec3 N = normalize(vec3(model * vec4(aNormal,    0.0)));\n"
        "T = normalize(T - dot(T, N) * N);\n"
        "vec3 B = cross(T, N);\n"

        "normal = N;\n"

        "TBNviewspace = transpose(inverse(mat3(view * model))) * mat3(T, B, N);\n"

        "texCoord = aTexCoord;\n"
        "fragPos = vec3(view * model * vec4(aPos, 1.0));\n"

        "modelPos = (model * vec4(aPos, 1.f)).xyz;\n"

        "gl_Position = proj * view * model * vec4(aPos, 1.0);\n"

    "}\n"; 
    
    const char *fragmentShaderSource = "#version 330 core\n"
    "in vec3 modelPos;\n"
    "in vec3 fragPos;\n"
    "in vec3 normal;\n"
    "in vec2 texCoord;\n"
    "in mat3 TBNviewspace;\n"
    

    "struct Material\n"
    "{\n"
        "vec4 ambient;\n"
        "vec4 diffuse;\n"
        "vec4 specular;\n"

        "float shininess;\n"
    "};\n"

    "struct Light\n"
    "{\n"

        "vec3 direction;\n"
        "vec3 color;\n"

    "};\n"

    "uniform Light light;\n"
    "uniform Material material;\n"

    "out vec4 FragColor;\n"
        

    "vec3 phong(Light l, vec3 Normal)\n"
    "{\n"
        
        "vec3 lightDir = normalize(-l.direction);\n"

        "float d = max(dot(normalize(Normal), lightDir), 0.0);\n"
        
        "vec3 diffuse = d * l.color * material.diffuse.rgb;\n"

        "vec3 reflection = normalize(reflect(-lightDir, Normal));\n"

        "vec3 viewDir = normalize( -fragPos);\n"
        "float s = pow(max(dot(viewDir, reflection), 0.0), material.shininess);\n"
        "vec3 specular = s * l.color * material.specular.rgb;\n"

        "return ( material.ambient.rgb + diffuse + specular);\n"
    "}\n"   

    "void main(void)\n"
    "{\n"
        "vec3 Normal = normal;\n"
        "Normal = normalize(TBNviewspace * (Normal * 2.0 - vec3(1.0)));\n"

        "vec3 result = phong(light, Normal);\n"

        "FragColor =  vec4(result + (light.color * 0.25f), 1.f);\n"
    "}\n";

    TerrainRenderer* terrainRenderer;
    SkyboxRenderer* skyboxRenderer;
    Shader* shader;

    Model* model;


    std::vector<TerrainChunk*> chunks;

    const int terrainSize = 64;

    float elapsed = 0;

    void Dispose()
    {
        delete shader;
    }

    void Init()
    {
        shader = new Shader(vertexShaderSource, fragmentShaderSource);
        glEnable(GL_DEPTH_TEST);
        

        glEnable( GL_BLEND );
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        

        
        model = new Model("Assets/Kenney/grass.obj");

        terrainRenderer = new TerrainRenderer();
        skyboxRenderer = new SkyboxRenderer();


        FastNoise noise;
        noise.SetFrequency(0.005);

        int size = 8;
        for(int x = -size; x < size; ++x)
        {
            for(int z = -size; z < size; ++z)
            {   
                chunks.push_back(new TerrainChunk(noise, x, z));
            }
        }


        std::cout << "FINISHED LOADING" << std::endl;


        initialized = true;
    }

    void Draw(glm:: mat4 view)
    {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        elapsed += 0.01f;

        glm::mat4 proj = glm::perspective(45.f, 1280.f/720.f, 0.01f, 10000.f);


        skyboxRenderer->draw(view);
        terrainRenderer->draw(view, chunks);

        shader->use();
        shader->setMat4("proj", proj);
        shader->setMat4("view", view);
    

        shader->setVec3("light.direction", glm::vec3(0, 1, 1));
        shader->setVec3("light.color", glm::vec3(0.05f, 0.5f, 0.1f));

        // for (auto mesh : model->getMeshes()) 
        // {

        //     shader->setVec4("material.ambient",    mesh->material.ambient);
        //     shader->setVec4("material.diffuse",    mesh->material.diffuse);
        //     shader->setVec4("material.specular",   mesh->material.specular);    
        //     shader->setFloat("material.shininess", mesh->material.shininess);

        //     glBindVertexArray(mesh->getVertexArray());

        //     for(int x = 0; x < terrainSize; ++x)
        //     {
        //         for(int z = 0; z < terrainSize; ++z)
        //         {
        //             glm::mat4 model = glm::translate(glm::mat4(1.f), glm::vec3(x * 0.5f + glm::sin(z), 
        //                                                                        0, 
        //                                                                        z * 0.5f + glm::sin(x)));

        //             model = glm::rotate(model, x * 0.214f - z * 0.315f + x * z * 0.618f, glm::vec3(0, 1, 0));
        //             model = glm::scale(model, glm::vec3(0.2, 2, 0.2));

        //             shader->setMat4("model", model);            
        //             glDrawElements(GL_TRIANGLES, mesh->numIndices, GL_UNSIGNED_INT, 0);
        //         }   
        //     }
        // }
    }
}

#endif