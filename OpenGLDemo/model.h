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
#include "gameobject.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
using namespace std;

struct Animation {
    string name;
};

static unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false);

class Model : public GameObject
{
public:
    vector<Texture> textures_loaded;  //保存已经加载的贴图，避免重复加载
    vector<Mesh> meshes;
    map<string, unsigned> bone_mapping;
    vector<Animation *> animations;
    string directory;
    vector<BoneInfo> bone_infos;
    bool gammaCorrection;
    bool hasBone;
    bool hasAnimation;
    unsigned boneNum;

	Model(string const &path, Shader* shader, Type tt = COMMON_OBJECT, bool gamma = false);
	~Model();
	virtual void draw(float time);
    
    void loadAnimation(string const &path, string anim_name);
    void changeAnimation(string name);
private:
	const aiScene* scene;
    const aiScene* cur_animation;
    map<string, const aiScene* > animation_scenes;
    vector<Assimp::Importer*> anim_importers;
    Assimp::Importer importer;
    aiMatrix4x4 globalInverseTransform;

    /*  Functions   */
    void drawModel(float time);
    // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
    void loadModel(string const &path);

    // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);

    //加载贴图，同时判断是否已经被加载
    vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);

    //骨骼动画
    const aiNodeAnim* FindNodeAnim(const aiAnimation* pAnimation, const string NodeName);
    void BoneTransform(float TimeInSeconds, vector<glm::mat4>& Transforms);
    void ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const aiMatrix4x4& ParentTransform);
	//找到对应时间点的各维度信息
    unsigned int FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);
    unsigned int FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
    unsigned int FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);
	//计算各个维度的插值
    void CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
    void CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
    void CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);

};
#endif