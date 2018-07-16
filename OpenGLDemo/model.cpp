#include "model.h"
Model::Model(string const &path, Shader* shader, Type tt) :
GameObject(shader, tt),
importer(new Assimp::Importer())
{
	hasBone = false;
	hasAnimation = false;
	boneNum = 0;
	scene = importer->ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	loadModel(path);
	cur_animation = scene;
}

Model::Model(Shader* shader, Type tt):
GameObject(shader, tt)
{
    importer = NULL;
    scene = NULL;
    cur_animation = NULL;
    hasBone = false;
    hasAnimation = false;
    boneNum = 0;
}

Model::~Model(){
	for (map<string, Animation* >::iterator it = animations.begin(); it != animations.end(); it++)
	{
		if (it->second != NULL){
			delete(it->second);
			it->second = NULL;
		}
	}
	animations.clear();

	if (cur_animation != NULL){
		delete(cur_animation);
		cur_animation = NULL;
	}

	if (scene != NULL){
		delete(scene);
		scene = NULL;
	}

	if (importer != NULL){
		delete(importer);
		importer = NULL;
	}
}

void Model::draw(float time){
	if (type == EMPTY || shader == NULL)
		return;
	glm::mat4 model_mat = getModelMat1(this).tomat4();
	shader->setMat4("model", model_mat);

	drawModel(time);
}

void Model::drawModel(float time)
{
    if (hasBone){
        vector<glm::mat4> transform;
        transform.resize(bone_infos.size());
        BoneTransform(time, transform);
        char name[32];
        memset(name, 0, sizeof(name));
        for (size_t i = 0; i < transform.size(); i++)
        {
            _snprintf_s(name, sizeof(name), "gBones[%d]", i);
            shader->setMat4(name, transform[i]);
        }
    }

    for (unsigned int i = 0; i < meshes.size(); i++)
        meshes[i].Draw(shader);
}

void Model::changeAnimation(string name){
    if (animations.find(name) != animations.end())
        cur_animation = animations[name]->scene;
}

Model* Model::plane(Shader* shader){
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;

    float planeVertices[] = {
        // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
        5.0f, 0.0f, 5.0f, 2.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -5.0f, 0.0f, 5.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -5.0f, 0.0f, -5.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,
        5.0f, 0.0f, -5.0f, 2.0f, 2.0f, 0.0f, 1.0f, 0.0f,
    };

    indices = { // 注意索引从0开始! 
        0, 1, 3, // 第一个三角形
        1, 2, 3  // 第二个三角形
    };

    Vertex temp;

    for (int i = 0; i < 4; i++)
    {
        temp.Position = glm::vec3(planeVertices[0 + i * 8], planeVertices[1 + i * 8], planeVertices[2 + i * 8]);
        temp.TexCoords = glm::vec2(planeVertices[3 + i * 8], planeVertices[4 + i * 8]);
        temp.Normal = glm::vec3(planeVertices[5 + i * 8], planeVertices[6 + i * 8], planeVertices[7 + i * 8]);
        vertices.push_back(temp);
    }

    Material mat;
    mat.specular = glm::vec3(0.0f, 0.0f, 0.0f);

    Texture texture;
    texture.id = TextureFromFile("wall.jpg", "assets\\images");
    texture.type = "texture_diffuse";
    texture.path = "assets\\images\\wall.jpg";
    textures.push_back(texture);

    Model* ret = new Model(shader);
    ret->meshes.push_back(Mesh(ret, vertices, indices, textures, ret->hasBone, ret->boneNum, mat));

    return ret;
}

Model* Model::vertical_plane(Shader* shader){
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;

    float planeVertices[] = {
        0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.0f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    };

    indices = { // 注意索引从0开始! 
        0, 1, 2, // 第一个三角形
        0, 2, 3  // 第二个三角形
    };

    Vertex temp;

    for (int i = 0; i < 4; i++)
    {
        temp.Position = glm::vec3(planeVertices[0 + i * 8], planeVertices[1 + i * 8], planeVertices[2 + i * 8]);
        temp.TexCoords = glm::vec2(planeVertices[3 + i * 8], planeVertices[4 + i * 8]);
        temp.Normal = glm::vec3(planeVertices[5 + i * 8], planeVertices[6 + i * 8], planeVertices[7 + i * 8]);
        vertices.push_back(temp);
    }

    Material mat;

    Texture texture;
    texture.id = TextureFromFile("blending_transparent_window.png", "assets\\images");
    texture.type = "texture_diffuse";
    texture.path = "assets\\images\\blending_transparent_window.png";
    textures.push_back(texture);

    Model* ret = new Model(shader, BLEND_OBJECT);
    ret->meshes.push_back(Mesh(ret, vertices, indices, textures, ret->hasBone, ret->boneNum, mat));

    return ret;
}

void Model::loadModel(string const &path)
{
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // 如果没有mesh
    {
        cout << "ERROR::ASSIMP:: " << importer->GetErrorString() << endl;
        return;
    }
    hasAnimation = scene->HasAnimations();
    if (hasAnimation){
        animations["default"] = new Animation(importer, scene);
    }
    // retrieve the directory path of the filepath
    directory = path.substr(0, path.find_last_of('\\'));

    // process ASSIMP's root node recursively
    processNode(scene->mRootNode, scene);
}

// loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
void Model::loadAnimation(string const &path, string anim_name)
{
    Assimp::Importer* anim_importer = new Assimp::Importer();
    const aiScene* anim_scene = anim_importer->ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    //这里不用判空mesh，因为可能只有动作数据
    if (!anim_scene)
    {
        cout << "ERROR::ASSIMP:: " << importer->GetErrorString() << endl;
        return;
    }
    if (anim_scene->HasAnimations())
    {
		animations[anim_name] = new Animation(anim_importer, anim_scene);
    }
}

// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
void Model::processNode(aiNode *node, const aiScene *scene)
{
    // process each mesh located at the current node
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        // the node object only contains indices to index the actual objects in the scene. 
        // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }

}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
    // data to fill
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;

	bool mesh_hasBone = false;
    //为了防止向量多次自动扩容影响加载速度
    vertices.reserve(mesh->mNumVertices);

    //遍历mesh的所有顶点
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
        // positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;
        // normals
        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.Normal = vector;
        // texture coordinates
        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
        }
        else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }
    //骨骼信息
    if (mesh->HasBones()){
        mesh_hasBone = hasBone = true;
        unsigned BoneIndex;
        for (unsigned int i = 0; i < mesh->mNumBones; i++) {
			//从骨骼数组中获取骨骼名字
            string BoneName(mesh->mBones[i]->mName.data);
			//从已有的map中查找
            if (bone_mapping.find(BoneName) == bone_mapping.end()) {//没有的话再map中加入这个骨骼，并赋予一个index
                BoneIndex = boneNum;
                boneNum++;
                BoneInfo binfo;
                bone_infos.push_back(binfo);
                bone_mapping[BoneName] = BoneIndex;
            }
            else {
                BoneIndex = bone_mapping[BoneName];
            }
			//以index为索引创建一个骨骼信息的vector，记录骨骼的偏移矩阵
            bone_infos[BoneIndex].BoneOffsetMat4 = mesh->mBones[i]->mOffsetMatrix;

			//为每个顶点绑上骨骼信息
            for (unsigned j = 0; j < mesh->mBones[i]->mNumWeights; j++) {
                unsigned int VertexID = + mesh->mBones[i]->mWeights[j].mVertexId;
                float Weight = mesh->mBones[i]->mWeights[j].mWeight;
                vertices[VertexID].addBoneData(BoneIndex, Weight);
            }
        }
    }
    // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    // process materials
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

    Material mat;
    aiColor3D ambi(0.0f, 0.0f, 0.0f);
    material->Get(AI_MATKEY_COLOR_DIFFUSE, ambi);
    mat.ambient = glm::vec3(ambi.r, ambi.g, ambi.b);
    aiColor3D diff(0.0f, 0.0f, 0.0f);
    material->Get(AI_MATKEY_COLOR_DIFFUSE, diff);
    mat.diffuse = glm::vec3(diff.r, diff.g, diff.b);
    aiColor3D spec(0.0f, 0.0f, 0.0f);
    material->Get(AI_MATKEY_COLOR_DIFFUSE, spec);
    mat.specular = glm::vec3(spec.r, spec.g, spec.b);

    // 1. ambient maps
    vector<Texture> ambientMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_ambient");
    textures.insert(textures.end(), ambientMaps.begin(), ambientMaps.end());
    // 2. diffuse maps
    vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    // 3. specular maps
    vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    // 4. normal maps
    std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_NORMALS, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    // 5. height maps
    std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_height");
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    // return a mesh object created from the extracted mesh data
    return Mesh(this, vertices, indices, textures, mesh_hasBone, mesh->mNumBones, mat);
}

vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName)
{
    vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);

        bool skip = false;
        for (unsigned int j = 0; j < textures_loaded.size(); j++)
        {
            if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }
        if (!skip)
        {   
            Texture texture;
            texture.id = TextureFromFile(str.C_Str(), this->directory);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture);
        }
    }
    return textures;
}


unsigned int TextureFromFile(const char *path, const string &directory, bool gamma)
{
    string filename = string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
unsigned int Model::FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
    assert(pNodeAnim->mNumRotationKeys > 0);

    for (unsigned int i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++) {
        if (AnimationTime < (float)pNodeAnim->mRotationKeys[i + 1].mTime) {
            return i;
        }
    }

    assert(0);
}

unsigned int Model::FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
    assert(pNodeAnim->mNumScalingKeys > 0);

    for (unsigned int i = 0; i < pNodeAnim->mNumScalingKeys - 1; i++) {
        if (AnimationTime < (float)pNodeAnim->mScalingKeys[i + 1].mTime) {
            return i;
        }
    }

    assert(0);
}

unsigned int Model::FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
    assert(pNodeAnim->mNumPositionKeys > 0);

    for (unsigned int i = 0; i < pNodeAnim->mNumPositionKeys - 1; i++) {
        if (AnimationTime < (float)pNodeAnim->mPositionKeys[i + 1].mTime) {
            return i;
        }
    }

    assert(0);
}

void Model::CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
    //至少两个值才能计算差值，一个值就直接返回
    if (pNodeAnim->mNumRotationKeys == 1) {
        Out = pNodeAnim->mRotationKeys[0].mValue;
        return;
    }

    unsigned int RotationIndex = FindRotation(AnimationTime, pNodeAnim);
    unsigned int NextRotationIndex = (RotationIndex + 1);
    assert(NextRotationIndex < pNodeAnim->mNumRotationKeys);
    float DeltaTime = pNodeAnim->mRotationKeys[NextRotationIndex].mTime - pNodeAnim->mRotationKeys[RotationIndex].mTime;
    float Factor = (AnimationTime - (float)pNodeAnim->mRotationKeys[RotationIndex].mTime) / DeltaTime;
    assert(Factor >= 0.0f && Factor <= 1.0f);
    const aiQuaternion& StartRotationQ = pNodeAnim->mRotationKeys[RotationIndex].mValue;
    const aiQuaternion& EndRotationQ = pNodeAnim->mRotationKeys[NextRotationIndex].mValue;
    aiQuaternion::Interpolate(Out, StartRotationQ, EndRotationQ, Factor);
    Out = Out.Normalize();
}

void Model::CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
    if (pNodeAnim->mNumScalingKeys == 1) {
        Out = pNodeAnim->mScalingKeys[0].mValue;
        return;
    }

    unsigned ScalingIndex = FindScaling(AnimationTime, pNodeAnim);
    unsigned NextScalingIndex = (ScalingIndex + 1);
    assert(NextScalingIndex < pNodeAnim->mNumScalingKeys);
    float DeltaTime = (float)(pNodeAnim->mScalingKeys[NextScalingIndex].mTime - pNodeAnim->mScalingKeys[ScalingIndex].mTime);
    float Factor = (AnimationTime - (float)pNodeAnim->mScalingKeys[ScalingIndex].mTime) / DeltaTime;
    assert(Factor >= 0.0f && Factor <= 1.0f);
    const aiVector3D& Start = pNodeAnim->mScalingKeys[ScalingIndex].mValue;
    const aiVector3D& End = pNodeAnim->mScalingKeys[NextScalingIndex].mValue;
    aiVector3D Delta = End - Start;
    Out = Start + Factor * Delta;
}

void Model::CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
    if (pNodeAnim->mNumPositionKeys == 1) {
        Out = pNodeAnim->mPositionKeys[0].mValue;
        return;
    }

    unsigned PositionIndex = FindPosition(AnimationTime, pNodeAnim);
    unsigned NextPositionIndex = (PositionIndex + 1);
    assert(NextPositionIndex < pNodeAnim->mNumPositionKeys);
    float DeltaTime = (float)(pNodeAnim->mPositionKeys[NextPositionIndex].mTime - pNodeAnim->mPositionKeys[PositionIndex].mTime);
    float Factor = (AnimationTime - (float)pNodeAnim->mPositionKeys[PositionIndex].mTime) / DeltaTime;
    assert(Factor >= 0.0f && Factor <= 1.0f);
    const aiVector3D& Start = pNodeAnim->mPositionKeys[PositionIndex].mValue;
    const aiVector3D& End = pNodeAnim->mPositionKeys[NextPositionIndex].mValue;
    aiVector3D Delta = End - Start;
    Out = Start + Factor * Delta;
}

void Model::ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const aiMatrix4x4& ParentTransform)
{
    string NodeName(pNode->mName.data);

    const aiAnimation* pAnimation = cur_animation->mAnimations[0];

    aiMatrix4x4 NodeTransformation(pNode->mTransformation);

    const aiNodeAnim* pNodeAnim = FindNodeAnim(pAnimation, NodeName);

    if (pNodeAnim) {
        // Interpolate scaling and generate scaling transformation matrix
        aiVector3D Scaling;
        CalcInterpolatedScaling(Scaling, AnimationTime, pNodeAnim);
        aiMatrix4x4 ScalingM;
        aiMatrix4x4::Scaling(Scaling, ScalingM);

        // Interpolate rotation and generate rotation transformation matrix
        aiQuaternion RotationQ;
        CalcInterpolatedRotation(RotationQ, AnimationTime, pNodeAnim);
        aiMatrix4x4 RotationM = aiMatrix4x4(RotationQ.GetMatrix());

        // Interpolate translation and generate translation transformation matrix
        aiVector3D Translation;
        CalcInterpolatedPosition(Translation, AnimationTime, pNodeAnim);
        aiMatrix4x4 TranslationM;
        aiMatrix4x4::Translation(Translation, TranslationM);

        // Combine the above transformations
        NodeTransformation = TranslationM * RotationM * ScalingM;
    }

    aiMatrix4x4 GlobalTransformation = ParentTransform * NodeTransformation;

    if (this->bone_mapping.find(NodeName) != this->bone_mapping.end()) {
        unsigned BoneIndex = this->bone_mapping[NodeName];
        this->bone_infos[BoneIndex].FinalTransformation = globalInverseTransform * GlobalTransformation *
            this->bone_infos[BoneIndex].BoneOffsetMat4;
    }

    for (unsigned i = 0; i < pNode->mNumChildren; i++) {
        ReadNodeHeirarchy(AnimationTime, pNode->mChildren[i], GlobalTransformation);
    }
}


void Model::BoneTransform(float TimeInSeconds, vector<glm::mat4>& Transforms)
{
    aiMatrix4x4 Identity;
    if (!cur_animation->HasAnimations())
        return;

    float TicksPerSecond = cur_animation->mAnimations[0]->mTicksPerSecond != 0 ?
        cur_animation->mAnimations[0]->mTicksPerSecond : 30.0f;
    float TimeInTicks = TimeInSeconds * TicksPerSecond;
    float AnimationTime = fmod(TimeInTicks, cur_animation->mAnimations[0]->mDuration);

    ReadNodeHeirarchy(AnimationTime, cur_animation->mRootNode, Identity);


    for (unsigned i = 0; i < this->bone_infos.size(); i++) {
        Transforms[i] = mymathlib::aiMat_row_cast(this->bone_infos[i].FinalTransformation);
    }
}

const aiNodeAnim* Model::FindNodeAnim(const aiAnimation* pAnimation, const string NodeName)
{
    for (unsigned i = 0; i < pAnimation->mNumChannels; i++) {
        const aiNodeAnim* pNodeAnim = pAnimation->mChannels[i];

        if (string(pNodeAnim->mNodeName.data) == NodeName) {
            return pNodeAnim;
        }
    }

    return NULL;
}