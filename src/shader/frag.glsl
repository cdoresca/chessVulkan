#version 460

layout(binding = 1) uniform sampler2D texSampler;

layout(location = 2) in vec2 fragTexCoord;


void main() {
    outColor = texture(texSampler, fragTexCoord);
}