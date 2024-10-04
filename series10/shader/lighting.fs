#version 330 core
in vec3 normal;
in vec2 texCoord;
in vec3 position;
out vec4 fragColor;

uniform vec3 viewPos;

struct Light {
    vec3 position;
    vec3 direction;
    vec2 cutoff; // 내부 컷오프와 외부 컷오프
    vec3 attenuation;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light light;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
uniform Material material;

void main() {
    vec3 texColor = texture2D(material.diffuse, texCoord).xyz;
    vec3 ambient = texColor * light.ambient;

    float dist = length(light.position - position);
    vec3 distPoly = vec3(1.0, dist, dist*dist);
    float attenuation = 1.0 / dot(distPoly, light.attenuation);
    vec3 lightDir = (light.position - position) / dist;

    vec3 result = ambient;
    float theta = dot(lightDir, normalize(-light.direction)); // 빛의 방향과 물체의 방향 사이의 각도를 내적하여 theta를 구함
    float intensity = clamp( // 내부 컷오프와 외부 컷오프 사이에 theta가 있는지 확인
        (theta - light.cutoff[1]) / (light.cutoff[0] - light.cutoff[1]),
        0.0, 1.0);

    if (intensity > 0.0) { // theta가 cutoff 사이에 있으면 빛을 적용
        vec3 pixelNorm = normalize(normal);
        float diff = max(dot(pixelNorm, lightDir), 0.0);
        vec3 diffuse = diff * texColor * light.diffuse;

        vec3 specColor = texture2D(material.specular, texCoord).xyz;
        vec3 viewDir = normalize(viewPos - position);
        vec3 reflectDir = reflect(-lightDir, pixelNorm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = spec * specColor * light.specular;

        result += (diffuse + specular) * intensity; // intensity를 곱하여 빛의 강도를 조절
    }

    result *= attenuation;
    fragColor = vec4(result, 1.0);
}
