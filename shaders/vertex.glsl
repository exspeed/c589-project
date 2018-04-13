// ==========================================================================
// Vertex program for barebones GLFW boilerplate
//
// Author:  Sonny Chan, University of Calgary
// Date:    December 2015
// ==========================================================================
#version 410

// location indices for these attributes correspond to those specified in the
// InitializeGeometry() function of the main program
layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec3 VertexColour;
layout(location = 2) in vec3 VertexNormal;

out vec3 object_colour;
out vec3 vert_view_space;
out vec3 world_position;
out vec3 view_position;

uniform mat4x4 Model;
uniform mat4x4 View;
uniform mat4x4 Projection;

void main() {
  //normal_face = (normalize(Model * vec4(VertexNormal, 0.0f))).xyz;

  vert_view_space = vec3(View * Model * vec4(VertexPosition,1));

  world_position = (Model * vec4(VertexPosition, 1.0f)).xyz;

  view_position = -normalize((View * vec4(world_position, 1.0f))).xyz;

  gl_Position = Projection * View * Model * vec4(VertexPosition, 1.0f);

	object_colour = VertexColour;
}
