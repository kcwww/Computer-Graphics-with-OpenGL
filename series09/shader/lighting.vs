#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

uniform mat4 transform; // Projection * View * Model
uniform mat4 modelTransform; // local to world

out vec3 normal;
out vec2 texCoord;
out vec3 position; // diffuse 값 계산을 위한 position, world space

void main() {
  gl_Position = transform * vec4(aPos, 1.0);
  normal = (transpose(inverse(modelTransform)) * vec4(aNormal, 0.0)).xyz; // inverse transpose 하는 이유는 scaling 때문 벡터는 scaling 되면 안됨
  texCoord = aTexCoord;
  position = (modelTransform * vec4(aPos, 1.0)).xyz;
}
