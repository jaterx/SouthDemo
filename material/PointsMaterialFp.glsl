#version 400

// Attributes passed from the geometry shader
in vec4 frag_colour;

// Attributes passed with the material file
uniform vec4 point_colour;


void main (void)
{
	//gl_FragColor = point_colour;
	gl_FragColor = frag_colour;
}