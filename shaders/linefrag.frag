#version 410

out vec4 FragmentColour;

in vec3 object_colour;

void main() {
  FragmentColour = vec4(object_colour, 1.0);
}