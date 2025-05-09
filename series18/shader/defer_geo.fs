#version 330 core

layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in vec3 position;
in vec3 normal;
in vec2 texCoord;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
};
uniform Material material;

void main() {
    // store the fragment position vector in the first gbuffer texture
    gPosition = vec4(position, 1.0);
    // also store the per-fragment normals into the gbuffer
    gNormal = vec4(normalize(normal), 1.0);
    // and the diffuse per-fragment color
    gAlbedoSpec.rgb = texture(material.diffuse, texCoord).rgb;
    // store specular intensity in gAlbedoSpec’s alpha component
    gAlbedoSpec.a = texture(material.specular, texCoord).r;
}
