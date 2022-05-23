#version 460 core

in float iXOffSet;

layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 texCoords;

out VOutput
{
    vec2 texCoords;
} vOutput;

uniform float depth;

void main()
{
	vec2 currentCoord = pos + vec2(iXOffSet, 0);
    vOutput.texCoords = texCoords;
    gl_Position = projection * view * model * vec4(currentCoord, depth, 1.0);
}