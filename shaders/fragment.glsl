#version 410

out vec4 FragmentColour;

uniform vec3 lightPos = vec3(0,0,10);
uniform vec3 lightColour = vec3(1,1,1);


in vec3 normal_face;
in vec3 fragPos;
in vec3 viewPos;
in vec3 objColour;


void main() {

  // Ambient
  float ambientStrength = 0.1;
  vec3 ambient = ambientStrength * lightColour;

  // Diffuse
  vec3 nFace = normalize(normal_face);
  vec3 vPos = normalize(viewPos);
  vec3 lightDir = normalize(lightPos - fragPos);
  vec3 diffuse = max(dot(normal_face, lightDir),0.0) * objColour;

  // Specular
  float specularStrength = 0.7;
  vec3 viewDir = normalize(vPos - fragPos);
  vec3 reflectDir = reflect(-lightDir, nFace);
  float spec = pow(max(dot(reflectDir, viewDir),0.0), 256.0);
  vec3 specular = specularStrength * spec * lightColour;


  vec3 shade =  ambient +diffuse + specular;

  FragmentColour = vec4(shade, 1.0);
}