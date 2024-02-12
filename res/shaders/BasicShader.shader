#shader vertex
#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 trans;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * trans * vec4(aPos, 1.0f); 
    TexCoord = aTexCoord;
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    // Normal = aNormal;
}

#shader fragment
# version 410 core

out vec4 FragColor;

struct Material {
    sampler2D diffuse; // diffuse = ambient
    sampler2D specular;
    sampler2D emission;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

// Light
uniform vec3 viewPos;
uniform Material material;
uniform Light light;
uniform float time;


void main() {
    vec3 lightDir = normalize(light.position - FragPos);

    // Ambient
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoord).rgb;

    // Diffuse
    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoord).rgb;
    
    // Specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, TexCoord).rgb;
    
    // Emission
    vec3 emission = vec3(0.0);
    // rough check for blackbox inside spec texture 
    if (texture(material.specular, TexCoord).rgb == vec3(0.0f)) {
        // apply emission texture
        emission = texture(material.emission, TexCoord).rgb;
        
        // some extra fun stuff with "time uniform" 
        emission = texture(material.emission, TexCoord + vec2(0.0,time)).rgb;   /*moving */
        emission = emission * (sin(time) * 0.5 + 0.5) * 2.0;                     /*fading */
    }

    // spolight
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = (light.cutOff - light.outerCutOff);
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    diffuse  *= intensity;
    specular *= intensity;

    // attenuation
    float distance    = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

    ambient  *= attenuation;  
    diffuse   *= attenuation;
    specular *= attenuation;
    emission *= attenuation;

    vec3 Color = ambient + diffuse + specular + emission;
    FragColor = vec4(Color, 1.0f);
}