#include "mesh.h"

#include <iostream>

const char* Mesh::vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aNormal;\n"
    "layout (location = 2) in vec2 aTexCoord;\n"
    "layout (location = 3) in vec3 aTangent;\n"

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

        "gl_Position = proj * view * model * vec4(aPos, 1.0);\n"

    "}\n"; 
    
const char* Mesh::fragmentShaderSource = "#version 330 core\n"
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

void Mesh::createOnGPU()
{
    if(createdOnGPU) return;

    glGenVertexArrays(1, &this->vertexArray);
    glBindVertexArray(this->vertexArray);
    glGenBuffers(1, &this->indexBuffer);
    glGenBuffers(4, &vertexBuffers[0]);

    GLuint* indices = &this->indices[0];
    GLfloat* vertices = &this->positions[0];
    GLfloat* normals = &this->normals[0];
    GLfloat* texCoords = &this->texCoords[0];
    GLfloat* tangents = &this->tangents[0];

    //indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->numIndices * sizeof(GLuint), indices, GL_STATIC_DRAW);

    //position
    glBindBuffer(GL_ARRAY_BUFFER, this->vertexBuffers[0]);
    glBufferData(GL_ARRAY_BUFFER, 3 * numVertices * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    //normal
    glBindBuffer(GL_ARRAY_BUFFER, this->vertexBuffers[1]);
    glBufferData(GL_ARRAY_BUFFER, 3 * numVertices * sizeof(GLfloat), normals, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    //texcoord
    glBindBuffer(GL_ARRAY_BUFFER, this->vertexBuffers[2]);
    glBufferData(GL_ARRAY_BUFFER, 2 * numVertices * sizeof(GLfloat), texCoords, GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(2);

    //tangent
    glBindBuffer(GL_ARRAY_BUFFER, this->vertexBuffers[3]);
    glBufferData(GL_ARRAY_BUFFER, 3 * numVertices * sizeof(GLfloat), tangents, GL_STATIC_DRAW);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(3);

    glBindVertexArray(0);

    createdOnGPU = true;
}