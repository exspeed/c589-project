
#version 410

// interpolated colour received from vertex stage
in vec3 Colour;

out vec4 FragmentColour;

void main(void)
{
	
	FragmentColour = vec4(0.04, 0.28, 0.26, 1.0);
	
}
