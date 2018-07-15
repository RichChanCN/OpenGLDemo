#include "mesh.h"
#include "model.h"
//渲染mesh
void Mesh::Draw(Shader* shader)
{
	//设置贴图参数
    for (unsigned int i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        string name = textures[i].type;
        if (name == "texture_ambient")
            shader->setBool("hasAmbientTex", true);
        if (name == "texture_diffuse")
            shader->setBool("hasDiffuseTex", true);
        if (name == "texture_specular")
            shader->setBool("hasSpecularTex", true);
        if (name == "texture_normal")
            shader->setBool("hasNormalTex", true);
        glUniform1i(glGetUniformLocation(shader->ID, ("tex_material." + name).c_str()), i);
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }
    shader->setVec3("material.ambient", material.ambient);
    shader->setVec3("material.diffuse", material.diffuse);
    shader->setVec3("material.specular", material.specular);
    shader->setFloat("tex_material.shininess", 32.0);
    shader->setFloat("material.shininess", 32.0);

	//设置骨骼标志符号
	shader->setBool("hasBone", hasBone);
	shader->setInt("boneNum", boneNum);

    //绘制
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0);
}
// 初始化顶点数组缓存
void Mesh::setupMesh()
{
    //创建缓存
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    //加载数据
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //创建缓存
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
	//索引缓存对象
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    //设置顶点属性指针
    //顶点位置
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    //法向量
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    //贴图
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
    //骨骼数据
    glEnableVertexAttribArray(3);
    //注意这下面的函数式传int用的
    glVertexAttribIPointer(3, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, bonedata[0].IDs));
    glEnableVertexAttribArray(4);
    glVertexAttribIPointer(4, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, bonedata[1].IDs));
    glEnableVertexAttribArray(5);
    glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, bonedata[2].IDs));
    glEnableVertexAttribArray(6);
    glVertexAttribIPointer(6, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, bonedata[3].IDs));
    //权重
	glEnableVertexAttribArray(7);
    glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bonedata[0].Weights));
    glEnableVertexAttribArray(8);
    glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bonedata[1].Weights));
    glEnableVertexAttribArray(9);
    glVertexAttribPointer(9, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bonedata[2].Weights));
    glEnableVertexAttribArray(10);
    glVertexAttribPointer(10, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bonedata[3].Weights));

    glBindVertexArray(0);
}