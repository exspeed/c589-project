#version 410

out vec4 FragmentColour;

in vec3 object_colour;
in vec3 normal_face;
in vec3 world_position;
in vec3 view_position;


void main() {
  const float ambient_strength = 0.1;
  const vec3 light_position = vec3(4,3,5);
  const vec3 light_colour = vec3(1,1,1);

  // Ambient
  vec3 ambient = ambient_strength * object_colour;

  // Diffuse
  vec3 light_direction = normalize(light_position - world_position);
  vec3 diffuse = clamp(dot(normal_face, light_direction),0.0, 1.0) * object_colour;

  // Specular
  float specular_strength = 0.7;
  vec3 view_direction = normalize(view_position - world_position);
  vec3 reflect_direction = reflect(-light_direction, normal_face);
  float spec = pow(clamp(dot(view_direction, reflect_direction), 0.0, 1.0), 256.0);
  vec3 specular = specular_strength * spec * light_colour;


  vec3 colour = ambient + diffuse + specular;

  FragmentColour = vec4(colour, 1.0);
}
