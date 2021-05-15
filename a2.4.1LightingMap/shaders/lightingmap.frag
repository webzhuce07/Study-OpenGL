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
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light light;

void main()
{
     // 环境光
    vec3 ambient = light.ambient *  vec3(texture(material.diffuse, TexCoords));

    // 漫反射光
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

    // 镜面高光
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));  

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0f);
}