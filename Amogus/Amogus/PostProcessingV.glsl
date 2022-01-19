#version 460 core

layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 texCoords;

out VOutput
{
    vec2 texCoords;
} vOutput;

void main()
{
    vOutput.texCoords = texCoords;
    gl_Position = vec4(pos * 2.0, 0.0, 1.0);
}