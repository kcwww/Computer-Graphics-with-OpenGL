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
  // vec3 ambient;
  // vec3 diffuse;
  sampler2D diffuse; // use texture

  // specular;
  sampler2D specular; // use texture
  // vec3 specular;

  float shininess;
};

uniform Material material;

// uniform vec3 lightPos;
// uniform vec3 lightColor;
// uniform vec3 objectColor;
// uniform float ambientStrength;

// // specular lighting
// uniform float specularStrength; 
// uniform float specularShininess;

uniform vec3 viewPos;

void main() {
  // ambient lighting
  // vec3 ambient = ambientStrength * lightColor;
  // vec3 lightDir = normalize(lightPos - position);

  // use material
  // vec3 ambient = light.ambient * material.ambient;

  // use texture
  vec3 texColor = texture(material.diffuse, texCoord).xyz;
  vec3 ambient = light.ambient * texColor;

  vec3 lightDir = normalize(light.position - position);
  vec3 pixelNorm = normalize(normal);

  // lightDir과 pixelNorm의 내적을 구하면 두 벡터가 얼마나 비슷한 방향을 향하고 있는지 알 수 있다. 이 값이 0보다 작으면 두 벡터가 반대 방향을 향하고 있음을 의미한다.
  // vec3 diffuse = max(dot(pixelNorm, lightDir), 0.0) * lightColor;
  
  float diff = max(dot(pixelNorm, lightDir), 0.0);
  
  // use material
  // vec3 diffuse = diff * light.diffuse * material.diffuse;

  // use texture
  vec3 diffuse = diff * light.diffuse * texColor;

  // specular lighting texture
  vec3 specColor = texture(material.specular, texCoord).xyz;

  // specular lighting
  vec3 viewDir = normalize(viewPos - position); // direction from fragment to camera
  vec3 reflectDir = reflect(-lightDir, pixelNorm); // direction of reflected light
  // float spec = pow(max(dot(viewDir, reflectDir), 0.0), specularShininess); // 0 ~ 1 사이의 값이기 때문에 pow를 사용하여 제곱을 해준다. 이렇게 하면 specularShininess가 높을수록 더 뚜렷한 반사가 나타난다.
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
  // vec3 specular = specularStrength * spec * lightColor;

  // use texture only one
  // vec3 specular = spec * light.specular * material.specular;

  // use texture both
  vec3 specular = spec * light.specular * specColor;

  // vec3 result = (ambient + diffuse + specular) * objectColor;
  vec3 result = (ambient + diffuse + specular);
  fragColor = vec4(result, 1.0);
}
