
#version 410

// interpolated colour received from vertex stage
in vec3 Colour;

out vec4 FragmentColour;

void main(void)
{
///127-255-0	
	FragmentColour = vec4(0.5, 1.0, 0, 1.0);
	
}
