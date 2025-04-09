#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out VS_OUT {
    vec3 fragPos;
    vec3 normal;
    vec2 texCoord;
    vec4 fragPosLight; // 빛 위치에 대한 프래그먼트 위치
} vs_out;

uniform mat4 transform;
uniform mat4 modelTransform;
uniform mat4 lightTransform; // 빛의 변환 행렬

void main() {
    gl_Position = transform * vec4(aPos, 1.0);
    vs_out.fragPos = vec3(modelTransform * vec4(aPos, 1.0)); // 모델 변환을 적용한 월드 좌표계의 위치
    vs_out.normal = transpose(inverse(mat3(modelTransform))) * aNormal;
    vs_out.texCoord = aTexCoord;
    vs_out.fragPosLight = lightTransform * vec4(vs_out.fragPos, 1.0); // 빛의 변환 행렬을 적용한 빛 위치
}
