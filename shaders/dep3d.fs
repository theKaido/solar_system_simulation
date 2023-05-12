#version 330
uniform vec4 couleur;
uniform sampler2D tex;
in vec3 vsoNormal;
in vec2 vsoTexCoord;
in float vsoDiffuse; // ajout de la variable d'entrée
out vec4 fragColor;

void main(void) {
  vec4 texColor = texture(tex, vsoTexCoord); // Récupère la couleur de la texture aux coordonnées de texture données

  fragColor = vec4((texColor.rgb * vsoDiffuse), couleur.a);

  fragColor.rgb *= 1.0 - abs(vsoDiffuse); // afficher la valeur de vsoDiffuse dans la couleur
}
