#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in ivec4 BoneIDs1;
layout (location = 4) in ivec4 BoneIDs2;
layout (location = 5) in ivec4 BoneIDs3;
layout (location = 6) in ivec4 BoneIDs4;
layout (location = 7) in vec4 Weights1;
layout (location = 8) in vec4 Weights2;
layout (location = 9) in vec4 Weights3;
layout (location = 10) in vec4 Weights4;

out vec2 TexCoords;
out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

const int MAX_BONES = 100;
uniform bool hasBone;
uniform int boneNum;
uniform mat4 gBones[MAX_BONES];

void main()
{
	vec4 NormalL;
	vec4 PosL;
	if(hasBone){
		//mat4 BoneTransform = mat4(1.0,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0,1.0);
		mat4 BoneTransform = gBones[BoneIDs1[0]] * Weights1[0];
		for (int i = 1; i < boneNum; i++){
			int index = int(mod(i,4));
			if(i < 4)
				BoneTransform += gBones[BoneIDs1[index]] * Weights1[index];
			else if(i < 8)
				BoneTransform += gBones[BoneIDs2[index]] * Weights2[index];
			else if(i < 12)
				BoneTransform += gBones[BoneIDs3[index]] * Weights3[index];
			else
				BoneTransform += gBones[BoneIDs4[index]] * Weights4[index];
		}

		PosL = BoneTransform * vec4(aPos, 1.0);

		FragPos = vec3(model * PosL);

		NormalL = BoneTransform * vec4(aNormal, 0.0);

		Normal = mat3(transpose(inverse(model))) * vec3(NormalL.x, NormalL.y, NormalL.z);

		TexCoords = aTexCoords;  
		  
		gl_Position = projection * view * model * PosL;

	}
	else{
		FragPos = vec3(model * vec4(aPos, 1.0));
		Normal = mat3(transpose(inverse(model))) * aNormal; 
		TexCoords = aTexCoords;    
		gl_Position = projection * view * model * vec4(aPos, 1.0);
	}

}