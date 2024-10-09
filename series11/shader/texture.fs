#version 330 core
in vec4 vertexColor;
in vec2 texCoord;
out vec4 fragColor;

uniform sampler2D tex;


void main() {
    // fragColor = texture(tex, texCoord);

    // remove transparent pixels
    vec4 pixel = texture(tex, texCoord);
    if (pixel.a < 0.01) {
        discard;
    }
    fragColor = pixel;
}
