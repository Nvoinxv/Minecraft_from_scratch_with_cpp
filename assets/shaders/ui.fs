#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D image;
uniform vec4 u_Color;
uniform bool u_HasTexture;
uniform bool u_IsText;

void main()
{
    if (u_HasTexture)
    {
        vec4 texColor = texture(image, TexCoords);
        if (u_IsText) {
            // For generated images, black background should be treated as transparent
            if (texColor.r < 0.1 && texColor.g < 0.1 && texColor.b < 0.1) {
                discard;
            }
        }
        FragColor = texColor * u_Color;
    }
    else
    {
        FragColor = u_Color;
    }
}
