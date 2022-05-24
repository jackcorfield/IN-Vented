#version 460 core

in VOutput
{
    vec2 texCoords;
} fInput;

out vec4 color;

uniform sampler2D image;
uniform vec3 tint;

void main()
{
    color = texture(image, fInput.texCoords) * vec4(tint, 1.0);
}