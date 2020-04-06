#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>

#include <tuple>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
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
	Model(const std::string path);
	~Model() {}

    Shader* getShader();
	

	std::vector<Mesh*> getMeshes();
	
	void Draw(mat4 view, mat4 proj, mat4 transform)
    {
        getShader()->use();
        getShader()->setMat4("proj", proj);
        getShader()->setMat4("view", view);
        getShader()->setMat4("model", transform);

        getShader()->setVec3("light.direction", glm::vec3(0, 1.f, 1.f));
        getShader()->setVec3("light.color", glm::vec3(0.1f, 0.4f, 0.2f));

        for (auto mesh : meshes) 
        {
            getShader()->setVec4("material.ambient",    mesh->material.ambient);
            getShader()->setVec4("material.diffuse",    mesh->material.diffuse);
            getShader()->setVec4("material.specular",   mesh->material.specular);    
            getShader()->setFloat("material.shininess", mesh->material.shininess);

            glBindVertexArray(mesh->getVertexArray());
            glDrawElements(GL_TRIANGLES, mesh->numIndices, GL_UNSIGNED_INT, 0);
        }
    }
	
};

#endif //__MODEL_H__