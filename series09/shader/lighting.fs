#version 330 core
in vec3 normal;
in vec2 texCoord;
in vec3 position;
out vec4 fragColor;

struct Light {
  vec3 position;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

uniform Light light;

struct Material {
  sampler2D diffuse;
  sampler2D specular; // texture for specular lighting
  float shininess;
};

uniform Material material;

uniform vec3 viewPos;

void main() {
  
  // use texture
  vec3 texColor = texture(material.diffuse, texCoord).xyz;
  vec3 ambient = light.ambient * texColor;

  vec3 lightDir = normalize(light.position - position);
  vec3 pixelNorm = normalize(normal);

  
  float diff = max(dot(pixelNorm, lightDir), 0.0);

  // use texture
  vec3 diffuse = diff * light.diffuse * texColor;
  // specular lighting texture
  vec3 specColor = texture(material.specular, texCoord).xyz;

  // specular lighting
  vec3 viewDir = normalize(viewPos - position);
  vec3 reflectDir = reflect(-lightDir, pixelNorm);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
  

  vec3 specular = spec * light.specular * specColor;

  vec3 result = (ambient + diffuse + specular);
  fragColor = vec4(result, 1.0);
}
