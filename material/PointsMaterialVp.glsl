#version 400

// Attributes passed automatically by OGRE
in vec3 vertex;
in vec4 colour;

// Attributes passed with the material file
uniform mat4 world_mat;
uniform mat4 view_mat;

// Attributes forwarded to the geometry shader
out vec4 point_colour;

void main()
{   
	// Transform point position
	gl_Position = view_mat * world_mat * vec4(vertex, 1.0);
	point_colour = colour;
}