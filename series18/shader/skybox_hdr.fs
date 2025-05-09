#version 330 core

out vec4 fragColor;
in vec3 localPos;

uniform samplerCube cubeMap;
// uniform float roughness; // 임시 추가

void main() {
  vec3 envColor = texture(cubeMap, localPos).rgb;
  // vec3 envColor = textureLod(cubeMap, localPos, roughness * 4).rgb; // 임시 추가

  envColor = envColor / (envColor + vec3(1.0));   // reinhard
  envColor = pow(envColor, vec3(1.0/2.2));    // to sRGB
  fragColor = vec4(envColor, 1.0);
}
