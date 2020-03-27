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
};

#endif //__MODEL_H__