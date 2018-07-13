#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in ivec4 BoneIDs;
layout (location = 4) in vec4 Weights;

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
		mat4 BoneTransform = gBones[BoneIDs[0]] * Weights[0];
		for (int i = 1; i < boneNum; i++){
			BoneTransform += gBones[BoneIDs[i]] * Weights[i];
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