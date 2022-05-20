#version 460 core

in VOutput
{
    vec2 texCoords;
} fInput;

out vec4 color;

uniform sampler2D image;
uniform vec3 textColor;

void main()
{   
	vec4 sampled = vec4(1.0, 1.0, 1.0, texture(image, fInput.texCoords).r);
    color = vec4(textColor, 1.0) * sampled;
}