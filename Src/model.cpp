#include "model.h"

#include "error.h"



#include <iostream>
#include <tuple>

#include <string>

static const std::string  DEFAULT_DIFFUSE_PATH = "Assets/default_diffuse.png";
static const std::string  DEFAULT_NORMAL_PATH  = "Assets/default_normal.png";


std::vector<Model*> Model::loadedModels;

Model::Model(const std::string path) : transform(mat4(1.f))
{
    loadModel(path);

    loadedModels.push_back(this);
}  



std::vector<Mesh*> Model::getMeshes()
{
    return meshes;
}


Mesh* Model::loadMesh(aiMesh* mesh, const aiScene* scene)
{
    Mesh* result = new Mesh();

    //VERTEX DATA/////////////////////////////////////////////////////////////
    result->numVertices = mesh->mNumVertices;
    result->numIndices = mesh->mNumFaces * 3;

     for(size_t i = 0; i < mesh->mNumVertices; ++i) 
     {
        result->positions.push_back(mesh->mVertices[i].x);
        result->positions.push_back(mesh->mVertices[i].y);
        result->positions.push_back(mesh->mVertices[i].z);

        result->normals.push_back(mesh->mNormals[i].x);
        result->normals.push_back(mesh->mNormals[i].y);
        result->normals.push_back(mesh->mNormals[i].z);

        if(mesh->HasTextureCoords(0)) 
        {
            result->texCoords.push_back(mesh->mTextureCoords[0][i].x);
            result->texCoords.push_back(mesh->mTextureCoords[0][i].y);
        } 
        else 
        {
            result->texCoords.push_back(0.0f);
            result->texCoords.push_back(0.0f);
        }

        if(mesh->HasTangentsAndBitangents()) 
        {
            result->tangents.push_back(mesh->mTangents[i].x);
            result->tangents.push_back(mesh->mTangents[i].y);
            result->tangents.push_back(mesh->mTangents[i].z);
        } 
        else 
        {
            result->tangents.push_back(0.f);
            result->tangents.push_back(0.f);
            result->tangents.push_back(0.f);
        }
    }

    //INDEX DATA/////////////////////////////////////////////////////////////
    for(size_t i = 0; i < mesh->mNumFaces; ++i) 
    {
        aiFace face = mesh->mFaces[i];
        for(size_t j = 0; j < face.mNumIndices; ++j) 
        {
            result->indices.push_back(face.mIndices[j]);
        }
    }  

    //MATERIAL DATA//////////////////////////////////////////////////////////
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

    result->material.shininess = material->Get(AI_MATKEY_SHININESS, result->material.shininess);

    if(!result->material.shininess)
    {
        result->material.shininess = 80.f;
    }
    result->material.shininess /= 3.f;


    aiColor3D color;
    material->Get(AI_MATKEY_COLOR_AMBIENT, color);
    result->material.ambient = glm::vec4(color.r, color.g, color.b, 1.0);


    material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
    result->material.diffuse = glm::vec4(color.r, color.g, color.b, 1.0);

    material->Get(AI_MATKEY_COLOR_SPECULAR, color);
    result->material.specular = glm::vec4(color.r, color.g, color.b, 1.0);


    std::cout << "=====BEFORE=====" << std::endl;
    std::cout << "AMBIENT: " << result->material.ambient.r << ":" << result->material.ambient.g << ":" << result->material.ambient.b << std::endl;
    std::cout << "DIFFUSE: " << result->material.diffuse.r << ":" << result->material.diffuse.g << ":" << result->material.diffuse.b << std::endl;
    std::cout << "SPECULAR: " << result->material.specular.r << ":" << result->material.specular.g << ":" << result->material.specular.b << std::endl;

    //sometimes materials ambient component is 0
    if(result->material.ambient.r < 0.1 && result->material.ambient.g < 0.1 && result->material.ambient.b < 0.1)
    {
        result->material.ambient = result->material.diffuse * 0.5f;
    }

    std::cout << "=====AFTER=====" << std::endl;
    std::cout << "AMBIENT: " << result->material.ambient.r << ":" << result->material.ambient.g << ":" << result->material.ambient.b << std::endl;
    std::cout << "DIFFUSE: " << result->material.diffuse.r << ":" << result->material.diffuse.g << ":" << result->material.diffuse.b << std::endl;
    std::cout << "SPECULAR: " << result->material.specular.r << ":" << result->material.specular.g << ":" << result->material.specular.b << std::endl;


    std::cout << "\n\n\n" << std::endl;

    result->createOnGPU();

    return result;
}

void Model::importAssimpNode(aiNode* node, const aiScene* scene)
{
    for(size_t i = 0; i < node->mNumMeshes; ++i) 
    {
        aiMesh* m = scene->mMeshes[node->mMeshes[i]];


        Mesh* loadedMesh = loadMesh(m, scene);

        meshes.push_back(loadedMesh);
    }

    for(size_t i = 0; i < node->mNumChildren; ++i) 
    {
        importAssimpNode(node->mChildren[i], scene);
    }
}


void Model::loadModel(std::string filepath)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filepath,
                                             aiProcess_Triangulate |
                                             aiProcess_JoinIdenticalVertices | 
                                             aiProcess_GenNormals |
                                             aiProcess_GenUVCoords |
                                             aiProcess_CalcTangentSpace);

    if(!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
    {
        Error::throwError(Error::CANT_LOAD_MODEL, importer.GetErrorString());
     
        return;
    }
    directory = filepath.substr(0, filepath.find_last_of('/'));
    
    importAssimpNode(scene->mRootNode, scene);
}