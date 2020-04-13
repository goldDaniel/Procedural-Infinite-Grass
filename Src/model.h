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
	
};

#endif //__MODEL_H__