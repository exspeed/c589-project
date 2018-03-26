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

out vec3 objColour;
out vec3 normal_face;
out vec3 fragPos;
out vec3 viewPos;

uniform mat4x4 Model;
uniform mat4x4 View;
uniform mat4x4 Projection;

void main() {
  normal_face = normalize(vec3(Model*vec4(VertexNormal, 0.0f)));

  fragPos = vec3(View*Model*vec4(VertexPosition, 1.0f));

  viewPos = -fragPos;

  gl_Position = Projection*View*Model*vec4(VertexPosition, 1.0f);

	objColour = VertexColour;
}
