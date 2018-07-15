#include "mesh.h"
#include "model.h"
//��Ⱦmesh
void Mesh::Draw(Shader* shader)
{
	//������ͼ����
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

	//���ù�����־����
	shader->setBool("hasBone", hasBone);
	shader->setInt("boneNum", boneNum);

    //����
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0);
}
// ��ʼ���������黺��
void Mesh::setupMesh()
{
    //��������
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    //��������
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //��������
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
	//�����������
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    //���ö�������ָ��
    //����λ��
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    //������
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    //��ͼ
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
    //��������
    glEnableVertexAttribArray(3);
    //ע��������ĺ���ʽ��int�õ�
    glVertexAttribIPointer(3, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, bonedata[0].IDs));
    glEnableVertexAttribArray(4);
    glVertexAttribIPointer(4, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, bonedata[1].IDs));
    glEnableVertexAttribArray(5);
    glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, bonedata[2].IDs));
    glEnableVertexAttribArray(6);
    glVertexAttribIPointer(6, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, bonedata[3].IDs));
    //Ȩ��
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