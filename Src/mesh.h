#ifndef MESH_H
#define MESH_H

#include <iostream>
#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <assimp/scene.h>
#include <glad/glad.h>

struct Material
{
    glm::vec4 ambient;
    glm::vec4 diffuse;
    glm::vec4 specular;

    float shininess;
};

class Mesh
{
public:
    GLuint numVertices;
    GLuint numIndices;

    Material material;

    std::vector<GLuint> indices;

    std::vector<GLfloat> positions;
    std::vector<GLfloat> normals;
    std::vector<GLfloat> texCoords;
    std::vector<GLfloat> tangents;

    void createOnGPU();

    GLuint getVertexArray() { return vertexArray; }

    

private: 
    GLuint vertexArray;
    GLuint indexBuffer;
    GLuint vertexBuffers[4];

    bool createdOnGPU;

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
};





#endif