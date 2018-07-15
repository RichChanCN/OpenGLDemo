/*
ģ���࣬�̳���gameobject�����ļ��м���ģ��������Ϣ���������mesh�����ʣ���ͼ�������������ȣ��ǿ��ƵĻ�����Ԫ
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

//���������ṹ
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

    string directory;//ģ���ļ�����Ŀ¼
    vector<Texture> textures_loaded;  //�����Ѿ����ص���ͼ�������ظ�����
    vector<Mesh> meshes;//��ģ�Ͱ�������������
    map<string, unsigned> bone_mapping;//�������ƺ�������ӳ��
    vector<BoneInfo> bone_infos;//������Ϣ�����飬����ͨ�������map�͹������ƶ�Ӧ
    bool hasBone;
    bool hasAnimation;
    unsigned boneNum;

	//��gameobject�̳еĻ����麯��
	virtual void draw(float time);
    
	//���ض����ļ�
    void loadAnimation(string const &path, string anim_name);
	//�л�����
    void changeAnimation(string name);
private:
	const aiScene* scene;
    Assimp::Importer* importer;
    const aiScene* cur_animation;
	map<string, Animation*> animations;
    aiMatrix4x4 globalInverseTransform;

    /*  Functions   */
	//����ģ�ͣ��麯��draw�ڲ�������
    void drawModel(float time);
    // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
    void loadModel(string const &path);

    // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);

    //������ͼ��ͬʱ�ж��Ƿ��Ѿ�������
    vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);

    //��������
    const aiNodeAnim* FindNodeAnim(const aiAnimation* pAnimation, const string NodeName);
    void BoneTransform(float TimeInSeconds, vector<glm::mat4>& Transforms);
    void ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const aiMatrix4x4& ParentTransform);
	//�ҵ���Ӧʱ���ĸ�ά����Ϣ
    unsigned int FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);
    unsigned int FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
    unsigned int FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);
	//�������ά�ȵĲ�ֵ
    void CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
    void CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
    void CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);

};
#endif