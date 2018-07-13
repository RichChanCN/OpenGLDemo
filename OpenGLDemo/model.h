#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "stb_image.h"
#include "mesh.h"
#include "shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
using namespace std;

struct Frame {
    vector <Mesh*> meshes;
};

struct Animation {
    string name;
    vector <Frame*> frames;
};

static unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false);

class Model 
{
public:
    /*  Model Data */
    vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
    vector<Mesh> meshes;
    map<string, unsigned> bone_mapping;
    vector<Animation *> animations;
    string directory;
    vector<BoneInfo> bone_infos;
    bool gammaCorrection;
    bool hasBone;
    bool hasAnimation;
    unsigned boneNum;

    /*  Functions   */
    // constructor, expects a filepath to a 3D model.
	Model(string const &path, bool gamma = false) :
		gammaCorrection(gamma), 
		importer(Assimp::Importer())
    {
        hasBone = false;
        hasAnimation = false;
        boneNum = 0;
		scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
        loadModel(path);
    }

    // draws the model, and thus all its meshes
    void Draw(Shader* shader,float time);
    
private:
	const aiScene*  scene;
    Assimp::Importer importer;
    aiMatrix4x4 globalInverseTransform;
    /*  Functions   */
    // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
    void loadModel(string const &path);

    // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    void processNode(aiNode *node, const aiScene *scene);

    Mesh processMesh(aiMesh *mesh, const aiScene *scene);

    // checks all material textures of a given type and loads the textures if they're not loaded yet.
    // the required info is returned as a Texture struct.
    vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);

    //animation
    void ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const aiMatrix4x4& ParentTransform);

    void CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
    void CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
    void CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);

    unsigned int FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);
    unsigned int FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
    unsigned int FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);

    const aiNodeAnim* FindNodeAnim(const aiAnimation* pAnimation, const string NodeName);
    void BoneTransform(float TimeInSeconds, vector<glm::mat4>& Transforms);
};
#endif