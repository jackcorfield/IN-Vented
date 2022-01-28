#version 460 core

in VOutput
{
    vec2 texCoords;
} fInput;

out vec4 color;

uniform sampler2D image;
uniform vec3 effects; // r = shake, g = hdr, b = 
uniform float time;

void main()
{    
    vec4 hdrColor = texture(image, fInput.texCoords);
    vec3 result = hdrColor.xyz;
    if (effects.g == 1.0)
    {
        result = hdrColor.xyz / (hdrColor.xyz + vec3(1.0));
    }

    color = vec4(result, 1.0);
}