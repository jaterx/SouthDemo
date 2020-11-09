#version 400

// Attributes passed from the geometry shader
in vec4 frag_colour;

// Attributes passed from the vertex shader
in vec3 position_interp;
in vec3 normal_interp;
in vec4 colour_interp;
in vec2 uv_interp;
in vec3 light_pos;

// Attributes passed with the material file
uniform vec4 ambient_colour;
uniform vec4 diffuse_colour;
uniform vec4 specular_colour;
uniform float ambient_amount;
uniform float phong_exponent;
uniform sampler2D diffuse_map;


void main (void)
{
	// Very simple fragment shader, but we can do anything we want here
	// We could apply a texture to the particle, illumination, etc.
	//Phong shading
	vec3 N, L, V, R; //Interpolated normal for fragment, Light-source direction, View direction, Reflection direction

	// Compute Lambertian lighting Id
    N = normalize(normal_interp);

	L = (light_pos - position_interp);
	L = normalize(L);

	float Id = max(dot(N, L), 0.0);
	
	// Compute specular term for Phong shading
	//View Position is the same as its in Bling-Phong
	// Initially: V = (eye_position - position_interp);
	V = - position_interp; // Eye position is (0, 0, 0) in view coordinates
    V = normalize(V);

	// Compute the reflection as it's mirrored direction from light source direction
	//This is the formula for reference, R =  (-L) + 2 * max(dot(L, N), 0.0) * N
	//Using the built in reflect function to calculate the reflection result, and normalize the result
	R = normalize(-reflect(L,N));

	// Compute the angle between V and R
	float spec_angle_cos = max(dot(V, R), 0.0);
	float Is = pow(spec_angle_cos, phong_exponent);
	    
	// Retrieve texture value
	vec4 pixel = texture(diffuse_map, uv_interp);

	// Use texture in determining fragment colour
	//gl_FragColor = pixel;
	gl_FragColor = (ambient_amount + Id)*pixel + Is*specular_colour;
	//gl_FragColor = (ambient_amount + lambertian_amount + specular_amount)*pixel;
	//gl_FragColor = frag_colour;
}