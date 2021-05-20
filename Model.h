#ifndef MODEL_H
#define MODEL_H

#include "Mesh.h"
#include "Shader.h"
#include <string>
#include <vector>


using namespace std;


class Model
{
public:
    vector<Texture> textures_loaded;
    vector<Mesh>    meshes;
    string directory;
    bool gammaCorrection;

    Model(string const& path, bool invertedTexture = false, bool gamma = false);
    void Draw(Shader* shader);

private:
    void loadModel(string const& path, bool invertedTexture);
    void processNode(aiNode* node, const aiScene* scene, bool invertedTexture);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene, bool invertedTexture);
    vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
};

unsigned int TextureFromFile(const char* path, const string& directory, bool gamma = false);

#endif