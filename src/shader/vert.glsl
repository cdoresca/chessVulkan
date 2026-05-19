#version 460

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexcoord;

layout(location = 2) out vec2 fragTexCoord;

layout(push_constant,std430) uniform camera{
    mat4 view;
    mat4 proj;
};

layout(set = 0, binding = 0) uniform uniformModel{
    mat4 model;
};

void main(){

    gl_Position = proj * view * model * vec4(inPosition, 1.0f);
    fragTexCoord = inTexcoord;
}
