#version 460 core

layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 texCoords;

out VOutput
{
    vec2 texCoords;
} vOutput;

uniform mat4 view;
uniform mat4 model;
uniform mat4 projection;

uniform float depth;

void main()
{
    vOutput.texCoords = texCoords;
    gl_Position = projection * view * model * vec4(pos, depth, 1.0);
}