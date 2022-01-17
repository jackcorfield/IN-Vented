#version 460 core

layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 texCoords;

out VOutput
{
    vec2 texCoords;
} vOutput;

uniform mat4 model;
uniform mat4 projection;

void main()
{
    vOutput.texCoords = texCoords;
    gl_Position = projection * model * vec4(pos, 0.0, 1.0);
}