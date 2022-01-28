#version 460 core

in VOutput
{
    vec2 texCoords;
} fInput;

out vec4 color;

uniform sampler2D image;
uniform vec3 effects; // r = shake, g = , b = 
uniform float time;

void main()
{    
    // Insert fancy post-processing effects here
    
    color = texture(image, fInput.texCoords);
}