#version 330 core

out float fragColor;

in vec2 texCoord;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D texNoise;


uniform mat4 view;
uniform mat4 projection;

uniform vec2 noiseScale;
uniform float radius;

const int KERNEL_SIZE = 64;
const float BIAS = 0.025;
uniform vec3 samples[KERNEL_SIZE];

void main () {
  vec4 worldPos = texture(gPosition, texCoord);
  if (worldPos.w <= 0.0f) discard;

  vec3 fragPos = (view * vec4(worldPos.xyz, 1.0)).xyz; // view space 로 변환
  vec3 normal = (view * vec4(texture(gNormal, texCoord).xyz, 0.0)).xyz; // view space 로 변환
  vec3 randomVec = texture(texNoise, texCoord * noiseScale).xyz; // 노이즈 텍스쳐에서 랜덤 벡터를 가져옴

  vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal)); // normal 벡터와 수직인 랜덤 벡터를 구함
  vec3 binormal = normalize(cross(normal, tangent)); // normal 벡터와 tangent 벡터의 외적을 구함
  mat3 TBN = mat3(tangent, binormal, normal);

  float occlusion = 0.0;
  for (int i = 0; i < KERNEL_SIZE; i++) {
    vec3 sample = fragPos + TBN * samples[i] * radius; // sample 위치를 계산
    vec4 screenSample = projection * vec4(sample, 1.0); // clip space 로 변환
    screenSample.xyz /= screenSample.w; // homogeneous divide
    screenSample.xyz = screenSample.xyz * 0.5 + 0.5; // NDC to screen space

    float sampleDepth = (view * texture(gPosition, screenSample.xy)).z; // sample depth
    float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth)); // depth 차이를 이용한 범위 체크
    occlusion += (sampleDepth >= sample.z + BIAS ? 1.0 : 0.0) * rangeCheck; // occlusion 계산
  }

  fragColor = 1.0 - occlusion / KERNEL_SIZE; // occlusion 값을 평균내서 최종 색상으로 설정
}

// #version 330 core

// out float fragColor;

// in vec2 texCoord;

// uniform sampler2D gPosition;
// uniform sampler2D gNormal;
// uniform sampler2D texNoise;


// uniform mat4 view;

// uniform vec2 noiseScale;

// void main () {
//   vec4 worldPos = texture(gPosition, texCoord);
//   if (worldPos.w <= 0.0f) discard;

//   vec3 fragPos = (view * vec4(worldPos.xyz, 1.0)).xyz; // view space 로 변환
//   vec3 normal = (view * vec4(texture(gNormal, texCoord).xyz, 0.0)).xyz; // view space 로 변환
//   vec3 randomVec = texture(texNoise, texCoord * noiseScale).xyz; // 노이즈 텍스쳐에서 랜덤 벡터를 가져옴

//   vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal)); // normal 벡터와 수직인 랜덤 벡터를 구함
//   vec3 binormal = normalize(cross(normal, tangent)); // normal 벡터와 tangent 벡터의 외적을 구함
//   mat3 TBN = mat3(tangent, binormal, normal);

//   fragColor = tangent.x;
// }


// #version 330 core

// out float fragColor;

// in vec2 texCoord;

// uniform sampler2D gPosition;
// uniform sampler2D gNormal;

// uniform mat4 view;

// void main () {
//   vec4 worldPos = texture(gPosition, texCoord);
//   if (worldPos.w <= 0.0f) discard;

//   fragColor = (view * vec4(worldPos.xyz, 1.0)).x * 0.1 + 0.5;
// }
