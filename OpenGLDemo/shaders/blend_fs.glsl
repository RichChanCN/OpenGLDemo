#version 330 core
out vec4 FragColor;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;    
    float shininess;
};
struct Tex_Material  
{   
	sampler2D texture_ambient;
    sampler2D texture_diffuse;
    sampler2D texture_specular;
	sampler2D texture_normal;
    float shininess; 
}; 
struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;  
in vec3 Normal;  

uniform bool hasAmbientTex;
uniform bool hasDiffuseTex;
uniform bool hasSpecularTex;
uniform bool hasNormalTex;

uniform vec3 viewPos;
uniform Tex_Material tex_material;
uniform Material material;
uniform Light light;

in vec2 TexCoords;

void main()
{   
    vec3 norm = normalize(Normal);
	if(hasNormalTex){
		vec3 norm = texture(tex_material.texture_normal, TexCoords).rgb;
		norm = normalize(norm * 2.0 - 1.0);
	}

	// ambient
	vec4 ambient;
	if(hasAmbientTex){
		ambient = vec4(light.ambient, 1.0) * texture(tex_material.texture_ambient, TexCoords);
	}
	else{
		ambient =  vec4(light.ambient, 1.0) *  vec4(material.ambient, 1.0);
	}

    // diffuse 
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
	vec4 diffuse;
	if(hasDiffuseTex){
		diffuse = vec4(light.diffuse, 1.0) * (diff * texture(tex_material.texture_diffuse, TexCoords));
	}
	else{
		diffuse = vec4(light.diffuse * (diff * material.diffuse), 1.0);
	}

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
	vec4 specular; 
	if(hasSpecularTex){
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), tex_material.shininess);
		specular = vec4(light.specular, 1.0) * (spec * texture(tex_material.texture_specular, TexCoords));  
	}
	else{
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
		specular = vec4(light.specular * (spec * material.specular), 1.0); 
	}
  	
    FragColor = ambient + diffuse + specular;
}