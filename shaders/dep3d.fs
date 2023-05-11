#version 330
uniform vec4 couleur;
uniform sampler2D tex;
in vec3 vsoNormal;
in vec2 vsoTexCoord;
out vec4 fragColor;

void main(void) {
  vec3 N = normalize(vsoNormal);
  vec3 L = normalize(vec3(0, 0, -1)); /*vers le bas vers la gauche*/
  float diffuse = dot(N, -L);

  vec4 texColor = texture(tex, vsoTexCoord); // Récupère la couleur de la texture aux coordonnées de texture données

  fragColor = vec4((texColor.rgb * diffuse), couleur.a);
}
