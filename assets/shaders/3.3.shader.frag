#version 330 core

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;
out vec4 FragColor;

uniform sampler2D texture1;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform bool isSun;

void main() {
    vec3 color = texture(texture1, TexCoords).rgb;

    if (isSun) {
        FragColor = vec4(color, 1.0);
        return;
    }

    vec3 norm     = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff    = max(dot(norm, lightDir), 0.0);

    vec3 ambient  = 0.2 * color;
    vec3 diffuse  = diff  * color;

    FragColor = vec4(ambient + diffuse, 1.0);
}