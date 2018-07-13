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

#define NUM_BONES_PER_VEREX 16

using namespace std;

struct VertexBoneData
{
    VertexBoneData(){
        for (int i = 0; i < 4; i++)
        {
            IDs[i] = 100;
            Weights[i] = 0.0f;
        }
    }
    unsigned int IDs[4];
    float Weights[4];

    void addBoneData(unsigned id, float w){
        for (int i = 0; i < 4; i++)
        {
            if (IDs[i] == 100){
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
    VertexBoneData bonedata[NUM_BONES_PER_VEREX/4];
    /* delete the property for moment
    // tangent
    glm::vec3 Tangent;
    // bitangent
    glm::vec3 Bitangent;
    */

    void addBoneData(unsigned id, float w){
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                if (bonedata[i].IDs[j] == 100){
                    bonedata[i].IDs[j] = id;
                    bonedata[i].Weights[j] = w;
                    return;
                }
            }
        }
    }
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
    Material material;
    unsigned int VAO;
	bool hasBone;
	int boneNum;
    /*  Functions  */
    // constructor
	Mesh(const Model* m, const aiScene* sc, vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures, bool hasBone = false, int boneNum = 0, Material material = Material()) : model(m)
    {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;
        this->material = material;
		this->hasBone = hasBone;
		this->boneNum = boneNum;
        // now that we have all the required data, set the vertex buffers and its attribute pointers.
        setupMesh();
    }

    // render the mesh
    void Draw(Shader* shader);

private:
    /*  Render data  */
    unsigned int VBO, EBO;
    const Model* model;
    /*  Functions    */
    // initializes all the buffer objects/arrays
    void setupMesh();
};
#endif