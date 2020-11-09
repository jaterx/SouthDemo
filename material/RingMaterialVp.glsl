#version 400

// Attributes passed automatically by OGRE
in vec3 vertex;
in vec3 normal;
in vec4 colour;
in vec2 uv0;

// Attributes passed with the material file
uniform mat4 world_mat;
uniform mat4 view_mat;
uniform mat4 normal_mat;
uniform vec3 up_vec;
uniform float timer;
uniform vec3 object_colour22;
uniform vec3 light_position;

// Attributes forwarded to the geometry shader
out vec3 vertex_colour;

// Attributes forwarded to the fragment shader
out vec3 position_interp;
out vec3 normal_interp;
out vec4 colour_interp;
out vec2 uv_interp;
out vec3 light_pos;

void main()
{
    // Let time cycle every four seconds
	float circtime2 = timer - 4.0 * floor(timer / 4);
	float t2 = circtime2; // Our time parameter
	
	// Settings for the explosion
	// Could also define these in the material file to have multiple particle systems with different settings
	//it is the initial gravity, the firework need to 
    float grav2 = 8.65; // Initial acceleration that support the firework going upward first
    float slow2 = 0.7; // Allows to slow down the explosion, control the speed of the explosion, in this case it's the original speed v0
	
	// Let's first work in model space (apply only world matrix)
	vec4 position2 = world_mat * vec4(vertex, 1.0); //use the new position (nposition) to calculate the position, this old one is used as initial distance x(0)
	vec4 norm2 = normal_mat * vec4(normal, 1.0);

	//declare the new position according to the formula: x(t) = x(0) + v*t + 0.5*g*t*t
	vec4 nposition2 = world_mat * vec4(vertex, 1.0);
	
	//set the gravity that the firework needs to overcome to earth gravity g=9.8m/s
	grav2 -= 9.8;

	//for position x and z, since it is not influenced by the gravity, leave them as original (do not change the variable name to "position.x, position.z" in order to put them in the same vector 4 "nposition")
	nposition2.x += norm2.x*t2*slow2 - grav2*slow2*up_vec.x*t2*t2;
	//calculate y position directly based on the formula, add a 0.01 constant to simulate air resistance influence (firework particles are light, density and weight can be ignored, thus just add a constant value)
	nposition2.y = norm2.y*t2*slow2 + position2.y + slow2*circtime2 + 0.5*grav2*circtime2*circtime2*up_vec.y - 0.01;
	//for position x and z, since it is not influenced by the gravity, leave them as original (do not change the variable name to "position.x, position.z" in order to put them in the same vector 4 "nposition")
	nposition2.z += norm2.z*t2*slow2 - grav2*slow2*up_vec.z*t2*t2;
	
	// Now apply view transformation
	gl_Position = view_mat * nposition2;
	    
	// Define color of vertex
	vertex_colour = object_colour22; // Uniform color assigned in material file

	position_interp = vec3(view_mat * world_mat * vec4(vertex, 1.0));
	
	normal_interp = vec3(normal_mat * vec4(normal, 0.0));

	colour_interp = colour;

	uv_interp = uv0;

    light_pos = vec3(view_mat * vec4(light_position, 1.0));

}
