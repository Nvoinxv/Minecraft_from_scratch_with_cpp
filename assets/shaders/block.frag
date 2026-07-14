#version 330 core
out vec4 FragColor;

in vec2 v_TexCoord;
in vec3 v_Normal;
in vec3 v_FragPos;

uniform sampler2D u_Texture;
uniform vec3 u_ColorTint = vec3(1.0, 1.0, 1.0);

void main()
{
    vec4 texColor = texture(u_Texture, v_TexCoord);
    if (texColor.a < 0.1)
    {
        discard;
    }

    vec3 norm = normalize(v_Normal);
    float lightFactor = 1.0;
    
    // Minecraft classic face-based directional shading:
    if (abs(norm.y - 1.0) < 0.1) {
        lightFactor = 1.0;  // Top (+Y)
    } else if (abs(norm.y + 1.0) < 0.1) {
        lightFactor = 0.5;  // Bottom (-Y)
    } else if (abs(norm.z) > 0.5) {
        lightFactor = 0.8;  // North/South (+/-Z)
    } else if (abs(norm.x) > 0.5) {
        lightFactor = 0.65; // East/West (+/-X)
    }

    vec3 finalColor = texColor.rgb * lightFactor * u_ColorTint;
    FragColor = vec4(finalColor, texColor.a);
}
