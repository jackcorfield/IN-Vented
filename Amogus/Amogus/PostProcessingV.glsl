#version 460 core

layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 texCoords;

uniform vec3 effects; // r = shake, g = , b = 
uniform float time;

out VOutput
{
    vec2 texCoords;
} vOutput;

void main()
{
    vOutput.texCoords = texCoords;
    gl_Position = vec4(pos * 2.0, 0.0, 1.0);

    if (effects.r > 0.0)
    {
        float strength = 0.01;
        gl_Position.x += cos(time * 50) * strength;        
        gl_Position.y += cos(time * 55) * strength;
    }
}