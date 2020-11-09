#version 400

// Definition of the geometry shader
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

// Attributes passed from the vertex shader
in vec4 point_colour[];

// Attributes passed with the material file
uniform mat4 projection_mat;
uniform float point_size;

// Attributes passed to the fragment shader
out vec4 frag_colour;

void main(void){

	// Get the position of the point
	vec4 position = gl_in[0].gl_Position;

	// Define the positions of the four vertices that will form a quad 
	vec4 v[4];
	v[0] = vec4(position.x - 0.5*point_size, position.y - 0.5*point_size, position.z, 1.0);
    v[1] = vec4(position.x + 0.5*point_size, position.y - 0.5*point_size, position.z, 1.0);
    v[2] = vec4(position.x - 0.5*point_size, position.y + 0.5*point_size, position.z, 1.0);
    v[3] = vec4(position.x + 0.5*point_size, position.y + 0.5*point_size, position.z, 1.0);

	// Create the new geometry: a quad with four vertices from the vector v
    for (int i = 0; i < 4; i++){
		gl_Position = projection_mat * v[i];
		frag_colour = point_colour[0];
        EmitVertex();
     }

	 EndPrimitive();
}