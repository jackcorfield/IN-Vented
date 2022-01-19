#version 460 core

in VOutput
{
    vec2 texCoords;
} fInput;

out vec4 color;

uniform sampler2D image;

void main()
{    
    // Insert fancy post-processing effects here
    
    color = texture(image, fInput.texCoords);
    //color = vec4(0.5, 0.5, 0.5, 1.0);
}