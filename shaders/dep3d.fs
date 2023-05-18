#version 330
uniform sampler2D tex;
in vec2 vsoTexCoord;
in vec3 vsoNormal;
out vec4 fragColor;

void main(void) {
  vec3 N = normalize(vsoNormal);
  
  
  vec3 L1 = normalize(vec3(0, 0, 1)); // lumiere venant de devant
  float diffuse1 = max(dot(N, L1), 0.0);
  vec4 color1 = vec4(texture(tex, vsoTexCoord).rgb * diffuse1, 1.0);

  
  vec3 L2 = normalize(vec3(0,1,0));// lumiere venant d'en haut
  float diffuse2 = max(dot(N, L2), 0.0);
  vec4 color2 = vec4(texture(tex, vsoTexCoord).rgb * diffuse2 , 1.0);

  vec3 L3 = normalize(vec3(0,-1,0));// lumiere venant d'en bas 
  float diffuse3 = max(dot(N,L3),0.0);
  vec4 color3=vec4(texture(tex,vsoTexCoord).rgb * diffuse3 , 1.0); 

  
  fragColor = 0.5 * (color1 + color2+color3);
}
