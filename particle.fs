#version 330 core

in vec2 TexCoords;
in vec4 ParticleColor;

out vec4 color;

uniform sampler2D sprite;

// vec3 hsv2rgb(vec3 c)
// {
//     vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
//     vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
//     return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
// }

void main()
{
    color = texture(sprite, TexCoords) * ParticleColor;
    // color = texture(sprite, TexCoords);
    // color = vec4(hsv2rgb(vec3(ParticleColor)), 1.0f);
}