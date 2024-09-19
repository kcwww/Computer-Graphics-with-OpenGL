// vertex shader
#version 330 core
layout (location = 0) in vec3 aPos; // 0 번째 attribute 가 정점의 위치

out vec4 vertexColor; // 출력값으로 fragment shader 로 전달할 변수

void main() {
  gl_Position = vec4(aPos, 1.0); // vec3 를 vec4 생성자에 사용
  vertexColor = vec4(0.5, 0.0, 0.0, 1.0); // 어두운 빨간색을 출력값으로
}
