#ifndef MESH_H
#define MESH_H

#include <glad/glad.h> // holds all OpenGL type declarations

#include "shader.h"
#include "mymathlib.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <map>

#define NUM_BONES_PER_VEREX 4

using namespace std;

struct VertexBoneData
{
    VertexBoneData(){
        for (int i = 0; i < NUM_BONES_PER_VEREX; i++)
        {
            IDs[i] = -1;
            Weights[i] = 0.0f;
        }
    }
    unsigned int IDs[NUM_BONES_PER_VEREX];
    float Weights[NUM_BONES_PER_VEREX];

    void addBoneData(unsigned id, float w){
        for (int i = 0; i < NUM_BONES_PER_VEREX; i++)
        {
            if (IDs[i] == -1){
                IDs[i] = id;
                Weights[i] = w;
                return;
            }
        }
    }
};

struct BoneInfo{
    aiMatrix4x4 BoneOffsetMat4;
    aiMatrix4x4 FinalTransformation;
};

struct Vertex {
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;
    // bone
    VertexBoneData bonedata;
    /* delete the property for moment
    // tangent
    glm::vec3 Tangent;
    // bitangent
    glm::vec3 Bitangent;
    */
};

struct Texture {
    unsigned int id;
    string type;
    string path;
};

struct Material{
    Material(){
        ambient = glm::vec3(0.2, 0.2, 0.2);
        diffuse = glm::vec3(0.5, 0.5, 0.5);
        specular = glm::vec3(1.0, 1.0, 1.0);

        shininess = 32.0;
    }
    Material(glm::vec3 ambi, glm::vec3 diff, glm::vec3 sqec, float shin = 32.0){
        ambient = ambi;
        diffuse = diff;
        specular = sqec;

        shininess = shin;
    }
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    float shininess;
};

class Model;

class Mesh {
public:
    /*  Mesh Data  */
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;
    vector<VertexBoneData> bones;
    Material material;
    unsigned int VAO;

    /*  Functions  */
    // constructor
    Mesh(const Model* m, const aiScene* sc, vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures, vector<VertexBoneData> bones, Material material = Material()) :scene(sc), model(m)
    {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;
        this->material = material;
        this->bones = bones;
        // now that we have all the required data, set the vertex buffers and its attribute pointers.
        setupMesh();
    }

    // render the mesh
    void Draw(Shader* shader);

private:
    /*  Render data  */
    unsigned int VBO, EBO;
    const aiScene* scene;
    const Model* model;
    aiMatrix4x4 globalInverseTransform;
    /*  Functions    */
    // initializes all the buffer objects/arrays
    void setupMesh();
    //animation
    void CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
    void ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const aiMatrix4x4& ParentTransform);

    void CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
    void CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
    void CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);

    unsigned int FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);
    unsigned int FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
    unsigned int FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);

    const aiNodeAnim* FindNodeAnim(const aiAnimation* pAnimation, const string NodeName);
    aiMatrix4x4 Mesh::BoneTransform(float TimeInSeconds, vector<glm::mat4>& Transforms);
};
#endif