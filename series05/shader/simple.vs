#version 330 core // version 3.3
layout (location = 0) in vec3 aPos; // aPos is the input vertex attribute

void main() {
    gl_Position = vec4(aPos, 1.0); // set position
}
