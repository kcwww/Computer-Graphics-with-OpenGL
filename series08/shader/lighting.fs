#version 330 core
in vec3 normal;
in vec2 texCoord;
in vec3 position;
out vec4 fragColor;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform float ambientStrength;

// specular lighting
uniform float specularStrength; 
uniform float specularShininess;
uniform vec3 viewPos;

void main() {
  vec3 ambient = ambientStrength * lightColor;
  vec3 lightDir = normalize(lightPos - position);
  vec3 pixelNorm = normalize(normal);
  vec3 diffuse = max(dot(pixelNorm, lightDir), 0.0) * lightColor;

  // specular lighting
  vec3 viewDir = normalize(viewPos - position); // direction from fragment to camera
  vec3 reflectDir = reflect(-lightDir, pixelNorm); // direction of reflected light
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), specularShininess); // 0 ~ 1 사이의 값이기 때문에 pow를 사용하여 제곱을 해준다. 이렇게 하면 specularShininess가 높을수록 더 뚜렷한 반사가 나타난다.
  vec3 specular = specularStrength * spec * lightColor;

  vec3 result = (ambient + diffuse + specular) * objectColor;
  fragColor = vec4(result, 1.0);
}
