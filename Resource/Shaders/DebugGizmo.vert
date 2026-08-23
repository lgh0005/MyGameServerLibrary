#version 460 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec4 aColor;

layout(std140, binding = 0) uniform CameraData
{
    mat4 view;
    mat4 projection;
    mat4 viewProjection;
    vec4 worldPosition;
};

out vec4 vColor;

void main()
{
    gl_Position = viewProjection * vec4(aPosition, 1.0);
    vColor = aColor;
}