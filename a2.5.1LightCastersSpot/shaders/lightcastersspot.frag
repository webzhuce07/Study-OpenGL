#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;  
in vec3 FragPos; 

uniform vec3 viewPos;

struct Material
{
    sampler2D  diffuse;
    sampler2D  specular;
    float shininess;
};
uniform Material material;

struct Light
{
    vec3 position;
	vec3 direction;
    float cutOff;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	
	float constant;
    float linear;
    float quadratic;
};
uniform Light light;

void main()
{
    vec3 lightDir = normalize(light.position - FragPos);
	// Check if lighting is inside the spotlight cone
    float theta = dot(lightDir, normalize(-light.direction)); 
	if(theta > light.cutOff) // Remember that we're working with angles as cosines instead of degrees so a '>' is used.
    {    

		 // Ambient
		vec3 ambient = light.ambient *  vec3(texture(material.diffuse, TexCoords));

		// Diffuse 
		vec3 norm = normalize(Normal);
		//vec3 lightDir = normalize(light.position - FragPos);
		//vec3 lightDir = normalize(-light.direction);
		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

		// Specular
		vec3 viewDir = normalize(viewPos - FragPos);
		vec3 reflectDir = reflect(-lightDir, norm);  
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
		vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));  
		
		 // Attenuation
		float distance    = length(light.position - FragPos);
		float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));  
		
		// ambient  *= attenuation;  // Also remove attenuation from ambient, because if we move too far, the light in spotlight would then be darker than outside (since outside spotlight we have ambient lighting).
		diffuse  *= attenuation;
		specular *= attenuation;
		
		vec3 result = ambient + diffuse + specular;
		FragColor = vec4(result, 1.0f);
	}
	 else    // else, use ambient light so scene isn't completely dark outside the spotlight.
        FragColor = vec4(light.ambient * vec3(texture(material.diffuse, TexCoords)), 1.0f);
}