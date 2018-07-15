/*
模型类，继承于gameobject，从文件中加载模型数据信息，包含多个mesh，材质，贴图，骨骼，动画等，是控制的基本单元
*/
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

//骨骼动画结构
struct Animation{
	Animation(){
		importer = NULL;
		scene = NULL;
	}
	Animation(Assimp::Importer* ip, const aiScene* sc){
		importer = ip;
		scene = sc;
	}
	~Animation(){
		delete scene;
		scene = NULL;
		delete importer;
		scene = NULL;
	}
	Assimp::Importer* importer;
	const aiScene* scene;
};

static unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false);

class Model : public GameObject
{
public:
	Model(string const &path, Shader* shader, Type tt = COMMON_OBJECT, bool gamma = false);
	~Model();

    string directory;//模型文件所在目录
    vector<Texture> textures_loaded;  //保存已经加载的贴图，避免重复加载
    vector<Mesh> meshes;//该模型包含的网格数组
    map<string, unsigned> bone_mapping;//骨骼名称和索引的映射
    vector<BoneInfo> bone_infos;//骨骼信息的数组，索引通过上面的map和骨骼名称对应
    bool hasBone;
    bool hasAnimation;
    unsigned boneNum;

	//从gameobject继承的绘制虚函数
	virtual void draw(float time);
    
	//加载动画文件
    void loadAnimation(string const &path, string anim_name);
	//切换动画
    void changeAnimation(string name);
private:
	const aiScene* scene;
    Assimp::Importer* importer;
    const aiScene* cur_animation;
	map<string, Animation*> animations;
    aiMatrix4x4 globalInverseTransform;

    /*  Functions   */
	//绘制模型，虚函数draw内部调用它
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