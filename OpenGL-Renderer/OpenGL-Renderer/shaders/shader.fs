#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

// texture samplers
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    // Apply transparency to the first texture
	vec4 texColor = texture(texture1, TexCoord);
    if(texColor.a < 0.1)
        FragColor = texture(texture2, TexCoord);
    else
        // Linearly interpolate between both textures (50% texture1, 50% texture2)
        FragColor = mix(texColor, texture(texture2, TexCoord), 0.5);
}