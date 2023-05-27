#version 330

uniform sampler2D tex;
uniform sampler2D nm;
uniform int use_nm;

in vec2 vsoTexCoord;
in vec3 vsoNormal;

out vec4 fragColor;

void main(void) {
    // Obtenir la normale de la normal map et l'amplifier
    vec3 N = vec3(0.0);
    if (use_nm == 1) {
        N = texture(nm, vsoTexCoord).rgb * vec3(2.0, 2.0, 2.0) - vec3(1.0, 1.0, 1.0);
    } else {
        N = normalize(vsoNormal);
    }
  
    // Calculer l'éclairage pour chaque lumière
    vec3 L1 = normalize(vec3(0, 0, 1));
    float diffuse1 = max(dot(N, L1), 0.0);
    vec4 color1 = vec4(texture(tex, vsoTexCoord).rgb * diffuse1, 1.0);

    vec3 L2 = normalize(vec3(0,1,0));
    float diffuse2 = max(dot(N, L2), 0.0);
    vec4 color2 = vec4(texture(tex, vsoTexCoord).rgb * diffuse2 , 1.0);

    vec3 L3 = normalize(vec3(0,-1,0));
    float diffuse3 = max(dot(N,L3),0.0);
    vec4 color3=vec4(texture(tex,vsoTexCoord).rgb * diffuse3 , 1.0); 

    // Combinaison des couleurs éclairées
    fragColor = 0.5 * (color1 + color2 + color3);
}
