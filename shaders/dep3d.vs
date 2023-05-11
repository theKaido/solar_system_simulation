#version 330

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform vec4 couleur;
layout(location = 0) in vec3 vsiPosition;
layout(location = 1) in vec3 vsiNormal;
layout(location = 2) in vec2 vsiTexCoord;

out vec3 vsoNormal;
out vec2 vsoTexCoord;
out float vsoDiffuse;

void main() {
    gl_Position = projectionMatrix * modelViewMatrix * vec4(vsiPosition, 1.0);
    vsoNormal = vsiNormal;
    vsoTexCoord = vsiTexCoord;
    vec3 N = normalize(vsiNormal);
    vec3 L = normalize(vec3(0, 0, -1)); /*vers le bas vers la gauche*/
    vsoDiffuse = max(dot(N, -L), 0.0); // Calcule le facteur de diffuse light
}
