#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>

#include <tuple>

#include <glm/glm.hpp>
using namespace glm;

#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "shader.h"

#include "mesh.h"

class Model 
{
private:

	static std::vector<Model*> loadedModels;


	std::vector<Mesh*> meshes;

	std::string directory;

	Mesh* loadMesh(aiMesh* mesh, const aiScene* scene);

	void loadModel(std::string path);
	void importAssimpNode(aiNode* node, const aiScene* scene);
	

public:

	static std::vector<Model*> getModels() { return loadedModels; }

	mat4 transform;

	Model(const std::string path);
	~Model() {}

	

	std::vector<Mesh*> getMeshes();
	
	void Draw(Shader shader);
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
};

#endif //__MODEL_H__