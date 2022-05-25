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
uniform float cameraX;

const int viewSpaceWidth = 1280 / 2;

void main()
{
	float xDiff = cameraX / viewSpaceWidth;
	
    vOutput.texCoords = texCoords;
	vOutput.texCoords.x += xDiff;
	
    gl_Position = vec4(pos * 2.0, depth, 1.0);
}
