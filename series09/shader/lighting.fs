#version 330 core
in vec3 normal;
in vec2 texCoord;
in vec3 position;
out vec4 fragColor;

struct Light {
  // vec3 direction; // light direction
  vec3 position; // light position
  vec3 attenuation; // light attenuation
  
  // spotlight
  vec3 direction;
  float cutOff;
  
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

  float dist = length(light.position - position); // distance between light and fragment
  vec3 distPoly = vec3(1.0, dist, dist * dist); // 1.0, dist, dist^2 이렇게 하는 이유는 attenuation 식이 1.0 / (1.0 + k1 * d + k2 * d^2) 이기 때문
  
  // distPoly.x * light.attenuation.x + distPoly.y * light.attenuation.y + distPoly.z * light.attenuation.z
  float attenuation = 1.0 / dot(light.attenuation, distPoly); // 내적을 통해 attenuation 계산
  vec3 lightDir = (light.position - position) / dist; // light direction
  
  // spotlight
  float theta = dot(lightDir, normalize(-light.direction)); // spotlight angle
  vec3 result = ambient;

  if (theta > light.cutOff) {
    vec3 pixelNorm = normalize(normal);
    float diff = max(dot(pixelNorm, lightDir), 0.0);
    vec3 diffuse = diff * light.diffuse * texColor;

    // specular lighting texture
    vec3 specColor = texture(material.specular, texCoord).xyz;
    vec3 viewDir = normalize(viewPos - position);
    vec3 reflectDir = reflect(-lightDir, pixelNorm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = spec * light.specular * specColor;

    result += diffuse + specular;
  }

  result *= attenuation;
  fragColor = vec4(result, 1.0);
}


// point light

// // vec3 lightDir = normalize(-light.direction); // light direction 음수는 light direction이 바깥을 향하도록
//   vec3 pixelNorm = normalize(normal);

  
//   float diff = max(dot(pixelNorm, lightDir), 0.0);

//   // use texture
//   vec3 diffuse = diff * light.diffuse * texColor;
//   // specular lighting texture
//   vec3 specColor = texture(material.specular, texCoord).xyz;

//   // specular lighting
//   vec3 viewDir = normalize(viewPos - position);
//   vec3 reflectDir = reflect(-lightDir, pixelNorm);
//   float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
  

//   vec3 specular = spec * light.specular * specColor;

//   // vec3 result = (ambient + diffuse + specular);

//   // attenuation
//   vec3 result = (ambient + diffuse + specular) * attenuation;
  