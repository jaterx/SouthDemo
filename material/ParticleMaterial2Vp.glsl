#version 400

// Attributes passed automatically by OGRE
in vec3 vertex;
in vec3 normal;
in vec4 colour;

// Attributes passed with the material file
uniform mat4 world_mat;
uniform mat4 view_mat;
uniform mat4 normal_mat;
uniform vec3 up_vec;
uniform float timer;
uniform vec3 object_colour;

// Attributes forwarded to the geometry shader
out vec3 vertex_colour;


void main()
{
    // Let time cycle every four seconds
	float circtime = timer - 4.0 * floor(timer / 4);
	float t = circtime; // Our time parameter
	float t2 = circtime*1.5;
	float t3 = circtime/2;

	double times = floor(timer / 4);
	
    float grav = 8.65; // Initial gravity that support the firework going upward first
    float slow = 0.7; // Allows to slow down the explosion, control the speed of the explosion, in this case it's the original speed v0
	
	// Let's first work in model space (apply only world matrix)
	vec4 position = world_mat * vec4(vertex, 1.0); //use the new position (nposition) to calculate the position, this old one is used as initial distance x(0)
	vec4 norm = normal_mat * vec4(normal, 1.0);

	//declare the new position according to the formula: x(t) = x(0) + v*t + 0.5*g*t*t
	vec4 nposition = world_mat * vec4(vertex, 1.0);
	
	//set the default gravity that the firework needs to overcome to earth gravity g=9.8m/s
	grav -= 9.8;

	vertex_colour = object_colour; // Uniform color assigned in material file
	
	nposition.x += norm.x*t*slow - grav*slow*up_vec.x*t*t;
	
	nposition.y = norm.y*t*slow + position.y + slow*circtime + 0.5*grav*circtime*circtime*up_vec.y - 0.01;
	
	nposition.z += norm.z*t*slow - grav*slow*up_vec.z*t*t;

	gl_Position = view_mat * nposition;

	
}


