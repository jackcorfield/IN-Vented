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
	vec4 sampled = texture(image, fInput.texCoords);
    color = vec4(textColor, sampled.r);
}