#version 460 core

in VOutput
{
    vec2 texCoords;
} fInput;

out vec4 color;

uniform sampler2D image;

void main()
{    
    color = texture(image, fInput.texCoords);
}