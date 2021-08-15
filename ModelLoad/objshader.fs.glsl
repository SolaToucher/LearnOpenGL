#version 330 core
out vec4 FragColor;

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
}; 

uniform PointLight pointLight;
uniform DirLight dirLight;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
uniform sampler2D texture_diffuse3;
uniform sampler2D texture_diffuse4;
uniform vec3 viewPos;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main(){
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = CalcPointLight(pointLight, norm, FragPos, viewDir);
    result += CalcDirLight(dirLight, norm, viewDir);
    //if(spotLight.show == 1)result += CalcSpotLight(spotLight, norm, FragPos, viewDir);

    FragColor = vec4(result, 1.0);
}


vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir){
    vec3 tex = vec3(texture(texture_diffuse1, TexCoords)) 
              +vec3(texture(texture_diffuse2, TexCoords))
              +vec3(texture(texture_diffuse3, TexCoords))
              +vec3(texture(texture_diffuse4, TexCoords)) ;

    vec3 lightDir = normalize(light.position - fragPos);
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);
    // 镜面光着色
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    // 衰减
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
                 light.quadratic * (distance * distance));    
    // 合并结果
    vec3 ambient  = light.ambient  * tex;
    vec3 diffuse  = light.diffuse  * diff * tex;
    vec3 specular = light.specular * spec * tex;
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir){
    vec3 tex = vec3(texture(texture_diffuse1, TexCoords)) 
              +vec3(texture(texture_diffuse2, TexCoords))
              +vec3(texture(texture_diffuse3, TexCoords))
              +vec3(texture(texture_diffuse4, TexCoords)) ;

    vec3 lightDir = normalize(-light.direction);

    vec3 ambient = light.ambient * tex;

    float diff = max(dot(normal, lightDir), 0);
    vec3 diffuse = diff * light.diffuse * tex;

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0), 32);
    vec3 specular = spec * light.specular * tex;

    return ambient + diffuse + specular;
}